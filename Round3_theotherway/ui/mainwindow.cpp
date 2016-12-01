#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Databinding.h"
#include "SupportCanvas3D.h"
#include "CS123XmlSceneParser.h"
#include "camera/CamtransCamera.h"
#include "Settings.h"
#include <math.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow) {
   // Make sure the settings are loaded before the UI
   settings.loadSettingsOrDefaults();

   QGLFormat qglFormat;
   qglFormat.setVersion(4, 3);
   qglFormat.setProfile(QGLFormat::CoreProfile);
   qglFormat.setSampleBuffers(true);
   ui->setupUi(this);
   QGridLayout *gridLayout = new QGridLayout(ui->centralwidget);
   m_canvas3D = new Canvas3D(qglFormat, this);
   gridLayout->addWidget(m_canvas3D, 0, 1);

   // Restore the UI settings
   QSettings qtSettings("ROOM_IN_SPACE", "ROOM_IN_SPACE");
   restoreGeometry(qtSettings.value("geometry").toByteArray());
   restoreState(qtSettings.value("windowState").toByteArray());

   // Resize the window because the window is huge since all docks were visible.
   resize(this->width(), this->height());
//   ui->canvas3D->resize(this->width(), this->height());
//   m_canvas3D->resize(this->width(), this->height());
// Bind the UI elements to their properties in the global Settings object, using binding
// objects to simplify the code.  Each binding object connects to its UI elements and keeps
// the UI and its setting in sync.
   show();
}


MainWindow::~MainWindow() {
   delete ui;
}


void MainWindow::dataBind() {
   // make sure the aspect ratio updates when m_canvas3D changes size
   connect(m_canvas3D, SIGNAL(aspectRatioChanged()), this, SLOT(updateAspectRatio()));
}


void MainWindow::changeEvent(QEvent *e) {
   QMainWindow::changeEvent(e);  // allow the superclass to handle this for the most part...
   ui->retranslateUi(this);
}


void MainWindow::closeEvent(QCloseEvent *event) {
   // Save the settings before we quit
   settings.saveSettings();
   QSettings qtSettings("ROOM_IN_SPACE", "ROOM_IN_SPACE");
   qtSettings.setValue("geometry", saveGeometry());
   qtSettings.setValue("windowState", saveState());
   QMainWindow::closeEvent(event);
}


void MainWindow::settingsChanged() {
   m_canvas3D->settingsChanged();
}


void MainWindow::renderImage() {
   m_canvas3D->update();
   QApplication::processEvents();
}

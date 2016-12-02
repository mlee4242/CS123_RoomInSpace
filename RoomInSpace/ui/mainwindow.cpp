#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vrview.h"
#include "Settings.h"
#include "Databinding.h"
#include <QVBoxLayout>
#include <QSurfaceFormat>
#include <QSettings>
#include <QFileDialog>
#include <QStandardPaths>
#include <QOffscreenSurface>

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent), m_buttonGroup(0),
   ui(new Ui::MainWindow) {
   vr = new VRView(this);

   connect(vr, &VRView::deviceIdentifier, this, &MainWindow::setWindowTitle);
   connect(vr, &VRView::framesPerSecond, this, &MainWindow::showFramerate);

   ui->setupUi(this);
   ui->rightLayout->addWidget(vr);

   connect(vr, &VRView::statusMessage, this, &MainWindow::showStatus);
   dataBind();
}


MainWindow::~MainWindow() {
   delete ui;
   delete vr;
   delete m_buttonGroup;
   foreach(DataBinding * b, m_bindings) delete b;
}


void MainWindow::showFramerate(float fps) {
   ui->fpsLabel->setText(tr("%1 FPS").arg(fps));
}


void MainWindow::showStatus(const QString& message) {
   ui->statusBar->showMessage(message);
}


void MainWindow::dataBind() {
#define BIND(b)                 \
   {                            \
      DataBinding *_b = (b);    \
      m_bindings.push_back(_b); \
   }
   m_buttonGroup = new QButtonGroup;
   BIND(ChoiceBinding::bindRadioButtons(
           m_buttonGroup,
           NUM_WINDOWS_TYPES,
           settings.windowMode,
           ui->radioButton_2,
           ui->radioButton))
#undef BIND
}

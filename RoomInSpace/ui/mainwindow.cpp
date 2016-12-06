#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vrview.h"
#include "Settings.h"
#include <QVBoxLayout>
#include <QSurfaceFormat>
#include <QSettings>
#include <QFileDialog>
#include <QStandardPaths>
#include <QOffscreenSurface>

MainWindow::MainWindow(QWidget *parent) :
   QMainWindow(parent),
   ui(new Ui::MainWindow) {
   vr = new VRView(this);

   connect(vr, &VRView::deviceIdentifier, this, &MainWindow::setWindowTitle);
   connect(vr, &VRView::framesPerSecond, this, &MainWindow::showFramerate);

   ui->setupUi(this);
   ui->rightLayout->addWidget(vr);

   connect(vr, &VRView::statusMessage, this, &MainWindow::showStatus);
}


MainWindow::~MainWindow() {
   delete ui;
   delete vr;
}


void MainWindow::showFramerate(float fps) {
   ui->fpsLabel->setText(tr("%1 FPS").arg(fps));
}


void MainWindow::showStatus(const QString& message) {
   ui->statusBar->showMessage(message);
}

void MainWindow::on_overUnderButton_clicked() {
   if (ui->overUnderButton->isChecked()) {
      settings.windowMode = OverUnder;
   }else{ settings.windowMode = SideBySide; }
}


void MainWindow::on_sideSideButton_clicked(){}

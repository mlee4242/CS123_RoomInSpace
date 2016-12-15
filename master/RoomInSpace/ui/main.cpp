#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include "Settings.h"
#include <chrono>
int main(int argc, char *argv[]) {
   QCoreApplication::setOrganizationName("Brown University");
   QCoreApplication::setOrganizationDomain("cs.brown.edu");
   QCoreApplication::setApplicationName("Room In Space");
   QSurfaceFormat glFormat;
   glFormat.setVersion(4, 1);
   glFormat.setProfile(QSurfaceFormat::CoreProfile);
   //glFormat.setSamples(0);
   glFormat.setSwapInterval(0);
   glFormat.setOption(QSurfaceFormat::DebugContext);
   QSurfaceFormat::setDefaultFormat(glFormat);
   QApplication a(argc, argv);
   QMessageBox  msgBox;
   msgBox.setWindowTitle("Room in Space");
   msgBox.setText("Run it in VR mode?");
   msgBox.setStandardButtons(QMessageBox::Yes);
   msgBox.addButton(QMessageBox::No);
   msgBox.setDefaultButton(QMessageBox::No);
   if (msgBox.exec() == QMessageBox::Yes) {
      settings.VRMode = true;
   }else {
      settings.VRMode = false;
   }
   MainWindow w;
   w.show();
   return a.exec();
}

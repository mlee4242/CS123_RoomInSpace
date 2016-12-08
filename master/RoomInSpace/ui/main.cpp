#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>
#include <QSettings>
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
   MainWindow   w;
   w.show();

   return a.exec();
}

#include <QApplication>
#include <iostream>
#include "utilities/Settings.h"
#include "CMainApplication.h"
#include <QThread>
//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int main(int argc, char *argv[]) {
   QApplication app(argc, argv);
   settings.loadSettingsOrDefaults();

   QSurfaceFormat format;
   format.setSamples(16);


   CMainApplication pMainApplication(argc, argv);
   pMainApplication.setFormat(format);
   pMainApplication.BInit();
   pMainApplication.resize(640, 480);
   pMainApplication.RunMainLoop();


   return app.exec();
}

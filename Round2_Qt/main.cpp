//#include <QApplication>
//#include "mainwindow.h"

//int main(int argc, char *argv[])
//{
//    QApplication app(argc, argv);
//    MainWindow w;
//    bool startFullscreen = false;

//    w.show();

//    if (startFullscreen) {
//        // We cannot use w.showFullscreen() here because on Linux that creates the
//        // window behind all other windows, so we have to set it to fullscreen after
//        // it has been shown.
//        w.setWindowState(w.windowState() | Qt::WindowFullScreen);
//    }

//    return app.exec();
//}


#include "CMainApplication.h"


//-----------------------------------------------------------------------------
// Purpose:
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    CMainApplication pMainApplication( argc, argv );

    if (!pMainApplication.BInit())
    {
        pMainApplication.Shutdown();
        return 1;
    }

    pMainApplication.RunMainLoop();

    pMainApplication.Shutdown();

    return 0;
}

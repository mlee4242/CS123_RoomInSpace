#include <QApplication>
#include "mainwindow.h"


int main(int argc, char *argv[]) {
   QApplication app(argc, argv);
   MainWindow   w;
   w.setWindowTitle("Final Project: A Room in Space");
   w.show();
   return app.exec();
}

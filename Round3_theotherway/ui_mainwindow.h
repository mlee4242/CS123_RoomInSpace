/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
   QAction *actionNew;
   QAction *actionOpen;
   QAction *actionSave;
   QAction *actionQuit;
   QAction *actionRevert;
   QAction *actionCopy3Dto2D;
   QAction *actionClear;
   QWidget *canvas3D;
   QGridLayout *gridLayout_10;

   void setupUi(QMainWindow *MainWindow) {
      if (MainWindow->objectName().isEmpty()) {
         MainWindow->setObjectName(QStringLiteral("MainWindow"));
      }
      MainWindow->setEnabled(true);
      MainWindow->setDockNestingEnabled(true);
      MainWindow->setDockOptions(QMainWindow::AllowNestedDocks | QMainWindow::AllowTabbedDocks | QMainWindow::AnimatedDocks | QMainWindow::ForceTabbedDocks | QMainWindow::VerticalTabs);
      canvas3D = new QWidget(MainWindow);
      canvas3D->setObjectName(QStringLiteral("canvas3D"));
      MainWindow->setCentralWidget(canvas3D);

      retranslateUi(MainWindow);
      QMetaObject::connectSlotsByName(MainWindow);
   }  // setupUi

   void retranslateUi(QMainWindow *MainWindow) {
      MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Final Project: Room in Space", 0));
   }  // retranslateUi
};

namespace Ui {
class MainWindow : public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

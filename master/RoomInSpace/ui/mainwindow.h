#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QButtonGroup>
#include <QMainWindow>
#include <QMatrix4x4>
namespace Ui {
class MainWindow;
}

class VRView;
class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();

protected slots:
   void showFramerate(float fps);
   void showStatus(const QString& message);

private slots:
   void on_overUnderButton_clicked();

   void on_sideSideButton_clicked();

private:
   Ui::MainWindow *ui;
   VRView *vr;
};

#endif // MAINWINDOW_H

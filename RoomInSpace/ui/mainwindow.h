#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QButtonGroup>
#include <QMainWindow>
#include <QMatrix4x4>
namespace Ui {
class MainWindow;
}

class VRView;
class DataBinding;

class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   explicit MainWindow(QWidget *parent = 0);
   ~MainWindow();

protected slots:
   void showFramerate(float fps);
   void showStatus(const QString& message);

private:
   void dataBind();

   QButtonGroup *m_buttonGroup;
   Ui::MainWindow *ui;
   VRView *vr;
   QList<DataBinding *> m_bindings;
};

#endif // MAINWINDOW_H

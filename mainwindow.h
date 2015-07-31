#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    //void paintEvent(QPaintEvent *);
    void grab_start();

    ~MainWindow();

private:
    void grabAndShow();
    Ui::MainWindow *ui;
    QImage qImg;
private slots:
  // void on_pushButton_start_clicked();
    void paintEvent(QPaintEvent *);

};

#endif // MAINWINDOW_H

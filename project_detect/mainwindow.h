#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<cv.h>
//#include "ui_mainwindow.h"
#include<opencv2/opencv.hpp>
#include "highgui.h"

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void init_win();
     void show_camera(Mat frame,int LabelNum);
 //    void update(int num, int worn_cell, double worn_rate);
     void update_table(int num,  double worn_rate[],float thre,int flag);
     void Table2ExcelByTxt(QTabWidget *table);
     void show_result(int LabelNum);

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();
    /*
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    */

    void on_horizontalSlider_sliderPressed();

    void on_horizontalSlider_sliderReleased();

    void on_horizontalSlider_sliderMoved(int position);

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();
    void detect();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

#include "mainwindow.h"
#include <QApplication>
#include <cv.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <detect.h>
#include <wiringPi.h>
#include <locale>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <iostream>
#include<sys/stat.h>
#include<unistd.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <streambuf>
using namespace std;
using namespace cv;
extern bool key1,key2;
Mat frame;
extern int cell ;
extern int count_cell ;
extern double area_rate;
extern int exit_flag;
ofstream oFile;
int thres;
int fd;
int main(int argc, char *argv[])
{
    if ((fd = serialOpen("/dev/ttyUSB0", 115200)) < 0)

    {
        fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));

    }


    oFile.open("/home/pi/Desktop/outputdata.csv",ios::out |ios::trunc);
    locale::global(locale(""));

    //ifstream readfile("/home/pi/Desktop/outputdata.csv");
  //  getline(readfile,tmp);
    //cout<<tmp<<endl;
  //  if(tmp.empty())
    {
    oFile<<"日期"<<","<<"时间"<<","<<"批次"<<","<<"颜色"<<","<<"磨损阈值"<<","<<"总格子数"<<","<<"总磨损率"<<","<<"磨损格子/总格子"<<",";
    for(int i = 1;i<=100;i ++)
    {
        oFile<<"第"<<i<<"个格子"<<",,";
    }
    oFile<<endl;
    }
    QApplication a(argc, argv);

    MainWindow w;
    w.init_win();
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.showFullScreen();
    w.show_result(0);
    w.show_result(4);

    w.show();
    VideoCapture capture(0);
    Mat init_pic = imread("/home/home/pi/test_picture/test2.jpg");//
    resize(init_pic,init_pic,Size(init_pic.cols/2,init_pic.rows/2));
    w.show_camera(init_pic,1);
    w.show_camera(init_pic,2);


    while(1)
    {
    while(key1)
    {

            Mat aa;
            frame = imread("/home/home/pi/test_picture/test3.jpg");
            capture>>frame;  //读取当前帧
            resize(frame,aa,Size(frame.cols/2,frame.rows/2));
            w.show_camera(aa,1);
            waitKey(300);
            if(serialDataAvail(fd) > 0)
            {
                char rec =serialGetchar(fd);
                if(rec == 'y')
                {
                    w.show_result(4);
                }
                //cout<<rec<<endl;
                serialFlush(fd);
            }
            if(exit_flag==1)
            {
               oFile.close();

               a.closeAllWindows();
               a.exit();
               a.quit();
               QApplication::exit();
               capture.release();
               return 0;

            }

    }
    if(serialDataAvail(fd) > 0)
    {
        char rec =serialGetchar(fd);
        if(rec == 'y')
        {
            w.show_result(4);
        }
        //cout<<rec<<endl;
        serialFlush(fd);
    }
    if(exit_flag==1)
    {
       oFile.close();
       a.closeAllWindows();
       a.exit();
       a.quit();
       QApplication::exit();
       capture.release();
       return 0;

    }
    waitKey(300);
    }
    //QObject::connect(&quit,SIGNAL(on_pushButton_7_clicked()),&a,SLOT(a.exit()));
    return a.exec();
}

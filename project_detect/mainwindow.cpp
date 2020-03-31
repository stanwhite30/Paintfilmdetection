#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include <fstream>
#include <iostream>
#include <streambuf>
#include "detect.h"
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <QTextCodec>
#include <convert.h>

using namespace std;
bool key1=0,key2=0;
extern Mat frame;
extern int fd;
extern int cell_count ;
int exit_flag=0;
extern double area;
int flag_ad=1;
extern double worn_area;
int worn_cell=0;
extern double total_rate;
extern double worn_area_rate[100];
extern ofstream oFile;
extern Mat resultImg;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_3_clicked()
{

    if(key1==0)
    {
        key1=1;
        ui->tableWidget->clearContents();
    }
    else
    {
        key1 = 0;
        key2= 0;
    }


}

void MainWindow::show_camera(Mat pic,int LabelNum)
{
    QPalette a;
    //a.background();
    QImage showImage((const uchar*)pic.data,pic.cols,pic.rows,pic.cols*pic.channels(),QImage::Format_RGB888);
    ui->m_displayLabel->setVisible(false);
    //ui->label_13->setPalette(a);
    if(LabelNum==1)
    {
        ui->label->setPixmap(QPixmap::fromImage(showImage)); //ÃÂ¥ÃÂ°ÃÂÃÂ¥ÃÂÃÂ¾ÃÂ§ÃÂÃÂÃÂ¦ÃÂÃÂ¾ÃÂ§ÃÂ¤ÃÂºÃÂ¥ÃÂÃÂ¨QLabelÃÂ¤ÃÂ¸ÃÂ
    }
    if(LabelNum == 2)
    {
        ui->label_2->setPixmap(QPixmap::fromImage(showImage)); //ÃÂ¥ÃÂ°ÃÂÃÂ¥ÃÂÃÂ¾ÃÂ§ÃÂÃÂÃÂ¦ÃÂÃÂ¾ÃÂ§ÃÂ¤ÃÂºÃÂ¥ÃÂÃÂ¨QLabelÃÂ¤ÃÂ¸ÃÂ
    }
    //ui->groupBox->setBackgroundRole();
}

void MainWindow::show_result(int LabelNum)
{
    if(LabelNum == 0)
    {
 //   ui->label_11->setVisible(false);
   // ui->label_10->setVisible(false);
    ui->label_14->setVisible(false);
    ui->label_13->setVisible(false);
    }
    if(LabelNum == 1)
    {
        ui->label_11->setVisible(true);
 //       ui->label_10->setVisible(false);
    }
    if(LabelNum == 2)
    {
 //       ui->label_10->setVisible(true);
 //       ui->label_11->setVisible(false);
    }
    if(LabelNum == 3)
    {
        ui->label_13->setVisible(true);
        ui->label_14->setVisible(false);
    }
    if(LabelNum == 4)
    {
        ui->label_14->setVisible(true);
        ui->label_13->setVisible(false);
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    if(key1)
    {
    key2=1;
    show_result(0);
    show_result(4);
    static int flag=0;
    ui->tableWidget->clearContents();
    if(key1)
    {
        //key1=0;
        flag=0;

    }
    Mat test;
    int thres;
    //Mat test  = imread("/home/pi/project_pic/0.jpg");

   // Mat test  = imread("/home/home/pi/test_picture/test3.jpg");
    //test.copyTo(frame);
    frame.copyTo(test);
    cvtColor(test,test,CV_RGB2GRAY);
    thres=OTSU(test);
    cout<<thres<<endl;
    //change(test,test,150,0);
    //unevenLightCompensate(test,16);
    //test.copyTo(frame);
    test= hough_detect(test,thres);
    //imshow("test",test);
    cv::resize(test,test,Size(test.cols/2,test.rows/2));

    show_camera(test,2);

    //frame = detect_pic(frame,thres);
    //imshow("frame",frame);

   // cv::resize(resultImg,resultImg,Size(resultImg.cols/2,resultImg.rows/2));
   // show_camera(resultImg,2);


    char string1[5];
  //  double worn_rate = 1-(worn_area*1.0/area);

    thres=int((1-(thres*1.0/255))*100);
    if(thres>=20)
    {
        thres=20;
    }
    if(flag==0)
    {

        ui->horizontalSlider->setValue(thres);
        sprintf(string1,"%d%%",thres);
        ui->label_17->setText(string1);
        sprintf(string1,"%d%%",thres);
        ui->m_displayLabel->setText(string1);

    }
    cout<<thres<<endl;

//格子数 磨损率
    sprintf(string1,"%d",cell_count);
    ui->lineEdit_2->setText(string1);
    sprintf(string1,"%.2f%%",total_rate*100);
    ui->lineEdit_3->setText(string1);

    int threshold_worn;
    threshold_worn= ui->horizontalSlider->value();
    //cout<<threshold_worn<<endl;

    update_table(cell_count,worn_area_rate,threshold_worn*1.0/100,flag);

    sprintf(string1,"%d/%d",worn_cell,cell_count);
    ui->lineEdit_4->setText(string1);

    //rank
    if(total_rate*100>0||(worn_cell*1.0/cell_count)*100>0)
    {
        if(total_rate*100>5|| (worn_cell*1.0/cell_count)*100>5)
        {
            if(total_rate*100>15 || (worn_cell*1.0/cell_count)*100>15)
            {
                if(total_rate*100>35 || (worn_cell*1.0/cell_count)*100>35)
                {
                    if(total_rate*100>65||(worn_cell*1.0/cell_count)*100>65)
                    {
                        ui->label_11->setText(QString("0级"));
                    }
                    else
                    {
                    ui->label_11->setText(QString("I级"));

                    }
                }
                else
                {
                ui->label_11->setText(QString("II级"));

                }
            }

            else
            {
            ui->label_11->setText(QString("III级"));

            }
        }
        else
        {
        ui->label_11->setText(QString("IV级"));

        }
    }
    else
    {
        //show_result(2);

        ui->label_11->setText(QString("V级"));
    }
    flag ++ ;

    area=0;
    worn_area=0;

    }
}

void MainWindow::update_table(int num,  double worn_rate[], float thre,int flag)
{
    worn_cell=0;
    for(int i = 0; i<num;i++)
    {
        if(flag==0&&(num>=ui->tableWidget->rowCount())){
        ui->tableWidget->insertRow(i);
        }

        char string1[5];
        sprintf(string1,"%.2f%%",worn_rate[i+1]*100,"%");
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(string1));
        if(worn_rate[i+1]>=thre)
        {
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(("不合格")));
            worn_cell++;

        }
        else{
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(("合格")));
        }
    }
}



void MainWindow::on_horizontalSlider_sliderPressed()
{

    if(!ui->m_displayLabel->isVisible())
    {
        char string1[5];
        sprintf(string1,"%d%%",ui->horizontalSlider->value());
        ui->m_displayLabel->setVisible(true);
        ui->m_displayLabel->setText(string1);
        ui->label_17->setText(string1);
    }
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    if(ui->m_displayLabel->isVisible())
    {
        ui->m_displayLabel->setVisible(false);
    }
    if(key2)
    {
    detect();
    }
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    int  p,h;
   char string1[10];
   //ui->m_displayLabel->setVisible(true);

    sprintf(string1,"%d%%",position);

    ui->m_displayLabel->setText(string1);
    ui->label_17->setText(string1);
    //cout<<position<<endl;
   // ui->m_displayLabel->move((ui->horizontalSlider->width()-ui->m_displayLabel->width())*position/(ui->horizontalSlider->maximum()-ui->horizontalSlider->minimum()),3);
   p = ui->horizontalSlider->width()/100*position + ui->horizontalSlider->pos().rx();
   h = ui->horizontalSlider->pos().ry()-10;
   ui->m_displayLabel->move(p,h);
}

void MainWindow::on_pushButton_clicked()
{
    if(key2)
    {
    char str[30];
    time_t tt;
    time( &tt );
    tt = tt + 8*3600;  // transform the time zone
    tm* t= gmtime( &tt );


    sprintf(str,"%d-%02d-%02d,%02d:%02d:%02d",
              t->tm_year + 1900,
              t->tm_mon + 1,
              t->tm_mday,
              t->tm_hour,
              t->tm_min,
              t->tm_sec);

    //cout<<str<<endl;
    //int color = ui->comboBox->currentIndex();
    String color = ui->comboBox->currentText().toStdString();
    string num1 = ui->comboBox_2->currentText().toStdString();
    string num2 = ui->comboBox_3->currentText().toStdString();
    string num3 = ui->comboBox_4->currentText().toStdString();
    int threshold= ui->horizontalSlider->value();

    char temp[10];
    char* color_t = const_cast<char*>(color.c_str());
    utf8togb2312(color_t,strlen(color_t),temp,10);

    oFile<<str<<","<<num1<<num2<<num3<<","<<temp<<","<<threshold<<"%,"<<cell_count<<","<<total_rate*100<<","<<worn_cell<<"/"<<cell_count<<",";
    for(int i = 0 ; i< cell_count; i ++)
    {
        string data = ui->tableWidget->item(i,0)->text().toStdString();
        oFile<<data<<",";

        string data_s = ui->tableWidget->item(i,1)->text().toStdString();
        char temp[10];

        char* p = const_cast<char*>(data_s.c_str());

        utf8togb2312(p,strlen(p),temp,10);



        oFile<<temp<<",";
    }
    oFile<<endl;
    //oFile.close();
 //key2 = 0;
}

}

void MainWindow::on_pushButton_4_clicked()
{
    if(flag_ad==1)
    {
    show_result(3);
    //ui->label_14->setVisible(false);
    serialPuts(fd, "curve");
    //cout<<"OK"<<endl;

    //while(serialDataAvail(fd) < 0) ;
    //cout<<char(serialGetchar(fd))<<endl;
    //serialFlush(fd);
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    static bool times = 1;
    if(times)
    {
        serialPuts(fd, "light_on");
        //cout<<"light_on"<<endl;
        times=0;
    }
    else{
        serialPuts(fd, "light_off");
        //cout<<"light_off"<<endl;
        times=1;
    }
}


void MainWindow::on_pushButton_6_clicked()
{


    if(flag_ad==1)
    {
        serialPuts(fd, "adjust_on");
        //cout<<"adjust_on"<<endl;
        show_result(3);
        flag_ad=0;
    }
    else{
        serialPuts(fd, "adjust_off");
        //cout<<"adjust_off"<<endl;
        show_result(4);
        flag_ad=1;
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    if(exit_flag==0)
    {
        exit_flag=1;
         serialPuts(fd, "light_off");
    }
    else exit_flag = 0;

}
void MainWindow::init_win()
{
    char string1[10];
    sprintf(string1,"12rf");
    ui->comboBox_2->addItem(tr("string1"));
}

void MainWindow::detect()
{
    int threshold_worn;
    char string1[10];
    threshold_worn= ui->horizontalSlider->value();
    //cout<<threshold_worn<<endl;

    update_table(cell_count,worn_area_rate,threshold_worn*1.0/100,1);

    sprintf(string1,"%d/%d",worn_cell,cell_count);
    ui->lineEdit_4->setText(string1);
    if(total_rate*100>0||(worn_cell*1.0/cell_count)*100>0)
    {
        if(total_rate*100>5|| (worn_cell*1.0/cell_count)*100>5)
        {
            if(total_rate*100>15 || (worn_cell*1.0/cell_count)*100>15)
            {
                if(total_rate*100>35 || (worn_cell*1.0/cell_count)*100>35)
                {
                    if(total_rate*100>65||(worn_cell*1.0/cell_count)*100>65)
                    {
                        ui->label_11->setText(QString("0级"));
                    }
                    else
                    {
                    ui->label_11->setText(QString("I级"));

                    }
                }
                else
                {
                ui->label_11->setText(QString("II级"));

                }
            }

            else
            {
            ui->label_11->setText(QString("III级"));

            }
        }
        else
        {
        ui->label_11->setText(QString("IV级"));

        }
    }
    else
    {
        //show_result(2);

        ui->label_11->setText(QString("V级"));
    }
}




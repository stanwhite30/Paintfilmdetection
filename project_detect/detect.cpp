#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <cv.hpp>
#include <detect.h>

using namespace std;
using namespace cv;


Mat resultImg;
void unevenLightCompensate(Mat &image, int blockSize)
{
    if (image.channels() == 3) cvtColor(image, image, 7);
    double average = mean(image)[0];
    int rows_new = ceil(double(image.rows) / double(blockSize));
    int cols_new = ceil(double(image.cols) / double(blockSize));
    Mat blockImage;
    blockImage = Mat::zeros(rows_new, cols_new, CV_32FC1);
    for (int i = 0; i < rows_new; i++)
    {
        for (int j = 0; j < cols_new; j++)
        {
            int rowmin = i*blockSize;
            int rowmax = (i + 1)*blockSize;
            if (rowmax > image.rows) rowmax = image.rows;
            int colmin = j*blockSize;
            int colmax = (j + 1)*blockSize;
            if (colmax > image.cols) colmax = image.cols;
            Mat imageROI = image(Range(rowmin, rowmax), Range(colmin, colmax));
            double temaver = mean(imageROI)[0];
            blockImage.at<float>(i, j) = temaver;
        }
    }
    blockImage = blockImage - average;
    Mat blockImage2;
    resize(blockImage, blockImage2, image.size(), (0, 0), (0, 0), INTER_CUBIC);
    Mat image2;
    image.convertTo(image2, CV_32FC1);
    Mat dst = image2 - blockImage2;
    dst.convertTo(image, CV_8UC1);
}
void change(Mat src,Mat dst,int g_nContrastValue, int g_nBrightValue)
{
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {

            dst.at<uchar>(y, x) = saturate_cast<uchar>((g_nContrastValue*0.01)*(src.at<uchar>(y, x)) + g_nBrightValue);

        }
    }
}

float loss_count(Mat input,int thres)
{
    int loss = 0;
    //int area = input.rows * input.cols;
    int temp = 180;
    if(temp<=thres)
        temp=thres;
    for (int y = 0; y < input.rows; y++)
    {
        for (int x = 0; x < input.cols; x++)
        {

            if (input.at<uchar>(y, x) >=thres)
            {
                loss++;
            }

        }
    }
  //  float percent = loss*1.0 / area;

    return loss;
}

double area=0;
int cell_count=1;
double worn_area_rate[100];

double worn_area=0;
double total_rate=0;
vector<vector<Point>> contours_hough;
Mat hough_detect(Mat input,int thres)
{
    int cout_roi = 1;
    cell_count= 1;
    Mat srcImg = Mat::zeros(input.size(), input.type());
    int Absolute_offset = 1;
     Mat element = getStructuringElement(MORPH_RECT, Size(Absolute_offset * 2 + 1, Absolute_offset * 2 + 1), Point(Absolute_offset, Absolute_offset));

    //int thres=OTSU(input);
    input.copyTo(srcImg);
    Mat midImage, dstImage(srcImg.size(), CV_8U, Scalar(0));//临时变量和目标图的定义
    if(thres<=200)
    {
    //change(input, srcImg, 200, 0);
    //blur(srcImg,srcImg,Size(3,3));
   // Canny(srcImg, midImage, (thres), (thres)*3, 3);//进行一此canny边缘检测
    }

    //blur(srcImg,srcImg,Size(3,3));
    //medianBlur(srcImg,srcImg,3);
    // imshow("change",srcImg);

  //  else{
    Canny(srcImg, midImage, thres, thres*3, 3);//进行一此canny边缘检测
    //}
    morphologyEx(midImage, midImage, MORPH_DILATE, element);
    //cvtColor(test, dstImage, COLOR_GRAY2BGR);//转化边缘检测后的图为灰度图
    //imshow("midImage",midImage);

    //【3】进行霍夫线变换
    vector<Vec2f> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
    HoughLines(midImage, lines, 1, CV_PI / 180, 255, 0, 0);

    //【4】依次在图中绘制出每条线段
    for (size_t i = 0; i < lines.size(); i++)
    {
        float rho = lines[i][0], theta = lines[i][1];
        Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000 * (-b));
        pt1.y = cvRound(y0 + 1000 * (a));
        pt2.x = cvRound(x0 - 1000 * (-b));
        pt2.y = cvRound(y0 - 1000 * (a));
        //line(dstImage, pt1, pt2, Scalar(255, 255, 255), 1, 16);
        if (abs(pt1.y-pt2.y)<=50||abs(pt1.x-pt2.x)<=50)
        {
            line(dstImage, pt1, pt2, Scalar(255, 255, 255), 1);
        //    line(midImage , pt1, pt2, Scalar(255, 255, 255), 1);
        }
    }
    //cvtColor(dstImage, test, COLOR_BGR2GRAY);//转化边缘检测后的图为灰度图
    Mat test2(srcImg.size(), CV_8UC3, Scalar(0, 0, 0));
    //imshow("dst",dstImage);
//    imshow("mid",midImage);
    //midImage = detect_pic(midImage);
    findContours(dstImage, contours_hough, RETR_LIST, CV_CHAIN_APPROX_TC89_KCOS);
    //cout << contours_hough.size() << endl;
    cvtColor(input, test2, COLOR_GRAY2RGB);//转化边缘检测后的图为灰度图
                                              //计算轮廓的面积
    for (int i = 0; i < (int)contours_hough.size(); i++)
    {
        double g_dConArea = contourArea(contours_hough[i], false);

        if ((g_dConArea >= 1000) && (g_dConArea <= 5000))
        {

            int tempx = 0, tempy = 0;
            for (int j = 0; j < contours_hough[i].size(); j++)
            {

             tempx = contours_hough[i][j].x + tempx;
             tempy = contours_hough[i][j].y + tempy;
            }
             Point center;
             center.x = tempx / contours_hough[i].size()-20;
             center.y = tempy / contours_hough[i].size();

             tempx = 0;
             tempy = 0;

            char text[10];
           // cout << "【用轮廓面积计算函数计算出来的第" << i << "个轮廓的面积】" << g_dConArea << endl;
            sprintf(text, "%d", cell_count);

            putText(test2, text, center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 0, 0), 1, 1);
            putText(midImage, text, center, FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 0, 0), 1, 1);
            area = g_dConArea + area;
            drawContours(test2, contours_hough, i, Scalar(0, 255, 0), 2);

/*
            Mat test3(srcImg.size(), CV_8U, Scalar(0));
            drawContours(test3, contours_hough, i, Scalar(255), CV_FILLED);
            //imshow("test3", test3);
            Mat image;
            srcImg.copyTo(image, test3);
            drawContours(image, contours_hough, i, Scalar(0), 1, 16);
*/

            //imshow("srcImg",srcImg);
            Rect rect = boundingRect(Mat(contours_hough[i]));
            Mat roi = input(rect);
            Mat image_test;
            int roi_thres;

            roi.convertTo(image_test, roi.type());
            Mat roi_temp(image_test.size(), CV_8U, Scalar(0));
            image_test.copyTo(roi_temp);
            //change(roi_temp,roi_temp ,200,0);
            blur(image_test,image_test,Size(9,9));
            roi_thres = OTSU(roi_temp);


          //cvtColor(roi, roi, COLOR_BGR2GRAY);//转化边缘检测后的图为灰度图
         // change(image_test,image_test ,255,0);

           char aa[10];
           sprintf(aa,"%d",cout_roi);
        //   cout<<aa<<" roi thres:  "<<roi_thres<<endl;
           cout_roi++;

        //   cout<<thres<<endl;
        /*
           imshow("roi",roi_temp);
           while(1)
            {
                if(waitKey(0)!=0)
                    break;
            }
*/

            if(abs(thres-roi_thres)>=20)
            {
              roi_thres = thres;
            }
            else roi_thres= 180;
       //     cout<<roi_thres<<endl;


            double worn_local_area = loss_count(image_test,roi_thres);
          //  cout<<"worn_area: "<<worn_local_area<<endl;
          //  cout<<"area: "<<g_dConArea<<endl;

            worn_area = worn_local_area + worn_area;
            //cout << cell_count << "  磨损比  " << worn_local_area*1.0 / g_dConArea << endl;
            worn_area_rate[cell_count]= worn_local_area*1.0 / g_dConArea;
            if(worn_area_rate[cell_count]>=1)
            {
                worn_area_rate[cell_count] = 1;
            }

            cell_count++;
        }

    }
    total_rate = worn_area/area;
    cell_count = cell_count -1;
    midImage.copyTo(resultImg);
//imshow("midImage",midImage);
    return test2;
}

int OTSU(Mat grayImage)
{
    int nRows = grayImage.rows;//行数
    int nCols = grayImage.cols;//列数
    int threshold = 0;  //初始化阈值
                        //------------【1】初始化统计参数------------
    int nSumPix[256];   //用于灰度级中每个像素在整幅图像中的个数
    float nProDis[256]; //用于记录每个灰度级占图像中的概率分布
    for (int i = 0; i < 256; i++)
    {
        nSumPix[i] = 0;
        nProDis[i] = 0;
    }
    //------------【2】统计灰度级中每个像素在整幅图像中的个数------------
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            nSumPix[(int)grayImage.at<uchar>(i, j)]++;
        }
    }
    //------------【3】计算每个灰度级占图像中的概率分布------------
    for (int i = 0; i < 256; i++)
    {
        nProDis[i] = (float)nSumPix[i] / (nCols*nRows);
    }
    //------------【4】遍历灰度级[0,255]，计算出最大类间方差的阈值------------
    float wb, wf; //比重. wb-背景部分； wf-前景部分
    float u0_temp, u1_temp, u0, u1; //平均值
    float delta_temp;   //存放临时方差
    double delta_max = 0.0; //初始化最大类间方差
    for (int i = 0; i < 256; i++)
    {
        wb = wf = u0_temp = u1_temp = u0 = u1 = delta_temp = 0;//初始化相关参数
        for (int j = 0; j < 256; j++)
        {
            //背景部分
            if (j <= i)
            {
                //当前i为分割阈值，第一类总的概率
                wb += nProDis[j];   //比重
                u0_temp += j*nProDis[j];
            }
            //前景部分
            else
            {
                //当前i为分割阈值，第一类总的概率
                wf += nProDis[i];   //比重
                u1_temp += j*nProDis[j];
            }
        }
        //------------分别计算各类的平均值------------
        u0 = u0_temp / wb;
        u1 = u1_temp / wf;
        //-----------计算最大类间方差------------
        delta_temp = (float)(wb*wf*pow((u0 - u1), 2));//形如pow(x,y);其作用是计算x的y次方。
                                                      //------------依次找到最大类间方差下的阈值------------
        if (delta_temp > delta_max)
        {
            delta_max = delta_temp;
            threshold = i;
        }
    }//计算结束
    return threshold;   //返回OTUS计算出的阈值
}

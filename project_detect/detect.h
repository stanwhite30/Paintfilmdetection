#ifndef DETECT_H
#define DETECT_H
#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
using namespace cv;

void change(Mat src,Mat dst,int g_nContrastValue, int g_nBrightValue);
float loss_count(Mat input,int thres);
void unevenLightCompensate(Mat &image, int blockSize);
Mat hough_detect(Mat input,int thres);

int OTSU(Mat grayImage);
#endif // DETECT_H

#pragma once
#include <iostream>
#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <math.h>



#define PI 3.14159265

struct ImOpj {
	int COMx;
	int COMy;
	int mu20;
	int mu02;
	int mu11;
	int mu00;
	int I1;
	double OritAngel;
};



using namespace std;
using namespace cv;
void helloWorld();
void Billedlaeser(string Imname);
int largest(int arr[], int n);
void CaldrawHistogram(Mat& I);
void COMImgCal(Mat& Img);
void DrawX(Mat img, int x, int y, int offset);
void MyLine(Mat img, Point start, Point end);
int getOneImage();

Mat& GrayScaleCal(Mat& Img);
Mat& BinaryMaker(Mat& I, int T);
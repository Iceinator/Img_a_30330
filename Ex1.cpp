#include "Ex1.h"
#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


void helloWorld() {
	cout << "hello world\n";


}

void Billedlaeser(string Imname) {
	
	cout << "Hej\n";
	string image_path = samples::findFile(Imname);
	Mat img = imread(image_path, IMREAD_COLOR);
	
	if (img.empty())
	{
		cout << "Could not read the image: " << image_path << endl;
		//return 1;
	}

	imshow("Display window", img);
	int k = waitKey(0); // Wait for a keystroke in the window
	if (k == 's')
	{
		imwrite("starry_night.png", img);

	}



}

int largest(int arr[], int n)
{
	int i;

	// Initialize maximum element
	int max = arr[0];

	// Traverse array elements
	// from second and compare
	// every element with current max
	for (i = 1; i < n; i++)
		if (arr[i] > max)
			max = arr[i];

	return max;
}

void CaldrawHistogram(Mat& I) {
	int histSize = 256;
	int hist_w = 512;
	int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	unsigned int value = 0; // index value for the histogram
	int histogram[256]; // histogram array - remember to set to zero 

	int width = I.size[1]; // INIT USING IMAGE VALUES
	int height = I.size[0]; // INIT USING IMAGE VALUES 
	int k = 256;
	for (int i1 = 0; i1 < 256; i1++) {
		histogram[i1] = 0; // reset histogram entry 
	}



	for (int i = 0; i < I.rows; i++) {
		for (int j = 211; j < I.cols; j++) {
			Vec3b intensity = I.at<Vec3b>(i, j);
			uchar blue = intensity.val[0];
			uchar green = intensity.val[1];
			uchar red = intensity.val[2];
			value = (0.3 * red) + (0.59 * green) + (0.11 * blue);
			if (value > 256) {
				value = 255;

			}


			histogram[value] += 1;
		}
	}

	Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

	int Max = largest(histogram, k);

	for (int i2 = 0; i2 < k; i2++) {
		histogram[i2] = hist_h * histogram[i2] / Max;

	}



	for (int i = 1; i < histSize; i++) {
		cv::line(
			histImage,
			cv::Point(bin_w * (i - 1), hist_h - histogram[i - 1]),
			cv::Point(bin_w * (i), hist_h - histogram[i - 1]),
			cv::Scalar(255, 0, 0), 2, 8, 0);

	}

	cv::namedWindow("calcHist Demo", cv::WINDOW_AUTOSIZE);
	cv::imshow("calcHist Demo", histImage);

}

Mat& GrayScaleCal(Mat& Img) {

	vector<cv::Mat> bgr_planes;
	split(Img, bgr_planes);

	Img = (0.3 * bgr_planes[0]) + (0.59 * bgr_planes[1]) + (0.11 * bgr_planes[2]);

	return Img;

}


ImOpj& COMImgCal(Mat& Img,ImOpj& COM) {
	int T = 50;
	int u = 0;
	int xpos = 0;
	int ypos = 0;
	int xposAv = 0;
	int yposAv = 0;

	BinaryMaker(Img, T);
	Mat_<Vec3b> _I = Img;
	for (int i = 0; i < Img.rows; ++i)
		for (int j = 0; j < Img.cols; ++j)
		{
			u = _I(i, j)[0];
			if (u > 0) {
				
				xpos += j;
				ypos += i;
			}
			

		}
	xposAv = xpos / Img.cols;
	yposAv = ypos / Img.rows;
	

	DrawX(Img, xposAv, yposAv, 10);

	COM.COMx = xposAv;
	COM.COMy = yposAv;

	return COM;

}

ImOpj& SecOrderMoCal(Mat& Img,ImOpj& Opj) {
	int u = 0;
	Opj.mu02 = 0;
	Opj.mu11 = 0;
	Opj.mu20 = 0;
	Opj.mu00 = 0;

	Mat_<Vec3b> _I = Img;
	for (int i = 0; i < Img.rows; ++i)
		for (int j = 0; j < Img.cols; ++j)
		{
			u = _I(i, j)[0];
			if (u > 0) {

				Opj.mu20 += (i - Opj.COMy) ^ 2;
				Opj.mu02 += (j - Opj.COMx) ^ 2;
				Opj.mu11 += (i - Opj.COMy) * (j - Opj.COMx);
				Opj.mu00++;

			}


		}


	return Opj;

}

ImOpj& InvaMoments(ImOpj& Opj) {
	int eta20 = 0;
	int eta02 = 0;

	eta20 = Opj.mu20 / (Opj.mu00 ^ (2));
	eta02 = Opj.mu02 / (Opj.mu00) ^ 2;

	Opj.I1 = eta20 + eta02;

	return Opj;
}


void OrientationCal(ImOpj& Opj) {
	
	
	
	Opj.OritAngel = atan(2*(double)Opj.mu11/ ((double)Opj.mu20- (double)Opj.mu02))/2;

	


}



void MyLine(Mat img, Point start, Point end)
{
	int thickness = 2;
	int lineType = LINE_8;
	line(img,
		start,
		end,
		Scalar(0, 0, 0),
		thickness,
		lineType);
}


void DrawX(Mat img,int x,int y, int offset) {

	Point xstart = Point(x - offset, y);
	Point xend = Point(x + offset, y);
	Point ystart = Point(x, y - offset);
	Point yend = Point(x, y + offset);
	MyLine(img, xstart, xend);
	MyLine(img, ystart, yend);

	cv::namedWindow("calcHist Demo", cv::WINDOW_AUTOSIZE);
	cv::imshow("Image with x", img);

}


Mat& BinaryMaker(Mat& I, int T) {
	int u = 0;




	// accept only char type matrices
	Mat_<Vec3b> _I = I;
	for (int i = 0; i < I.rows; ++i)
		for (int j = 0; j < I.cols; ++j)
		{
			u = (0.3 * _I(i, j)[0]) + (0.59 * _I(i, j)[1]) + (0.11 * _I(i, j)[2]);
			if (u > T) {
				_I(i, j)[0] = 255;
				_I(i, j)[1] = 255;
				_I(i, j)[2] = 255;
			}
			else {
				_I(i, j)[0] = 0;
				_I(i, j)[1] = 0;
				_I(i, j)[2] = 0;
			}



		}
	I = _I;
	return I;
}
/*
Mat& LPFilter(Mat& Img) {
	



}
*/
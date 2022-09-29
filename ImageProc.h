#pragma once

#include <string>
#include <stdio.h>
#include <iostream>
#include<opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

#define PI 3.14159265

using namespace cv;
using namespace std;

// Image Characteristics {Center of Mass, Moments, Moment Invariances, 
struct imCh_t {
	float CoM_x = 0;
	float CoM_y = 0;
	int mu_00 = 0; // Area
	float mu_11 = 0;
	float mu_20 = 0;
	float mu_02 = 0;
	float angle = 0;
	float mu_2 = 0; // Moment of Inertia
	float eta_02 = 0;
	float eta_20 = 0;
	float varphi_1 = 0;
};

int getMeAnImage(string imgName, Mat img, int x_loc, int y_loc, int y_size = 0, int x_size = 0);

void splitColor(string imgName, Mat img);

Mat RGBtoGrayscale(Mat img);

Mat thresholdFunc(Mat img, int thrVal, int channel);

class Histogram {
public:
	Mat calc_histogram(Mat scr) {
		Mat hist;
		hist = Mat::zeros(256, 1, CV_32F);
		scr.convertTo(scr, CV_32F);
		double value = 0;

		//cout << "\nRows: ";
		//cout << scr.rows;
		//cout << ", cols: ";
		//cout << scr.cols;
		//cout << "\n";

		for (int i = 0; i < scr.rows; i++) {
			for (int j = 0; j < scr.cols; j++) {
				value = scr.at<float>(i, j);
				hist.at<float>(value) = hist.at<float>(value) + 1;
			}
		}
		for (int k = 0; k < 256; k++) {
			//cout << "\nhist vals: ";
			//cout << (float)hist.at<int>(k);
		}
		return hist;
	}
	void plot_histogram(Mat histogram, string name, int thr) {
		Mat histogram_image(400, 512, CV_8UC3, Scalar(0, 0, 0));
		Mat normalized_histogram;
		normalize(histogram, normalized_histogram, 0, 400, NORM_MINMAX, -1, Mat());

		for (int i = 0; i < 256; i++) {
			rectangle(histogram_image, Point(2 * i, histogram_image.rows - normalized_histogram.at<float>(i)), Point(2 * (i + 1), histogram_image.rows), Scalar(255, 0, 0));
		}
		if (thr) {
			rectangle(histogram_image, Point(2 * thr, histogram_image.rows - normalized_histogram.at<float>(thr)), Point(2 * (thr)+1, histogram_image.rows), Scalar(0, 0, 255));
		}

		namedWindow(name, WINDOW_NORMAL);
		imshow(name, histogram_image);
	}
	uint32_t median(Mat histogram) {
		int i;
		int count = 0;
		for (int c = 0; c < 256; c++) count = count + histogram.at<int>(c);
		int n = count / 2; // 'hist' is the Hist struct at a given pixel location
		for (i = 0; i < 256 && ((n -= histogram.at<int>(i)) >= 0); ++i);
		for (int k = 0; k < 256; k++) {
			cout << "\nhist vals: ";
			cout << (float)histogram.at<int>(k);
		}
		return i;
	}
};

class ImageCharacteristics {
public:
	// Calculate the image characteristics
	void calculateCh(string imgName, Mat img, struct imCh_t* ic, int debug) {
		CenterOfMass(imgName, img, ic);
		reducedCentralMoment(imgName, img, ic);
		normCenMoments(ic);
		drawCh(imgName, img, ic);
		if (debug) {
			debugIC(img, ic);
		}
		//getMeAnImage(imgName, img, 0, 0, 0, 0);
	}
	// Needs binary image
	void CenterOfMass(string imgName, Mat img, struct imCh_t* ic) {
		float x_vals = 0;
		float y_vals = 0;
		ic->mu_00 = 0;
		Mat_<Vec3b> thr = img;
		for (int i = 0; i < thr.rows; i++) {
			for (int j = 0; j < thr.cols; j++) {
				if (thr(i, j)[0] == 0) {
					x_vals = x_vals + j;
					y_vals = y_vals + i;
					ic->mu_00++;
				}
			}
		}
		ic->CoM_x = x_vals / ic->mu_00;
		ic->CoM_y = y_vals / ic->mu_00;
	}


	void reducedCentralMoment(string imgName, Mat img, struct imCh_t* ic) {
		Mat_<Vec3b> thr = img;
		for (int i = 0; i < thr.rows; i++) {
			for (int j = 0; j < thr.cols; j++) {
				if (thr(i, j)[0] == 0) {
					ic->mu_11 = ic->mu_11 + (j - ic->CoM_x) * (i - ic->CoM_y);
					ic->mu_20 = ic->mu_20 + (j - ic->CoM_x) * (j - ic->CoM_x);
					ic->mu_02 = ic->mu_02 + (i - ic->CoM_y) * (i - ic->CoM_y);
				}
			}
		}
		ic->mu_11 = ic->mu_11 / ic->mu_00;
		ic->mu_20 = ic->mu_20 / ic->mu_00;
		ic->mu_02 = ic->mu_02 / ic->mu_00;

		ic->angle = atan2(2 * ic->mu_11, ic->mu_20 - ic->mu_02) * 180 / PI / 2;

		ic->mu_2 = ic->mu_20 + ic->mu_02;

	}

	void drawCh(string imgName, Mat img, struct imCh_t* ic) {

		float y_val = (img.cols / 4.0) * tan(ic->angle * PI / 180);
		Point A(ic->CoM_x - img.cols / 4, ic->CoM_y - y_val);
		Point B(ic->CoM_x + img.cols / 4, ic->CoM_y + y_val);

		line(img, A, B, Scalar(255, 255, 0), 2);
		drawMarker(img, Point(ic->CoM_x, ic->CoM_y), Scalar(0, 0, 255), MARKER_CROSS, 10, 1);
		getMeAnImage(imgName + " (altered)", img, 0, 0, 0, 0);
	}

	void resetImCh(struct imCh_t* ic) {
		ic->CoM_x = 0;
		ic->CoM_y = 0;
		ic->mu_00 = 0; // Area
		ic->mu_11 = 0;
		ic->mu_20 = 0;
		ic->mu_02 = 0;
		ic->angle = 0;
		ic->mu_2 = 0; // Moment of Inertia
	}

	void normCenMoments(struct imCh_t* ic) {
		ic->eta_20 = ic->mu_20 / (ic->mu_00 ^ (1 + 2 / 2));
		ic->eta_02 = ic->mu_02 / (ic->mu_00 ^ (1 + 2 / 2));
		ic->varphi_1 = ic->eta_20 + ic->eta_02;
	}

	void debugIC(Mat thr, struct imCh_t* ic) {

		// Print for debugging
		cout << "\nArea is: ";
		cout << ic->mu_00;
		cout << ", Center of Mass in X: ";
		cout << ic->CoM_x;
		cout << ", in Y : ";
		cout << ic->CoM_y;
		cout << "\n";

		cout << "The picture dimensions are X: ";
		cout << thr.rows;
		cout << ", by Y: ";
		cout << thr.cols,
		
		cout << "\n mu_20 is: ";
		cout << ic->mu_20 * ic->mu_00;
		cout << ", mu_02 is: ";
		cout << ic->mu_02 * ic->mu_00;
		cout << ", mu_11 is: ";
		cout << ic->mu_11 * ic->mu_00;

		cout << "\n Angle is: ";
		cout << ic->angle;
		cout << " deg";

		cout << "\n Moment of Inertia is: ";
		cout << ic->mu_2;

		cout << "\nVarphi 1: ";
		cout << ic->varphi_1;
		cout << "\n";
	}
};


class Filtering {
public:
	void filterPadding(Mat q, int xPad, int yPad) {
		// Padding of Mat q
		// Maybe use modulo %
	}

	Mat lowPass(Mat_<uchar> q, Mat_<uchar> p, int n, int m) {
		for (int i = 0; i < p.rows; i++) {
			for (int j = 0; j < p.cols; j++) {
				// Kernel
				uint32_t sum = 0;
				for (int k = -n; k < n + 1; k++) {
					for (int l = -m; l < m + 1; l++) {
						// Only apply pixel value, if the index is within bounds, 
						// i.e. bigger than index 0 and smaller than the width or length
						if (k + i > 0 && k + i < q.rows && l + j > 0 && l + j < q.cols) {
							sum += q(k + i, l + j);
						}
					}
				}
				float c_kl = 1 / ((2 * (float)n + 1) * (2 * (float)n + 1));
				p(i, j) = c_kl * sum;
			}
		}
		return p;
	}

	Mat highPass(Mat_<uchar> q, Mat_<uchar> p, int n, int m) {
		for (int i = 0; i < p.rows; i++) {
			for (int j = 0; j < p.cols; j++) {
				// Kernel
				uint32_t sum = 0;
				for (int k = -n; k < n + 1; k++) {
					for (int l = -m; l < m + 1; l++) {
						// Only apply pixel value, if the index is within bounds, 
						// i.e. bigger than index 0 and smaller than the width or length
						if (k + i > 0 && k + i < q.rows && l + j > 0 && l + j < q.cols) {
							sum += k * q(k + i, l + j) / 6 + l * q(k + i, l + j) / 6;
						}
					}
				}
				// Re-map values from [255, -255] to [127, -127] to [255, 0]
				sum = sum/2 + 127;
				p(i, j) = sum;
			}
		}
		return p;
	}

	Mat fractial(Mat_<uchar> q, float n, float m) {
		//Mat_<Vec3b> p = q;
		Mat_<uchar> p = q;
		float sum = 0;
		for (int i = 0; i < p.rows; i++) {
			for (int j = 0; j < p.cols; j++) {
				// Kernel
				sum = 0;
				for (int k = -n; k < n + 1; k++) {
					for (int l = -m; l < m + 1; l++) {
						if (k + i < 0 || k + i >= q.rows || l + j < 0 || l + j >= q.cols) {
							sum = sum - (255 / 9.0); // White
						}
						else if (k == 0 && l == 0) {
							sum = sum + (int)q.at<uchar>(k + i, l + j) * (8 / 9.0);
						}
						else {
							sum = sum - (int)q.at<uchar>(k + i, l + j) * (1 / 9.0);
						}
					}
				}
				p.at<uchar>(i, j) = sum;
			}
		}
		return p;
	}


	static constexpr const int LaplaceGaussianKernel[9][9] = 
	{{0, 0, 1, 2, 2, 2, 1, 0, 0},
	{0, 1, 5, 10, 12, 10, 5, 1, 0},
	{1, 5, 15, 19, 16, 19, 15, 5, 1},
	{2, 10, 19, -19, -64, -19, 19, 19, 2},
	{2, 12, 16, -64, -148, -64, 16, 12, 2},
	{2, 10, 19, -19, -64, -19, 19, 19, 2},
	{1, 5, 15, 19, 16, 19, 15, 5, 1},
	{0, 1, 5, 10, 12, 10, 5, 1, 0},
	{0, 0, 1, 2, 2, 2, 1, 0, 0}};

	Mat LaplaceGaussian(Mat_<uchar> q, Mat_<uchar> p, int n, int m) {
		int32_t max_val = 0;
		int32_t min_val = 0;
		for (int i = 0; i < p.rows; i++) {
			for (int j = 0; j < p.cols; j++) {
				// Kernel
				int32_t sum = 0;
				for (int k = -n; k < n + 1; k++) {
					for (int l = -m; l < m + 1; l++) {
						// Only apply pixel value, if the index is within bounds, 
						// i.e. bigger than index 0 and smaller than the width or length
						if (k + i > 0 && k + i < q.rows && l + j > 0 && l + j < q.cols) {
							sum += LaplaceGaussianKernel[k + n][l + m] * q(k + i, l + j)/81;
						}
					}
				}
				if (sum > max_val) max_val = sum;
				if (sum < min_val) min_val = sum;

				p(i, j) = sum/2 + 127;
				}
		}

		return p;
	}

	Mat stretchImg(Mat_<uchar> q, Mat_<uchar> p) {

	}

};


/*
CV_8U is unsigned 8bit/pixel - ie a pixel can have values 0-255, this is the normal range for most image and video formats.

CV_32F is float - the pixel can have any value between 0-1.0, this is useful for some sets of calculations on data - but it has to be converted into 8bits to save or display by multiplying each pixel by 255.

CV_32S is a signed 32bit integer value for each pixel - again useful of you are doing integer maths on the pixels, but again needs converting into 8bits to save or display. This is trickier since you need to decide how to convert the much larger range of possible values (+/- 2billion!) into 0-255

*/
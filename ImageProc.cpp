#include "ImageProc.h"

int getMeAnImage(string imgName, Mat img, int x_loc, int y_loc, int y_size, int x_size) {
	// UNCHANGED or -1,
	// IMREAD_GRAYSCALE or 0
	// IMREAD_COLOR or 1

	if (img.empty())
	{
		std::cout << "!!! Failed imread(): image not found" << std::endl;
		// don't let the execution continue, else imshow() will crash.
		return 0;
	}
	else {
		namedWindow(imgName, WINDOW_NORMAL);
		imshow(imgName, img);
		if (x_loc == 0 && y_loc == 0) {
		}
		else {
			moveWindow(imgName, x_loc, y_loc);
		}
		if (x_size == 0 && y_size == 0) {
			resizeWindow(imgName, img.size[0], img.size[1]);
		}
		else {
			resizeWindow(imgName, x_size, y_size);
		}
		return 1;
		// Wait for a keystroke. 
		//waitKey(0);
	}
}

void splitColor(string imgName, Mat img) {
	Mat different_Channels[3];
	split(img, different_Channels);	//splitting images into 3 different channels//  
	Mat b = different_Channels[0];		//loading blue channels//
	Mat g = different_Channels[1];		//loading green channels//
	Mat r = different_Channels[2];		//loading red channels//  


	namedWindow("Green Channel", WINDOW_NORMAL);
	moveWindow("Green Channel", 50, 300);
	resizeWindow("Green Channel", 250, 250);
	imshow("Green Channel", g);			//showing Green channel//

	namedWindow("Red Channel", WINDOW_NORMAL);
	moveWindow("Red Channel", 300, 50);
	resizeWindow("Red Channel", 250, 250);
	imshow("Red Channel", r);			//showing Red channel//

	namedWindow("Blue Channel", WINDOW_NORMAL);
	moveWindow("Blue Channel", 50, 50);
	resizeWindow("Blue Channel", 250, 250);
	imshow("Blue Channel", b);			//showing Blue channel//

	//imshow(imgName, img);	//showing actual image//
	//img_not = bitwise_not(img);
	//imshow(imgName, img_not);
	moveWindow(imgName, 300, 300);
	resizeWindow(imgName, 250, 250);
	
	// Wait for a keystroke. 
	waitKey(0);
}

Mat RGBtoGrayscale(Mat img) {
	
	Mat different_Channels[3];
	split(img, different_Channels);		//splitting images into 3 different channels//  
	Mat B = different_Channels[0];		//loading blue channels//
	Mat G = different_Channels[1];		//loading green channels//
	Mat R = different_Channels[2];		//loading red channels//  
	Mat grayscale = (0.3 * R) + (0.59 * G) + (0.11 * B);
	return grayscale;
}

//thrVal = 110
Mat thresholdFunc(Mat img, int thrVal, int channel) {
	// Channel
	// 0 - B
	// 1 - G
	// 2 - R
	// 3 - grayscale

	int output = 0;

	// accept only char type matrices
	Mat_<Vec3b> thr = img;
	for (int i = 0; i < img.rows; ++i) {
		for (int j = 0; j < img.cols; ++j)
		{
			switch (channel) {
			case 0:
				output = thr(i, j)[0];
				break;
			case 1:
				output = thr(i, j)[1];
				break;
			case 2:
				output = thr(i, j)[2];
				break;
			case 3:
				output = (0.3 * thr(i, j)[0]) + (0.59 * thr(i, j)[1]) + (0.11 * thr(i, j)[2]);
				break;
			default:
				break;
			}
			if (output > thrVal) {
				thr(i, j)[0] = 255;
				thr(i, j)[1] = 255;
				thr(i, j)[2] = 255;
			}
			else {
				thr(i, j)[0] = 0;
				thr(i, j)[1] = 0;
				thr(i, j)[2] = 0;
			}
			//cout << "\n";
			//cout << "Pixel value is: ";
			//uint32_t X = img.at<Vec3b>(i, j)[0];
			//cout << X;
			//cout << "\n";
		}
	}
	//img = thr;
	return thr;
}


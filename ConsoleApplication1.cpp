#include "ImageProc.h"

int main()
{	
	struct imCh_t imCh;

	// Read an image
	string path = "C:\\Users\\jonas\\Desktop\\30330 Image Analysis with Microcomputer Fall 22\\Images\\pen.pgm";
	string imgName = "Pen Image";
	string path_2 = "C:\\Users\\jonas\\Desktop\\30330 Image Analysis with Microcomputer Fall 22\\Images\\penrot.pgm";
	string imgName_2 = "Pen Rotated Image";
	//string path = "C:\\Users\\jonas\\Desktop\\30330 Image Analysis with Microcomputer Fall 22\\Images\\testImg_small.jpg";
	//path = "C:\\Users\\jonas\\Desktop\\30330 Image Analysis with Microcomputer Fall 22\\Images\\chessboard_big.png";
	//string imgName = "Test Image";
	//string path = "C:\\Users\\jonas\\Desktop\\30330 Image Analysis with Microcomputer Fall 22\\Images\\fish.jpg";
	//string imgName = "Fish Image";
	Mat img = imread(path, 1);
	Mat img_2 = imread(path_2, 1);

	int thrVal = 78;

	// ------------- Showcasing Image --------------- \\

	Mat grayscale = RGBtoGrayscale(img);
	getMeAnImage(imgName, grayscale, 0, 0, 0, 0);


	// Create an object, H1, of class Histogram
	Histogram H1;
	Mat hist = H1.calc_histogram(img);
	H1.plot_histogram(hist, "Histogram", thrVal);
	//H1.median(hist);

	waitKey();
	destroyAllWindows();


	// -------- Binary Images and processing ---------- \\

	ImageCharacteristics IC;
	Mat binaryImage = thresholdFunc(img.clone(), thrVal, 3);

	IC.calculateCh(imgName + "(Binary Image)", binaryImage, &imCh, 1);
	// Rotated image
	// IC.calculateCh(imgName_2 + "(Binary Image)", binaryImage, &imCh, 1);

	waitKey();
	destroyAllWindows();

	// ------------------ Filtering -------------------- \\
	
	Filtering FT;

	// ------------------ Low Pass

	Mat LP = grayscale.clone();
		LP = FT.lowPass(grayscale, LP, 1, 1);
	getMeAnImage(imgName + " (lowpass)", LP, 0, 0, 0, 0);

	Mat histLP = H1.calc_histogram(LP);
	H1.plot_histogram(histLP, "Histogram (lowpass)", 0);

	waitKey();
	destroyAllWindows();

	// ------------------ High Pass

	Mat HP = grayscale.clone();
		HP = FT.highPass(grayscale, HP, 1, 1);
	getMeAnImage(imgName + " (highpass)", HP, 0, 0, 0, 0);

	Mat histHP = H1.calc_histogram(HP);
	H1.plot_histogram(histHP, "Histogram (highpass)", 0);

	waitKey();
	destroyAllWindows();

	// -------------- Laplace-Gaussian 

	//Mat_<uchar> gs_min_LP = grayscale.clone - LP;
	//getMeAnImage(imgName + " (img - HP)", HP - grayscale, 0, 0, 0, 0);

	
	Mat LG = FT.LaplaceGaussian(grayscale, grayscale.clone(), 4, 4);
	getMeAnImage(imgName + " (Laplace-Gaussian)", LG, 0, 0, 0, 0);

	Mat histLG = H1.calc_histogram(LG);
	H1.plot_histogram(histLG, "Histogram (Laplace-Gaussian)", 0);
	

	waitKey();
	// Destroys all the windows created 
	destroyAllWindows();
}
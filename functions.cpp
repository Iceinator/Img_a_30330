#include "ex1.h"

int getOneImage() {
	string im_path = "C:\\Users\\silla\\Desktop\\";
	string im_names [5] = {"20220620_120336.jpg","20220620_120417.jpg","APOD.jpeg","Names.png","HPC1001Color.PNG"};
	for (int i = 0; i < 5; i++) {
		string image_path = im_path + im_names[i];
		Mat image = imread(image_path, 1);
		if (image.empty())
		{
			std::cout << "Could not read the image: " << image_path << std::endl;
			return 1;
		}
		imshow("Loaded image", image);
		char c = (char)waitKey(0);
		if (c == 10) continue; //Press continue loop
		else if (c == 27) break;
	}
	return 0;
}
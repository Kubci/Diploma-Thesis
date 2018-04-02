#include <iostream>
#include "opencv2/opencv.hpp"
#include "MaxTree.h"

int main(int, char)
{
	clock_t begin = std::clock();
	
	cv::Mat image = cv::imread("image.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	MaxTree tree(image);
	
	tree.reconstructImage();

	clock_t end = std::clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;


	/*
	VideoCapture cap(0); // open the default camera
	if (!cap.isOpened()) // check if we succeeded
		return -1;
	Mat edges;
	namedWindow("edges", 1);
	for (;;)
	{
		Mat frame;
		cap >> frame; // get a new frame from camera
		cvtColor(frame, edges, CV_BGR2GRAY);
		GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		Canny(edges, edges, 0, 30, 3);
		imshow("edges", edges);
		if (waitKey(30) >= 0)
			break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
	*/
	return 0;
}
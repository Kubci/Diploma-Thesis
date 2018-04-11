#include <iostream>
#include "opencv2/opencv.hpp"
#include "MaxTree.h"
#include "DataCarrier.h"
#include "RadixSort.h"

int main(int, char)
{
	clock_t begin = std::clock();
	
	cv::Mat image = cv::imread("qpi01.tif", CV_LOAD_IMAGE_ANYDEPTH);
	
	DataCarrier<int> dtc1(10);
	DataCarrier<int> dtc2(7);
	DataCarrier<int> dtc3(21);
	DataCarrier<int> dtc4(124);
	DataCarrier<int> dtc5(222);
	DataCarrier<int> dtc6(89);
	DataCarrier<int> dtc7(166);
	DataCarrier<int> dtc8(171);
	DataCarrier<int> dtc9(2);

	std::vector<DataCarrier<int>*> vec;
	vec.push_back(&dtc1);
	vec.push_back(&dtc2);
	vec.push_back(&dtc3);
	vec.push_back(&dtc4);
	vec.push_back(&dtc5);
	vec.push_back(&dtc6);
	vec.push_back(&dtc7);
	vec.push_back(&dtc8);
	vec.push_back(&dtc9);

	std::vector<DataCarrier<int>*> res(vec);

	int depth = 8;

	RadixSort<int>(vec, res, depth);

	MaxTree tree(image);
	
	tree.areaAttributeOpening(500);
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

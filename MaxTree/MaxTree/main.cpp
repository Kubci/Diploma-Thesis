#include <iostream>
#include "opencv2/opencv.hpp"
#include "MaxTreeNaive.h"
#include "MaxTreeBerger.h"
#include "RadixSort.h"
#include "GTParams.h"
#include "GTEvaluation.h"

int main(int, char)
{
	clock_t begin = std::clock();
	/*
	std::string path = "D:\\_SKOLA\\_Master\\_Diplomka\\Data\\vyhodnotenie GT\\";
	evalueteInFolder(path);
	*/

	cv::Mat image = cv::imread("D:\\Jakub Palenik\\Multimodal Segmentation\\Data\\vyhodnotenie GT\\QPI\\qpi11.tif", CV_LOAD_IMAGE_ANYDEPTH);
	cv::Mat gt = cv::imread("D:\\Jakub Palenik\\Multimodal Segmentation\\Data\\vyhodnotenie GT\\QPI\\GT\\qpi11_gt.tif", CV_LOAD_IMAGE_ANYDEPTH);
	convertTo8BitImage(gt);
	convertTo8BitImage(image);

	GTParams gtp(gt);
	MaxTreeBerger m_tree(image);
	m_tree.findBestJaccard(gtp);

	clock_t end = std::clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;

	return 0;
}

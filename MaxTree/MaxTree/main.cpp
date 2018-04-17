#include <iostream>
#include "opencv2/opencv.hpp"
#include "MaxTreeNaive.h"
#include "MaxTreeBerger.h"
#include "RadixSort.h"

int main(int, char)
{
	clock_t begin = std::clock();
	
	std::cout << sizeof(uint64) << std::endl;
	std::cout << sizeof(void*) << std::endl;
	cv::Mat image = cv::imread("image.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	MaxTreeBerger m_tree(image);
 	m_tree.areaOpening(500);
	m_tree.reconstruct();

	clock_t end = std::clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;

	return 0;
}

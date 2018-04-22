#include <iostream>
#include "opencv2/opencv.hpp"
#include "MaxTreeNaive.h"
#include "MaxTreeBerger.h"
#include "RadixSort.h"
#include "GTParams.h"

int main(int, char)
{
	std::string path_img = "D:\\_SKOLA\\_Master\\_Diplomka\\Data\\_vyhodnotit\\QPI\\";
	std::string img_name = "qpi11.tif";

	std::string path_gt = "D:\\_SKOLA\\_Master\\_Diplomka\\Data\\_vyhodnotit\\QPI\\GT\\";
	std::string img_gt = "qpi11_ohraniceni_maska.png";

	std::string out_path = "D:\\_SKOLA\\_Master\\_Diplomka\\Data\\tmp\\";

	clock_t begin = std::clock();
	
	cv::Mat gt = cv::imread(path_gt + img_gt, CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat image = cv::imread(path_img + img_name, CV_LOAD_IMAGE_ANYDEPTH);
	convertTo8BitImage(image);
	if (image.cols == 0 || image.rows == 0) return 0;

	GTParams gtp(gt);

	MaxTreeBerger m_tree(image);
	m_tree.compareToGT(gtp);
	m_tree.exportBestRois(gtp, out_path);

	clock_t end = std::clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;

	return 0;
}

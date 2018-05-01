#include <iostream>
#include "opencv2/opencv.hpp"
#include "MaxTreeNaive.h"
#include "MaxTreeBerger.h"
#include "RadixSort.h"
#include "ParamsGT.h"
#include "BatchEvaluator.h"

int main(int, char)
{
	clock_t begin = std::clock();
	
	std::string path = "D:\\_SKOLA\\_Master\\_Diplomka\\Data\\vyhodnotenie GT zle\\";
	evalueteInFolder(path);
	/*
	cv::Mat image = cv::imread(path + "QPI\\qpi11.tif", CV_LOAD_IMAGE_ANYDEPTH);
	cv::Mat gt = cv::imread(path + "QPI\\GT\\qpi11_gt.tif", CV_LOAD_IMAGE_ANYDEPTH);
	convertTo8BitImage(gt);
	convertTo8BitImage(image);

	ParamsGT gtp(gt);
	ParamsGT gtp2(gt);

	MaxTreeBerger m_tree(image);
	MaxTreeBerger m_tree2(image);
	cv::Mat global_jaccard = m_tree.findBestGlobalJaccard(gtp, path, std::string("qpi11") );
	m_tree2.findBestSingleJaccard(gtp, path, std::string("qpi11") );
	cv::Mat single_jaccard = m_tree2.exportBestRois(gtp, path, std::string("qpi11") );
	clock_t end = std::clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	*/
	return 0;
}

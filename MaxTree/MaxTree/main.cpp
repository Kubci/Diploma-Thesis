#include "MaxTreeBerger.h"
#include "BatchEvaluator.h"
#include "AutomaticSegmentation.h"

int main(int, char)
{
	clock_t begin = std::clock();
	
	//eval to gt
	//std::string path = "D:\\_SKOLA\\_Master\\_Diplomka\\Data\\vyhodnotenie GT\\";
	std::string path = "D:\\Jakub Palenik\\Multimodal Segmentation\\Data\\vyhodnotenie GT\\";
	//evalueteInFolder(path);
	
	
	
	cv::Mat image = cv::imread(path + "QPI\\qpi11.tif", CV_LOAD_IMAGE_ANYDEPTH);
	//cv::Mat gt = cv::imread(path + "QPI\\GT\\qpi11_gt.tif", CV_LOAD_IMAGE_ANYDEPTH);
	//convertTo8BitImage(gt);
	convertTo8BitImage(image);
	
	MaxTreeBerger m_tree(image);
	m_tree.areaOpening(50);

	AutomaticSegmentation sgm;
	sgm.segmentByEdges(m_tree);
	
	clock_t end = std::clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	
	return 0;
}

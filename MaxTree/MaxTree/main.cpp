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

	std::string path = "D:\\_SKOLA\\_Master\\_Diplomka\\Data\\vyhodnotenie GT\\";
	evalueteInFolder(path);

	clock_t end = std::clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;

	return 0;
}

#include "MaxTreeBerger.h"
#include "BatchEvaluator.h"
#include "AutomaticSegmentation.h"

int main(int, char)
{
	clock_t begin = std::clock();
	
	/*
	Expected name convenction and folder structure as is in attached dataset.
	To run this program, it is necessary to have OpenCV 3.4.1 installed, with its binaries on the PATH
	*/

	//std::string path = "insert path here of attached folder Data";
	std::string path = "D:\\_SKOLA\\_Master\\_Diplomka\\Data\\Data\\";
	evalueteInFolder(path);
	
	clock_t end = std::clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	std::cout << elapsed_secs << std::endl;
	
	return 0;
}

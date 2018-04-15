#pragma once
#include "opencv2/opencv.hpp"
#include "SetUF.h"
#include "PixelDataCarrier.h"
#include "RadixSort.h"

class MaxTreeBerger
{
public:

	cv::Mat image;
	cv::Mat reconstructed;
	SetUF<PixelDataCarrier>* root = nullptr;

	MaxTreeBerger(cv::Mat& image);
	void reconstruct();
	~MaxTreeBerger();

private:
	void reconstructRec(SetUF<PixelDataCarrier>* root);
	void retrievePixelsAsVector(std::vector<PixelDataCarrier*> & pixels);
	int index(cv::Point p);
	int index(int x, int y);
	void neighbours(cv::Point p, SetUF<PixelDataCarrier>** ef_mTree, std::vector< SetUF<PixelDataCarrier>* >& neighbours);

};


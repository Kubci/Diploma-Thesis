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
	SetUF<PixelDataCarrier>** tree = nullptr;

	MaxTreeBerger(cv::Mat& image);
	void reconstruct();
	~MaxTreeBerger();

private:
	SetUF<PixelDataCarrier>* dealocate;
	void reconstructRec(SetUF<PixelDataCarrier>* root);
	void retrievePixelsAsVector(std::vector<PixelDataCarrier*> &pixels);
	int index(cv::Point p) const;
	int index(int x, int y) const;
	void neighbours(cv::Point p, SetUF<PixelDataCarrier>** ef_mTree, SetUF<PixelDataCarrier>* (&neighbours)[9]) const;

};


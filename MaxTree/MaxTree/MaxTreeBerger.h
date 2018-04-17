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
	SetUF<PixelDataCarrier>** S = nullptr;

	MaxTreeBerger(cv::Mat& image);
	void reconstruct();
	void areaOpening(int area);
	~MaxTreeBerger();

private:
	SetUF<PixelDataCarrier>* dealocate;
	void computeArea();
	void canonicalize();
	void retrievePixelsAsVector(std::vector<PixelDataCarrier*> &pixels);
	int index(cv::Point p) const;
	int index(int x, int y) const;
	void neighbours(cv::Point p, SetUF<PixelDataCarrier>** ef_mTree, SetUF<PixelDataCarrier>* (&neighbours)[9]) const;

};


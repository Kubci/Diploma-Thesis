#pragma once
#include "opencv2/opencv.hpp"
#include "SetUF.h"
#include "PixelDataCarrier.h"
#include "RadixSort.h"

class MaxTreeBerger
{
public:

	cv::Mat image;
	SetUF<cv::Point>* root = nullptr;

	MaxTreeBerger(cv::Mat& image);
	~MaxTreeBerger();

private:
	void retrievePixelsAsVector(std::vector<PixelDataCarrier*> & pixels);
	int index(cv::Point p);
	int index(int x, int y);
	void neighbours(cv::Point p, SetUF<cv::Point>** parent, std::vector< SetUF<cv::Point>* >& neighbs);
};


#pragma once
#include "opencv2\opencv.hpp"
#include "Utils.h"
#include "SetUF.h"
#include <math.h>

class GTParams
{
public:
	cv::Mat labels;
	cv::Mat stats;
	cv::Mat centroids;
	cv::Mat best_cct;

	int max_area = -1;
	int min_area = INT_MAX;

	GTParams(cv::Mat gt);
	~GTParams();

	int getClosestLabel(cv::Point2f& cct_centroid);
	float computeJaccard(cv::Mat& roi, SetUFPix* cct, int label);
private:
	void getMinMaxArea();
};


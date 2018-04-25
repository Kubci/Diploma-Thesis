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
	int labelsCount;

	int max_area = -1;
	int min_area = INT_MAX;

	bool isCounterAllocated = false;
	int* labelCounter = nullptr;
	int* labelCounterDP = nullptr;

	GTParams(cv::Mat gt);
	~GTParams();

	int getClosestLabel(cv::Point2f& cct_centroid);
	float computeJaccard(cv::Mat& roi, SetUFPix* cct, int label);
	void initLabelCounter(int can_count);
	void addPixToLC(SetUF<PixelDataCarrier>* p);
	float computeJaccardLC(SetUF<PixelDataCarrier>* p) const;
	float computeJaccardLCDP(SetUF<PixelDataCarrier>* p) const;
	void pushJaccardToParentDP(SetUF<PixelDataCarrier>* p, bool flag);

private:
	void getMinMaxArea();
	int LCIdx(int can_idx) const;
};


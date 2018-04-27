#pragma once
#include "opencv2\opencv.hpp"
#include "Utils.h"
#include "SetUF.h"
#include <math.h>

class ParamsGT
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

	ParamsGT(cv::Mat gt);
	~ParamsGT();

	void initLabelCounter(int can_count);
	void freeLabelCounter();

	int getClosestLabel(cv::Point2f& cct_centroid);
	float computeJaccard(cv::Mat& roi, SetUFPix* cct, int label);
	void addPixToLC(SetUFPix* p);
	float computeJaccardLC(SetUFPix* p) const;
	bool intersestOneGTCompLC(SetUFPix* p, int* label) const;
	float computeJaccardLCDP(SetUFPix* p) const;
	void pushJaccardToParentDP(SetUFPix* p, bool flag);

private:
	void getMinMaxArea();
	int LCIdx(int can_idx) const;
};


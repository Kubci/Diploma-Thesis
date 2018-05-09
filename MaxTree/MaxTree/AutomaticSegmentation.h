#pragma once
#include "MaxTreeBerger.h"
#include "Utils.h"

class AutomaticSegmentation
{
public:

	float * gmSums;
	float * gmSumsDP;

	cv::Mat segmentByMaxEdges(MaxTreeBerger& mTree);
	cv::Mat segmentByEdges(MaxTreeBerger& mTree);

	AutomaticSegmentation();
	~AutomaticSegmentation();
};


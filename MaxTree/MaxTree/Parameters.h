#pragma once
#include "opencv2/core.hpp"

class Parameters 
{
public:
	int area = 1;

	int min_x = 1000000;
	int min_y = 1000000;
	int max_x = -1;
	int max_y = -1;

	void initBB(cv::Point point);
	void setBB(Parameters & p);
	

	Parameters();
	~Parameters();

};
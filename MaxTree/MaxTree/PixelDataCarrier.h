#pragma once
#include "opencv2\core.hpp"

class PixelDataCarrier
{
public:
	int			sort_value;
	cv::Point	data;

	PixelDataCarrier(int sort_value, cv::Point data);
	PixelDataCarrier(int sort_value);
	~PixelDataCarrier(){};
};



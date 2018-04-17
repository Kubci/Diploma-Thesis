#pragma once
#include "opencv2\core.hpp"

class PixelDataCarrier
{
public:
	int			value;
	int			rec_value;
	cv::Point	point;

	PixelDataCarrier(int value, cv::Point data);
	PixelDataCarrier(int value);
	PixelDataCarrier();
	~PixelDataCarrier(){};
};



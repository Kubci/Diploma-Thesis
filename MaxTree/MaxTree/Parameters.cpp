#include "Parameters.h"

void Parameters::initBB(cv::Point& point)
{
	min_x = point.x;
	max_x = point.x;
	max_y = point.y;
	min_y = point.y;
}

void Parameters::setBB(Parameters & p)
{
	if (p.min_x < min_x) min_x = p.min_x;
	if (p.min_y < min_y) min_y = p.min_y;
	if (p.max_x > max_x) max_x = p.max_x;
	if (p.max_y > max_y) max_y = p.max_y;
}


Parameters::Parameters()
{
}

Parameters::~Parameters()
{
}

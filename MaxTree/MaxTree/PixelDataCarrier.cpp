#include "PixelDataCarrier.h"


PixelDataCarrier::PixelDataCarrier(int sort_value, cv::Point data) : sort_value(sort_value), data(data)
{
}

PixelDataCarrier::PixelDataCarrier(int sort_value) : sort_value(sort_value)
{
}
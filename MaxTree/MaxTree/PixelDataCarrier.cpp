#include "PixelDataCarrier.h"


PixelDataCarrier::PixelDataCarrier(int value, cv::Point data) : value(value), point(data)
{
}

PixelDataCarrier::PixelDataCarrier(int value) : value(value)
{
}

PixelDataCarrier::PixelDataCarrier()
{
}

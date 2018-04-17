#include "PixelDataCarrier.h"


PixelDataCarrier::PixelDataCarrier(int value, cv::Point data) : value(value), point(data), rec_value(value)
{
}

PixelDataCarrier::PixelDataCarrier(int value) : value(value), rec_value(value)
{
}

PixelDataCarrier::PixelDataCarrier()
{
}

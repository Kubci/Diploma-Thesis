#include "Utils.h"

void AddComponentToImage(cv::Mat& reconstructed, cv::Mat& labelled_level, int label)
{
	for (int y = 0; y < labelled_level.rows; y++)
	{
		uchar* row_r = (uchar*)reconstructed.ptr(y);
		int* row_l = (int*)labelled_level.ptr(y);

		for (int x = 0; x < labelled_level.cols; x++)
		{
			if (row_l[x] == label)
			{
				row_r[x] += 1;
			}
		}
	}
}

void AddComponentsToImage(cv::Mat& reconstructed, cv::Mat& labelled_level, std::vector<bool>& labels)
{
	for (int y = 0; y < labelled_level.rows; y++)
	{
		uchar* row_r = (uchar*)reconstructed.ptr(y);
		int* row_l = (int*)labelled_level.ptr(y);

		for (int x = 0; x < labelled_level.cols; x++)
		{
			if (labels[row_l[x]])
			{
				row_r[x] += 1;
			}
		}
	}
}

void convertTo8BitImage(cv::Mat & image)
{
	double min, max;
	cv::minMaxLoc(image, &min, &max);

	image = 255 * ((image - min) / (max - min) );
	image.convertTo(image, CV_8UC1);
}

float distance(cv::Point2f & first, cv::Point2f & second)
{
	float x = second.x - first.x;
	float y = second.y - first.y;
	
	return std::abs(std::sqrt(x*x + y*y));
}

void centroid(cv::Mat& roi, SetUFPix* p, cv::Point2f& centroid)
{
	float c_x = 0;
	float c_y = 0;
	float count = 0;

	for (int y = 0; y < roi.rows; y++)
	{
		uchar* row = (uchar*)roi.ptr(y);
		for (int x = 0; x < roi.cols; x++)
		{
			if (row[x])
			{
				c_x += x;
				c_y += y;
				count++;
			}
		}
	}

	centroid.x = (c_x + count * p->params.min_x) / count;
	centroid.y = (c_y + count * p->params.min_y) / count;
}

void addRoiToImage(cv::Mat& image, cv::Mat& roi, SetUFPix * p)
{
	int w = p->params.max_x - p->params.min_x + 1;
	int h = p->params.max_y - p->params.min_y + 1;

	for (int y = 0; y < h; y++)
	{
		uchar* row_roi = (uchar*)roi.ptr(y);
		uchar* row_i = (uchar*)image.ptr(y + p->params.min_y);
		for (int x = 0; x < w; x++)
		{
			if (row_roi[x])
			{
				row_i[x + p->params.min_x]++;
			}
		}
	}
}


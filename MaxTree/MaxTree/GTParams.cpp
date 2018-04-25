#include "GTParams.h"

GTParams::GTParams(cv::Mat gt)
{
	labelsCount = cv::connectedComponentsWithStats(gt, labels, stats, centroids, 4);
	best_cct = cv::Mat(centroids.rows, centroids.cols, CV_32FC1, cv::Scalar(-1));
	getMinMaxArea();
}

GTParams::~GTParams()
{
	delete[] labelCounter;
	delete[] labelCounterDP;
}

int GTParams::getClosestLabel(cv::Point2f& cct_centroid)
{
	float min_dst = 9999999;
	int label = -1;
	for (int i = 1; i < stats.rows; i++)
	{
		double* row = (double*)centroids.ptr(i);
		float l_x = static_cast<float>(row[0]);
		float l_y = static_cast<float>(row[1]);

		float dst = distance(cct_centroid, cv::Point2f(l_x, l_y));
		if (dst < min_dst)
		{
			min_dst = dst;
			label = i;
		}

	}
	return label;
}

float GTParams::computeJaccard(cv::Mat& roi, SetUFPix * cct, int label)
{
	//computing the window
	int x0 = std::min(cct->params.min_x, stats.at<int>(cv::Point(0, label)));
	int y0 = std::min(cct->params.min_y, stats.at<int>(cv::Point(1, label)));
	int cct_width = cct->params.max_x - cct->params.min_x + 1;
	int cct_height = cct->params.max_y - cct->params.min_y + 1;
	int width = std::max(cct_width, stats.at<int>(cv::Point(2, label)));
	int heihgt = std::max(cct_height, stats.at<int>(cv::Point(3, label)));

	float _union = 0;
	float _intersection = 0;

	for (int y = y0; y < (y0 + heihgt); y++)
	{
		int* row_gt = (int*)labels.ptr(y);
		uchar* row_cct = nullptr;
		if (y >= cct->params.min_y && y <= cct->params.max_y)
		{
			row_cct = roi.ptr(y - cct->params.min_y);
		}

		for (int x = x0; x < (x0 + width); x++)
		{
			bool flag_gt = false;
			if (row_gt[x] == label)
			{
				flag_gt = true;
			}

			bool flag_cct = false;
			if (x >= cct->params.min_x && x <= cct->params.max_x && row_cct)
			{
				int x_off = x - cct->params.min_x;
				if (row_cct[x_off])
				{
					flag_cct = true;
				}
			}

			if (flag_gt && flag_cct)
			{
				_intersection++;
				_union++;
			}

			if (flag_gt ^ flag_cct)
			{
				_union++;
			}
		}
	}
	return _intersection / _union;
}

void GTParams::initLabelCounter(int can_count)
{
	isCounterAllocated = true;
	labelCounter = new int[can_count * labelsCount]{ 0 };
	labelCounterDP = new int[can_count * labelsCount]{ 0 };

	std::fill_n(labelCounter  , can_count * labelsCount, 0);
	std::fill_n(labelCounterDP, can_count * labelsCount, 0);
}

int GTParams::LCIdx(int can_idx) const
{
	return can_idx * labelsCount;
}

void GTParams::addPixToLC(SetUF<PixelDataCarrier>* p)
{
	int c_idx;
	if (p->isCanonical)
	{
		c_idx = p->canIndex;
	}
	else
	{
		c_idx = p->parent->canIndex;
	}
	c_idx = LCIdx(c_idx);
	int p_val = labels.at<int>(p->item.point);
	labelCounter[c_idx + p_val]++;

	if (p->isCanonical) {
		int idx2 = LCIdx(p->parent->canIndex);
		for (int i = 0; i < labelsCount; i++)
		{
			labelCounter[idx2 + i] += labelCounter[c_idx + i];
		}
	}
}

float GTParams::computeJaccardLC(SetUF<PixelDataCarrier>* p) const
{
	int c_idx = LCIdx(p->canIndex);

	int _union = 0;
	int _inter = 0;

	for (int i = 1; i < labelsCount; i++)
	{
		int val = labelCounter[c_idx + i];
		if (val)
		{
			_union += stats.at<int>(i, cv::CC_STAT_AREA);
			_inter += val;
		}

	}
	_union += _inter + labelCounter[c_idx];
	if (_union == 0) return 0;
	return static_cast<float>(_inter) / static_cast<float>(_union);
}

float GTParams::computeJaccardLCDP(SetUF<PixelDataCarrier>* p) const
{
	int c_idx = LCIdx(p->canIndex);

	int _union = 0;
	int _inter = 0;

	for (int i = 1; i < labelsCount; i++)
	{
		int val = labelCounterDP[c_idx + i];
		if (val)
		{
			_union += stats.at<int>(i, cv::CC_STAT_AREA);
			_inter += val;
		}

	}
	_union += _inter + labelCounterDP[c_idx];
	if (_union == 0) return 0;
	return static_cast<float>(_inter) / static_cast<float>(_union);
}

void GTParams::pushJaccardToParentDP(SetUF<PixelDataCarrier>* p, bool flag)
{
	int c_idx = LCIdx(p->canIndex);
	int idx2 = LCIdx(p->parent->canIndex);
	for (int i = 0; i < labelsCount; i++)
	{
		if (flag) {
			labelCounterDP[idx2 + i] += labelCounter[c_idx + i];
		}
		else
		{
			labelCounterDP[idx2 + i] += labelCounterDP[c_idx + i];
		}
	}
}

void GTParams::getMinMaxArea()
{
	for (int i = 1; i < stats.rows; i++)
	{
		max_area = std::max(max_area, stats.at<int>(cv::Point(4, i)));
		min_area = std::min(min_area, stats.at<int>(cv::Point(4, i)));
	}
}

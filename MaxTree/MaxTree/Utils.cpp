#include "Utils.h"

void AddComponentToImage(cv::Mat reconstructed, cv::Mat labelled_level, int label)
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

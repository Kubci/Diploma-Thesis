#include "AutomaticSegmentation.h"


cv::Mat AutomaticSegmentation::segmentByMaxEdges(MaxTreeBerger& m_tree)
{
	gmSums = new float[m_tree.can_count]{ 0 };
	gmSumsDP = new float[m_tree.can_count]{ 0 };

	cv::Mat dx, dy;
	Sobel(m_tree.image, dx, CV_32FC1, 1, 0);
	Sobel(m_tree.image, dy, CV_32FC1, 0, 1);

	cv::Mat gm;
	magnitude(dx, dy, gm);

	for (int i = (m_tree.pix_count - 1); i >= 1; i--)
	{
		SetUFPix* p = m_tree.S[i];
		SetUFPix* q = p->parent;
		if (!p->isCanonical)
		{
			gmSums[q->canIndex] += gm.at<float>(p->item.point);
		}
		else
		{
			gmSums[p->canIndex] += gm.at<float>(p->item.point);
			gmSums[q->canIndex] += gmSums[p->canIndex];

			float curr_E = gmSums[p->canIndex] / p->params.area;
			if (curr_E > gmSumsDP[p->canIndex])
			{
				gmSumsDP[q->canIndex] += curr_E;
				p->useThis = true;
			}
			else
			{
				gmSumsDP[q->canIndex] += gmSumsDP[p->canIndex];
			}
		}
	}

	cv::Mat roi;
	cv::Mat result(m_tree.image.size(), CV_8UC1, cv::Scalar(0));
	for (int i = 1; i < m_tree.pix_count; i++)
	{
		SetUF<PixelDataCarrier>* p = m_tree.S[i];
		if (p->isCanonical)
		{
			SetUF<PixelDataCarrier>* q = p->parent;
			if (q->useThis)
			{
				p->useThis = true;
			}
			else
			{
				if (p->useThis)
				{
					m_tree.extractRoiMask(roi, p);
					addRoiToImage(result, roi, p);
				}
			}
		}
	}

	delete[] gmSums;
	delete[] gmSumsDP;

	return result;
}

cv::Mat AutomaticSegmentation::segmentByEdges(MaxTreeBerger& m_tree)
{
	//gmSums = new float[m_tree.can_count]{ 0 };
	gmSumsDP = new float[m_tree.can_count]{ 0 };

	cv::Mat dx, dy;
	Sobel(m_tree.image, dx, CV_32FC1, 1, 0);
	Sobel(m_tree.image, dy, CV_32FC1, 0, 1);

	cv::Mat gm;
	magnitude(dx, dy, gm);

	cv::Mat roi;
	cv::Mat roi_m;
	cv::Mat tmp;
	cv::Mat elem = cv::getStructuringElement(CV_SHAPE_CROSS, cv::Size(5, 5));
	cv::Mat elem2 = cv::getStructuringElement(CV_SHAPE_CROSS, cv::Size(5, 5));
	for (int i = (m_tree.pix_count - 1); i >= 1; i--)
	{
		SetUFPix* p = m_tree.S[i];
		SetUFPix* q = p->parent;
		if (p->isCanonical)
		{
			//extract rois;
			m_tree.extractRoi(gm, roi, p);
			m_tree.extractRoiMask(roi_m, p);

			//get edge
			//fillHoles(roi_m);

			cv::erode(roi_m, tmp, elem, cv::Point(-1,-1), 1, cv::BORDER_CONSTANT, cv::Scalar(0));
			roi_m = roi_m - tmp;
			cv::dilate(roi_m, roi_m, elem);

			//extract edge values
			roi_m.convertTo(roi_m, CV_32FC1, 1.0 / 255.0);
			roi_m = roi.mul(roi_m);

			float val = cv::sum(roi_m)[0];
			if (val > gmSumsDP[p->canIndex])
			{
				gmSumsDP[q->canIndex] += val;
				p->useThis = true;
			}
			else
			{
				gmSumsDP[q->canIndex] += gmSumsDP[p->canIndex];
			}
		}
	}

	cv::Mat result(m_tree.image.size(), CV_8UC1, cv::Scalar(0));
	for (int i = 1; i < m_tree.pix_count; i++)
	{
		SetUF<PixelDataCarrier>* p = m_tree.S[i];
		if (p->isCanonical)
		{
			SetUF<PixelDataCarrier>* q = p->parent;
			if (q->useThis)
			{
				p->useThis = true;
			}
			else
			{
				if (p->useThis)
				{
					m_tree.extractRoiMask(roi_m, p);
					addRoiToImage(result, roi_m, p);
				}
			}
		}
	}

	delete[] gmSums;
	delete[] gmSums;

	return result;
}

AutomaticSegmentation::AutomaticSegmentation()
{
}


AutomaticSegmentation::~AutomaticSegmentation()
{
}

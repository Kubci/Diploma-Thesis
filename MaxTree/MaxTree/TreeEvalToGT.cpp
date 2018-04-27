#include "TreeEvalToGT.h"

void compareToGT(MaxTreeBerger& m_tree, ParamsGT& gt)
{
	cv::Mat roi;
	cv::Point2f cct_centroid;
	for (int i = 0; i < m_tree.pix_count; i++)
	{
		SetUF<PixelDataCarrier>* p = m_tree.S[i];
		if (p->isCanonical)
		{
			if (p->size < 0.8*gt.min_area || p->size > 1.2*gt.max_area) continue;

			m_tree.extractRoi(roi, p);
			centroid(roi, p, cct_centroid);

			int label = gt.getClosestLabel(cct_centroid);
			float jaccard = gt.computeJaccard(roi, p, label);
			float best_jacc = gt.best_cct.at<float>(label, 0);

			if (jaccard > best_jacc)
			{
				gt.best_cct.at<float>(label, 0) = jaccard;
				gt.best_cct.at<float>(label, 1) = static_cast<float>(i);
			}
		}
	}
}

cv::Mat findBestGlobalJaccard(MaxTreeBerger& m_tree, ParamsGT& gt, std::string& path, std::string& name)
{
	gt.initLabelCounter(m_tree.can_count);
	for (int i = m_tree.pix_count - 1; i >= 0; i--)
	{
		SetUF<PixelDataCarrier>* p = m_tree.S[i];
		gt.addPixToLC(p);
		if (p->isCanonical)
		{
			p->params.jaccard = gt.computeJaccardLC(p);
			p->params.jaccardDP = gt.computeJaccardLCDP(p);
			if (p->params.jaccard >= p->params.jaccardDP)
			{
				p->useThis = true;
				gt.pushJaccardToParentDP(p, true);
			}
			else
			{
				gt.pushJaccardToParentDP(p, false);
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
					if (p->params.jaccard > 0.01)
					{
						m_tree.extractRoi(roi, p);
						addRoiToImage(result, roi, p);
					}
				}
			}
		}
	}

	gt.freeLabelCounter();
	cv::imwrite(path + name + "_maxJaccard.png", result);
	return result;
}

void findBestSingleJaccard(MaxTreeBerger& m_tree, ParamsGT& gt, std::string& path, std::string& name)
{
	gt.initLabelCounter(m_tree.can_count);

	for (int i = m_tree.pix_count - 1; i >= 0; i--)
	{
		SetUF<PixelDataCarrier>* p = m_tree.S[i];
		gt.addPixToLC(p);
		if (p->isCanonical)
		{
			p->params.jaccard = gt.computeJaccardLC(p);
			int label = 0;
			if (gt.intersestOneGTCompLC(p, &label))
			{
				float jaccard = gt.best_cct.at<float>(label, 0);
				if (p->params.jaccard >= jaccard)
				{
					gt.best_cct.at<float>(label, 0) = p->params.jaccard;
					gt.best_cct.at<float>(label, 1) = i;
				}
			}
		}
	}
	gt.freeLabelCounter();
}

cv::Mat exportBestRois(MaxTreeBerger& m_tree, ParamsGT & gt, std::string& path, std::string& name)
{
	cv::Mat roi;
	cv::Mat overlay(m_tree.image.size(), CV_8UC1, cv::Scalar(0));
	for (int i = 1; i < gt.best_cct.rows; i++)
	{
		float index = gt.best_cct.at<float>(i, 1);
		if (index < 0) continue;

		SetUF<PixelDataCarrier>* p = m_tree.S[(int)index];
		m_tree.extractRoi(roi, p);

		addRoiToImage(overlay, roi, p);
	}
	cv::imwrite(path + name + "_labels.png", gt.labels);
	cv::imwrite(path + name + "_components.png", overlay);

	return overlay;
}
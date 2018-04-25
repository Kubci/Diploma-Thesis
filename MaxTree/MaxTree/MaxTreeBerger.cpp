#include "MaxTreeBerger.h"

MaxTreeBerger::MaxTreeBerger(cv::Mat & image) : image(image), reconstructed(image.clone())
{
	SetUF<PixelDataCarrier>* set_uf;
	SetUF<PixelDataCarrier>** S		= new SetUF<PixelDataCarrier>*[image.rows * image.cols]{ 0 }; // pointers to pixel sets in sorted order refers to S in paper
	SetUF<PixelDataCarrier>** repr	= new SetUF<PixelDataCarrier>*[image.rows * image.cols]{ 0 }; // pointer from root of component in zpar to root of the same component in tree hierarchy;
	SetUF<PixelDataCarrier>** zpar  = new SetUF<PixelDataCarrier>*[image.rows * image.cols]{ 0 }; // structure to track connected components effectively

	// toto sa da zoptimalizovat
	std::vector<PixelDataCarrier* > pixels;
	std::vector<PixelDataCarrier* > pixels_sorted(pixels);
	retrievePixelsAsVector(pixels);
	RadixSort(pixels, pixels_sorted, 8);

	// Optimalization, so 
	SetUF<PixelDataCarrier>* parent_sets = new SetUF<PixelDataCarrier>[image.rows * image.cols];
	SetUF<PixelDataCarrier>* zpar_sets	= new SetUF<PixelDataCarrier>[image.rows * image.cols];
	for (PixelDataCarrier* p : pixels) {
		int idx = index(p->point);
		parent_sets[idx].item = *p;
		parent_sets[idx].params.initBB(p->point);
		zpar_sets[idx].item = *p;
	}

	int pos = 0;
	for (PixelDataCarrier* p : pixels_sorted)
	{
		int idx = index(p->point);
		S[pos++] = &parent_sets[idx];
	}

	SetUF<PixelDataCarrier>* neighb[9];
	for (int i = (int) pixels_sorted.size()-1; i >= 0; i--) 
	{
		PixelDataCarrier* pdc = pixels_sorted[i];	
		int idx = index(pdc->point);
		
		SetUF<PixelDataCarrier>* s1 = &parent_sets[idx];
		SetUF<PixelDataCarrier>* s2 = &zpar_sets[idx];

		repr[idx] = s1;												
		zpar[idx] = s2;

		neighbours(pdc->point, zpar, neighb);	
		for (int j = 0; j < 8; j++)
		{
			SetUF<PixelDataCarrier>* n = neighb[j];
			if (n == 0) break;

			SetUF<PixelDataCarrier>* n_rep = set_uf->find(n);			// get representant of zpar component
			if (s2->parent == n_rep) continue;
			set_uf->unionInOrder(s1, repr[index(n_rep->item.point)]);	// hang component under s1, to create hierarchy
			SetUF<PixelDataCarrier>* c2 = repr[index(n_rep->item.point)];

			SetUF<PixelDataCarrier>* head = set_uf->unionByRank(s2, n_rep);	// union by rank in zpar
			repr[index(head->item.point)] = s1;							
		}
	}
	this->S = S;
	dealocate = parent_sets;
	canonicalize();

	//freeing buffers
	delete[] zpar;
	delete[] zpar_sets;
	for (auto a : pixels) { delete a; }
}

void MaxTreeBerger::canonicalize()
{
	int counter = 1;
	for (int i = 0; i < image.cols * image.rows; i++)
	{
		SetUF<PixelDataCarrier>* p = S[i];
		SetUF<PixelDataCarrier>* q = p->parent;
		if (q->item.value == q->parent->item.value)
		{
			p->parent = q->parent;
		}
		if (p->item.value != q->item.value) 
		{
			p->isCanonical = true;
			p->canIndex = counter;
			counter++;
		}
	}
	S[0]->isCanonical = true; //root is always cannonical
	S[0]->canIndex = 0;
	can_count = counter;

	canonicalLeafs();
}

void MaxTreeBerger::canonicalLeafs() const
{
	for (int i = image.cols * image.rows - 1; i > 0; i--)
	{
		SetUF<PixelDataCarrier>* p = S[i];
		SetUF<PixelDataCarrier>* q = p->parent;
		if (!p->isCanonical)
		{
			p->isCanLeaf = false;
		}
		else // if p is cannonical
		{
			// then q is not cannonical
			q->isCanLeaf = false;
		}
	}
}

void MaxTreeBerger::retrievePixelsAsVector(std::vector<PixelDataCarrier*> & pixels)
{
	for (int y = 0; y < image.rows; y++) {
		uchar* row = (uchar*)image.ptr(y);

		for (int x = 0; x < image.cols; x++)
		{
			pixels.push_back(new PixelDataCarrier(row[x], cv::Point(x, y)));
		}
	}
}

int MaxTreeBerger::index(cv::Point& p) const
{
	return index(p.x, p.y);
}

int MaxTreeBerger::index(int x, int y) const
{
	if (x < 0 || y < 0 || x >= image.cols || y >= image.rows) return -1;
	return x + y * image.cols;
}

// 0 1 2
// 7   3
// 6 5 4
void MaxTreeBerger::neighbours(cv::Point& p, SetUF<PixelDataCarrier>** ef_mTree, SetUF<PixelDataCarrier>* (&neighbours)[9]) const
{
	int i0 = index(p.x - 1, p.y - 1);
	int i1 = index(p.x    , p.y - 1);
	int i2 = index(p.x + 1, p.y - 1);
	int i3 = index(p.x + 1, p.y    );
	int i4 = index(p.x + 1, p.y + 1);
	int i5 = index(p.x    , p.y + 1);
	int i6 = index(p.x - 1, p.y + 1);
	int i7 = index(p.x - 1, p.y    );

	int idx[8] = { i0, i1, i2, i3, i4, i5, i6, i7 };
	
	int pos = 0;
	for (int i = 0; i < 8; i++)
	{
		if (idx[i] == -1) continue;
		SetUF<PixelDataCarrier>* n = ef_mTree[idx[i]];
		if (n == 0) continue;
		neighbours[pos++] = n;
	}
	neighbours[pos] = 0;
}

void MaxTreeBerger::reconstruct()
{
	reconstructed.setTo(cv::Scalar(0));
	if (!S[0]->isActive) return;

	for (int i = 0; i < image.cols * image.rows; i++)
	{
		SetUF<PixelDataCarrier>* q = S[i]->parent;
		SetUF<PixelDataCarrier>* p = S[i];

		if (!q->isActive) 
		{
			p->isActive = false;
		}
		if (!p->isActive) 
		{
			p->item.rec_value = q->item.rec_value;
		}
		reconstructed.at<uchar>(p->item.point) = p->item.rec_value;
	}
}

void MaxTreeBerger::computeArea()
{
	for (int i = image.cols * image.rows - 1; i > 0; i--)
	{
		SetUF<PixelDataCarrier>* q = S[i]->parent;
		q->params.area += S[i]->params.area;
	}
}

void MaxTreeBerger::computeBoundingBoxes()
{
	for (int i = image.cols * image.rows - 1; i >= 0; i--)
	{
		SetUF<PixelDataCarrier>* p = S[i];
		SetUF<PixelDataCarrier>* q = p->parent;
		q->params.setBB(p->params);
	}
}

void MaxTreeBerger::extractRoi(cv::Mat& roi, SetUFPix* p)
{
	int x, y, w, h;
	x = p->params.min_x;
	y = p->params.min_y;
	w = p->params.max_x - x + 1;
	h = p->params.max_y - y + 1;
	roi = image(cv::Rect(x, y, w, h)).clone();
	cv::threshold(roi, roi, p->item.value - 1, 1, CV_THRESH_BINARY);
	cv::Point CC_p(p->item.point.x - x, p->item.point.y - y);
	cv::floodFill(roi, CC_p, cv::Scalar(255), 0, cv::Scalar(0), cv::Scalar(0), 8);
	cv::threshold(roi, roi, 254, 255, CV_THRESH_BINARY);
}

void MaxTreeBerger::extractCanonicalLevels(std::string& path)
{
	computeArea();
	computeBoundingBoxes();

	cv::Mat roi;
	for (int i = 0; i < image.cols * image.rows; i++)
	{
		SetUF<PixelDataCarrier>* p = S[i];
		if (p->isCanonical) {
			extractRoi(roi, p);
			cv::imwrite(path + std::to_string(i) + ".png", roi);
		}
	}
}

void MaxTreeBerger::compareToGT(GTParams& gt)
{
	computeArea();
	computeBoundingBoxes();

	cv::Mat roi;
	cv::Point2f cct_centroid;
	for (int i = 0; i < image.cols * image.rows; i++)
	{
		SetUF<PixelDataCarrier>* p = S[i];
		if (p->isCanonical)
		{
			if (p->size < 0.8*gt.min_area || p->size > 1.2*gt.max_area) continue;

			extractRoi(roi, p);
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

void MaxTreeBerger::findBestJaccard(GTParams& gt)
{
	computeBoundingBoxes();
	gt.initLabelCounter(can_count);

	for (int i = image.cols * image.rows - 1; i >= 0; i--)
	{
		SetUF<PixelDataCarrier>* p = S[i];
		if(p->parent->item.point.x == 168 && p->parent->item.point.y == 22)
		{
			int brejkpoint = 0;
		}
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
	cv::Mat result(image.rows, image.cols, CV_8UC1, cv::Scalar(0));
	for (int i = 1; i < image.cols * image.rows; i++)
	{
		SetUF<PixelDataCarrier>* p = S[i];
		if (p->isCanonical)
		{
			SetUF<PixelDataCarrier>* q = p->parent;
			//already represented branch
			if(q->useThis)
			{
				p->useThis = true;
			}
			else
			{
				if (p->useThis)
				{
					if (p->params.jaccard > 0.01) {
						extractRoi(roi, p);
						addRoiToImage(result, roi, p);
						cv::imwrite(std::to_string(i) + "_" + std::to_string(p->params.jaccard) + ".png", roi);
					}
				}
			}
		}
	}
	cv::imwrite("result.png", result);
}

void MaxTreeBerger::exportBestRois(GTParams & gt, std::string& path, std::string& name)
{
	cv::Mat roi;
	cv::Mat overlay(image.rows, image.cols, CV_8UC1, cv::Scalar(0));
	for (int i = 1; i < gt.best_cct.rows; i++)
	{
		float index = gt.best_cct.at<float>(i, 1);
		if (index < 0) continue;

		SetUF<PixelDataCarrier>* p = S[(int)index];
		extractRoi(roi, p);

		addRoiToImage(overlay, roi, p);

		float jaccard = gt.best_cct.at<float>(i, 0);
		std::string out = path + name + "_" + std::to_string(jaccard) + "label_" + std::to_string(i) + ".png";
		cv::imwrite(out, roi);
	}
	cv::imwrite(path + name + "_labels.png", gt.labels);
	cv::imwrite(path + name + "_components.png", overlay);
}

void MaxTreeBerger::areaOpening(int area)
{
	computeArea();
	for (int i = 0; i < image.cols * image.rows; i++)
	{
		SetUF<PixelDataCarrier>* p = S[i];
		if (p->isCanonical)
		{
			if (p->params.area < area)
			{
				p->isActive = false;
			}
		}
	}
}

MaxTreeBerger::~MaxTreeBerger()
{
	delete[] S;
	delete[] dealocate;
}
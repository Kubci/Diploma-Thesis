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
		zpar_sets[idx].item = *p;
	}

	int pos = 0;
	for (PixelDataCarrier* p : pixels_sorted)
	{
		int idx = index(p->point);
		S[pos++] = &parent_sets[idx];
	}

	SetUF<PixelDataCarrier>* neighb[9];
	for (int i = pixels_sorted.size()-1; i >= 0; i--) 
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
		}
	}
	S[0]->isCanonical = true; //root is always cannonical
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

int MaxTreeBerger::index(cv::Point p) const
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
void MaxTreeBerger::neighbours(cv::Point p, SetUF<PixelDataCarrier>** ef_mTree, SetUF<PixelDataCarrier>* (&neighbours)[9]) const
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
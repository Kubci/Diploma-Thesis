#include "MaxTreeBerger.h"

MaxTreeBerger::MaxTreeBerger(cv::Mat & image) : image(image), reconstructed(image)
{
	SetUF<PixelDataCarrier>* set_uf;
	SetUF<PixelDataCarrier>** repr	= new SetUF<PixelDataCarrier>*[image.rows * image.cols]{ 0 }; // pointer from root of component in zpar to root of the same component in tree hierarchy;
	SetUF<PixelDataCarrier>** zpar  = new SetUF<PixelDataCarrier>*[image.rows * image.cols]{ 0 }; // structure to track connected components effectively

	std::fill_n(repr, image.rows * image.cols, (SetUF<PixelDataCarrier>*) 0);
	std::fill_n(zpar, image.rows * image.cols, (SetUF<PixelDataCarrier>*) 0);

	std::vector<PixelDataCarrier* > pixels;
	std::vector<PixelDataCarrier* > pixels_sorted(pixels);
	retrievePixelsAsVector(pixels);
	RadixSort(pixels, pixels_sorted, 8);

	std::vector<SetUF<PixelDataCarrier>* > neigh;
	neigh.reserve(8);

	for (int i = pixels_sorted.size()-1; i >= 0; i--) 
	{
		PixelDataCarrier* pdc = pixels_sorted[i];						// get processed pixel
		SetUF<PixelDataCarrier>* s1 = set_uf->makeSet(*pdc);			// make-set for tree hierarchy
		SetUF<PixelDataCarrier>* s2 = set_uf->makeSet(*pdc);			// make-set for zpar

		int idx = index(pdc->data);
		repr[idx] = s1;												
		zpar[idx] = s2;

		neigh.clear();
		neighbours(pdc->data, zpar, neigh);								// find neighbours
		for (SetUF<PixelDataCarrier>* n : neigh) {
			SetUF<PixelDataCarrier>* n_rep = set_uf->find(n);			// get representant of zpar component
			if (s2->parent == n_rep) continue;
			set_uf->unionInOrder(s1, repr[index(n_rep->item.data)]);	// hang component under s1, to create hierarchy
			
			SetUF<PixelDataCarrier>* head = set_uf->unionByRank(s2, n_rep);	// union by rank in zpar
			repr[index(head->item.data)] = s1;							
		}
	}
	root = repr[index(pixels_sorted[0]->data)];

	//free used arrays
	delete[] repr;
	delete[] zpar;

	for (auto a : pixels) 
	{
		delete a;
	}
}

void MaxTreeBerger::reconstruct()
{
	reconstructed = 0;
	reconstructRec(root);
}

void MaxTreeBerger::reconstructRec(SetUF<PixelDataCarrier>* root)
{
	reconstructed.at<uchar>(root->item.data) = root->item.sort_value;
	for (SetUF<PixelDataCarrier>* ch : root->successors) 
	{
		reconstructRec(ch);
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

int MaxTreeBerger::index(cv::Point p)
{
	return index(p.x, p.y);
}

int MaxTreeBerger::index(int x, int y)
{
	if (x < 0 || y < 0 || x >= image.cols || y >= image.rows) return -1;
	return x + y * image.cols;
}

// 0 1 2
// 7   3
// 6 5 4
void MaxTreeBerger::neighbours(cv::Point p, SetUF<PixelDataCarrier>** ef_mTree, std::vector< SetUF<PixelDataCarrier>* >& neighbours)
{
	int i0 = index(p.x - 1, p.y - 1);
	int i1 = index(p.x    , p.y - 1);
	int i2 = index(p.x + 1, p.y - 1);
	int i3 = index(p.x + 1, p.y    );
	int i4 = index(p.x + 1, p.y + 1);
	int i5 = index(p.x    , p.y + 1);
	int i6 = index(p.x - 1, p.y + 1);
	int i7 = index(p.x - 1, p.y    );

	std::vector<int> indcs; 
	indcs.push_back(i0);
	indcs.push_back(i1);
	indcs.push_back(i2);
	indcs.push_back(i3);
	indcs.push_back(i4);
	indcs.push_back(i5);
	indcs.push_back(i6);
	indcs.push_back(i7);

	for (int i : indcs) {
		if (i == -1) continue;
		SetUF<PixelDataCarrier>* n = ef_mTree[i];
		if (n == 0) continue;
		neighbours.push_back(n);
	}
}

MaxTreeBerger::~MaxTreeBerger()
{
}
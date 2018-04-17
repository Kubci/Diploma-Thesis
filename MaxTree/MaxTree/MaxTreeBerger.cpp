#include "MaxTreeBerger.h"

//problem pretoze nemam z rootu smerom dole ukazovatele
MaxTreeBerger::MaxTreeBerger(cv::Mat & image) : image(image), reconstructed(image)
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
		int idx = index(p->data);
		parent_sets[idx].item = *p;
		zpar_sets[idx].item = *p;
	}

	SetUF<PixelDataCarrier>* neighb[9];
	int position = 0;
	for (int i = pixels_sorted.size()-1; i >= 0; i--) 
	{
		PixelDataCarrier* pdc = pixels_sorted[i];	
		int idx = index(pdc->data);
		
		SetUF<PixelDataCarrier>* s1 = &parent_sets[idx];
		SetUF<PixelDataCarrier>* s2 = &zpar_sets[idx];

		S[position++] = s1;
		repr[idx] = s1;												
		zpar[idx] = s2;

		neighbours(pdc->data, zpar, neighb);	
		for (int j = 0; j < 8; j++)
		{
			SetUF<PixelDataCarrier>* n = neighb[j];
			if (n == 0) break;

			SetUF<PixelDataCarrier>* n_rep = set_uf->find(n);			// get representant of zpar component
			if (s2->parent == n_rep) continue;
			set_uf->unionInOrder(s1, repr[index(n_rep->item.data)]);	// hang component under s1, to create hierarchy
			
			SetUF<PixelDataCarrier>* head = set_uf->unionByRank(s2, n_rep);	// union by rank in zpar
			repr[index(head->item.data)] = s1;							
		}
	}

	tree = S;
	dealocate = parent_sets;
	delete[] repr;
	delete[] zpar;
	delete[] zpar_sets;

	for (auto a : pixels) 
	{
		delete a;
	}
}

void MaxTreeBerger::reconstruct()
{

}

void MaxTreeBerger::reconstructRec(SetUF<PixelDataCarrier>* root)
{
	/*
	reconstructed.at<uchar>(root->item.data) = root->item.sort_value;
	for (SetUF<PixelDataCarrier>* ch : root->successors) 
	{
		reconstructRec(ch);
	}
	*/
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
	
	int end_counter = 0;
	for (int i = 0; i < 8; i++)
	{
		if (i == -1) continue;
		SetUF<PixelDataCarrier>* n = ef_mTree[i];
		if (n == 0) continue;
		neighbours[i] = n;
		end_counter++;
	}
	neighbours[end_counter] = 0;
}

MaxTreeBerger::~MaxTreeBerger()
{
	delete[] dealocate;
	delete[] tree;
}
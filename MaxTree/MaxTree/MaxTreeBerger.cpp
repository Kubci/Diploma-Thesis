#include "MaxTreeBerger.h"

MaxTreeBerger::MaxTreeBerger(cv::Mat & image) : image(image)
{
	SetUF<cv::Point>* set_uf;
	SetUF<cv::Point>** m_tree	= new SetUF<cv::Point>*[image.rows * image.cols]{ 0 }; // array of pointers to set containing cv::Point
	SetUF<cv::Point>** ef_mTree = new SetUF<cv::Point>*[image.rows * image.cols]{ 0 }; // mtree structure with compression for effectivity
	cv::Point* connect			= new cv::Point[image.rows * image.cols];

	std::vector<PixelDataCarrier* > pixels;
	std::vector<PixelDataCarrier* > pixels_sorted(pixels);
	retrievePixelsAsVector(pixels);
	RadixSort(pixels, pixels_sorted, 8);

	std::vector<SetUF<cv::Point>* > neigh;

	for (int i = pixels_sorted.size()-1; i >= 0; i--) {
		PixelDataCarrier* pdc = pixels_sorted[i];					// gets processed pixel
		
		SetUF<cv::Point>* s1 = set_uf->makeSet(pdc->data);			// make-set for m_tree
		SetUF<cv::Point>* s2 = set_uf->makeSet(pdc->data);			// make-set for ef_mTree

		int idx = index(pdc->data);
		m_tree[idx] = s1;											// storing 
		ef_mTree[idx] = s2;
		connect[idx] = pdc->data;

		neighbours(pdc->data, ef_mTree, neigh);						// find neighbours
		for (SetUF<cv::Point>* n : neigh) {
			SetUF<cv::Point>* n_rep = set_uf->find(n);
			if (s2->parent == n_rep) continue;
			set_uf->unionInOrder(s1, m_tree[index(connect[index(n_rep->item)])]);
			
			SetUF<cv::Point>* head = set_uf->unionByRank(s2, n_rep);	// union by rank
			connect[index(head->item)] = pdc->data;						// pointer from root of ufSet to root of the same component in m_tree;
		}
		neigh.clear();
	}
	root = m_tree[index(pixels_sorted[0]->data)];


}

MaxTreeBerger::~MaxTreeBerger()
{
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
	return x + y * image.rows;
}


// 0 1 2
// 7   3
// 6 5 4
void MaxTreeBerger::neighbours(cv::Point p, SetUF<cv::Point>** parent, std::vector< SetUF<cv::Point>* >& neighbs)
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
		SetUF<cv::Point>* n = parent[i];
		if (n == 0) continue;
		neighbs.push_back(n);
	}
}

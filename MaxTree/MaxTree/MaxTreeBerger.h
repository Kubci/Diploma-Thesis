#pragma once
#include "opencv2/opencv.hpp"
#include "SetUF.h"
#include "PixelDataCarrier.h"
#include "RadixSort.h"
#include "ParamsGT.h"
#include "Utils.h"

class MaxTreeBerger
{
public:
	cv::Mat image;
	cv::Mat reconstructed;
	SetUF<PixelDataCarrier>** S = nullptr;
	int pix_count = 0;
	int can_count = 0;
	
	MaxTreeBerger(cv::Mat& image);
	void reconstruct();
	void areaOpening(int area);
	void extractCanonicalLevels(std::string& path);
	void extractRoi(cv::Mat& roi, SetUFPix* c_node);
	~MaxTreeBerger();

private:
	SetUF<PixelDataCarrier>* dealocate;
	void canonicalize();
	void computeArea();
	void computeBoundingBoxes();
	void canonicalLeafs() const;
	void retrievePixelsAsVector(std::vector<PixelDataCarrier*> &pixels);
	int index(cv::Point& p) const;
	int index(int x, int y) const;
	void neighbours(cv::Point& p, SetUF<PixelDataCarrier>** ef_mTree, SetUF<PixelDataCarrier>* (&neighbours)[9]) const;

};


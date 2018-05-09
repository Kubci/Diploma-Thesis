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
	SetUF<PixelDataCarrier>** S = nullptr;
	int pix_count = 0;
	int can_count = 0;
	
	MaxTreeBerger(cv::Mat& image);
	void reconstruct();
	void areaOpening(int area);
	void extractCanonicalLevels(std::string& path) const;
	void extractRoiMask(cv::Mat& roi, SetUFPix* c_node) const;
	void extractRoi(cv::Mat& roi, SetUFPix* c_node) const;
	static void extractRoi(cv::Mat& image, cv::Mat& roi, SetUFPix* c_node);
	~MaxTreeBerger();

private:
	SetUF<PixelDataCarrier>* dealocate;
	// called in constructor
	void canonicalize();
	void computeArea() const;
	void computeBoundingBoxes() const;
	// else
	void canonicalLeafs() const;
	void retrievePixelsAsVector(std::vector<PixelDataCarrier*> &pixels);
	int index(cv::Point& p) const;
	int index(int x, int y) const;
	void neighbours(cv::Point& p, SetUF<PixelDataCarrier>** ef_mTree, SetUF<PixelDataCarrier>* (&neighbours)[9]) const;

};


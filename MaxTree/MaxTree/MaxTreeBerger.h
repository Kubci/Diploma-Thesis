#pragma once
#include "opencv2/opencv.hpp"
#include "SetUF.h"
#include "PixelDataCarrier.h"
#include "RadixSort.h"
#include "GTParams.h"
#include "Utils.h"

class MaxTreeBerger
{
public:

	cv::Mat image;
	cv::Mat reconstructed;
	SetUF<PixelDataCarrier>** S = nullptr;
	int can_count = 0;

	MaxTreeBerger(cv::Mat& image);
	void reconstruct();
	void computeArea();
	void areaOpening(int area);
	void computeBoundingBoxes();
	void extractCanonicalLevels(std::string& path);
	void compareToGT(GTParams& gt);
	cv::Mat findBestJaccard(GTParams& gt, std::string& path, std::string& name);
	void findBestSingleJaccard(GTParams& gt, std::string& path, std::string& name);
	cv::Mat exportBestRois(GTParams& gt, std::string& path, std::string& name);
	~MaxTreeBerger();

private:
	SetUF<PixelDataCarrier>* dealocate;
	void extractRoi(cv::Mat& roi, SetUFPix* c_node);
	void canonicalize();
	void canonicalLeafs() const;
	void retrievePixelsAsVector(std::vector<PixelDataCarrier*> &pixels);
	int index(cv::Point& p) const;
	int index(int x, int y) const;
	void neighbours(cv::Point& p, SetUF<PixelDataCarrier>** ef_mTree, SetUF<PixelDataCarrier>* (&neighbours)[9]) const;

};


#pragma once
//STD
#include <vector>
#include <map>
//THIS
#include "Node.h"
#include "Utils.h"
//CV
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

//filtering constants
#define FILTER_AREA = 0

class MaxTreeNaive
{
private:
	std::vector<std::vector<Node*>>	tree;
	Node *							root = nullptr;
	std::vector<cv::Mat>			levels;
	cv::Mat							image;
	cv::Mat							reconstructed;

public:
	MaxTreeNaive(cv::Mat image);
	void reconstructImage();
	void areaAttributeOpening(int size);
	//void filter(); // should be a genarall function wit param variability
	~MaxTreeNaive();

private:
	void getLabelRepresentants	(int labels, cv::Mat& labelled_image, std::map<int, cv::Point> & representants);
	void retrieveLabelConjunction(int curr_lvl, std::map<int, cv::Point> & representants, std::map<int, std::vector<int>> & labels_conjuction);
	void pruneAbove(Node* node);
	void reconstructImageRec(Node* node);
	void areaAttributeOpeningRec(int size, Node* node);
	void deactivateRec(Node * node);

};


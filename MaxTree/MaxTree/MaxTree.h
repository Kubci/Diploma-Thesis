#pragma once
#include <vector>
#include <map>
#include "Node.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class MaxTree
{
private:
	std::vector<std::vector<Node*>>	tree;
	Node *							root = nullptr;
	std::vector<cv::Mat>			labels;
	std::vector<Node*>				leaves;
	cv::Mat							image;

public:
	MaxTree(cv::Mat image);
	void pruneAbove(Node* node);
	~MaxTree();

private:
	void getLabelRepresentants	(int labels, cv::Mat labelled_image, std::map<int, cv::Point> & representants);
	void retrieveLabelConjunction(int curr_lvl, std::map<int, cv::Point> & representants, std::map<int, std::vector<int>> & labels_conjuction);
};


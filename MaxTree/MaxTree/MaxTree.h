#pragma once
#include <vector>
#include "Node.h"
#include <opencv2/core/mat.hpp>

class MaxTree
{
public:
	std::vector<std::vector<Node*>>	tree;
	Node *							root = nullptr;
	std::vector<cv::Mat>			levels;
	std::vector<Node*>				leaves;
	cv::Mat							image;

	MaxTree(cv::Mat image);
	MaxTree();
	~MaxTree();
};


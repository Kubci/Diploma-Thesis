#pragma once
#include <vector>
#include "opencv2\core.hpp"

class Node
{
private:
	int					level;
	int					label;
	cv::Point			representant;
	Node *				predecessor = nullptr;
	std::vector<Node*>	successor;
	int					bb_width;
	int					bb_height;
	int					area;
	double				huMoments[7] = {0};

public:
	Node(int level, int label);
	~Node();
	void setRepresentant(cv::Point representant);
	void addSuccessor(Node* successor);
	void setPredecessor(Node* predecessor);
	void setBB_width(int width);
	void setBB_height(int height);
	void setArea(int area);
	void setHuMoments(double hu[7]);
	Node* getPredecessor();
};


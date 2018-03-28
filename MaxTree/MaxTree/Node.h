#pragma once
#include <vector>
#include <set>
#include "opencv2\core.hpp"

class Node
{
private:
	int					level;
	int					label;
	cv::Point			representant;
	Node *				predecessor = nullptr;
	std::set<Node*>		successor;
	int					bb_width;
	int					bb_height;
	int					area;
	double				huMoments[7] = {0};
	bool				isActive = true;

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
	void removeSuccessor(Node* node);
};


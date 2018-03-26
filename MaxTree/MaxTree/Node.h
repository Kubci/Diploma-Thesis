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

public:
	Node(int level, int label);
	~Node();
	void setRepresentant(cv::Point representant);
	void addSuccessor(Node* successor);
	void setPredecessor(Node* predecessor);
};


#include "Node.h"

Node::Node(int level, int label)
{
	successor = std::vector<Node*>();

	this->level = level;
	this->label = label;
}

Node::~Node(){}

void Node::setRepresentant(cv::Point representant)
{
	this->representant = representant;
}

void Node::addSuccessor(Node * successor)
{
	this->successor.push_back(successor);
}

void Node::setPredecessor(Node * predecessor)
{
	this->predecessor = predecessor;
}


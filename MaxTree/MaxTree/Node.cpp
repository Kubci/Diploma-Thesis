#include "Node.h"

Node::Node(int level, int label)
{
	successor = std::set<Node*>();

	this->level = level;
	this->label = label;
}

Node::~Node(){}

int Node::getLevel()
{
	return level;
}

int Node::getLabel()
{
	return label;
}

std::set<Node*>& Node::getSuccessor()
{
	return successor;
}

void Node::setRepresentant(cv::Point representant)
{
	this->representant = representant;
}

void Node::addSuccessor(Node * successor)
{
	this->successor.insert(successor);
}

void Node::setPredecessor(Node * predecessor)
{
	this->predecessor = predecessor;
}

void Node::setBB_width(int width)
{
	this->bb_width = width;
}

void Node::setBB_height(int height)
{
	this->bb_height = height;
}

void Node::setArea(int area)
{
	this->area = area;
}

void Node::setHuMoments(double hu[7])
{
	for (int i = 0; i < 7; i++) {
		huMoments[i] = hu[i];
	}
}

Node * Node::getPredecessor()
{
	return predecessor;
}

void Node::removeSuccessor(Node * node)
{
	successor.erase(node);
}


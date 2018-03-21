#include "MaxTree.h"


MaxTree::MaxTree(cv::Mat image)
{
	this->image = image;

	for (int i = 254; i < 0; i--)
	{
		
	}
}

MaxTree::~MaxTree()
{
	for (std::vector<Node*> v : tree)
	{
		for (Node* n : v)
		{
			delete n;
		}
	}
}

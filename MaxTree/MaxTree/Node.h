#pragma once
#include <vector>

class Node
{
public:
	int					level;
	int					label;
	Node *				predecessor;
	std::vector<Node*>	successor;
	//PARAMS?
	bool				is_Leaf = false;


	Node(int level, int label, Node* predecessor);
	~Node();
};


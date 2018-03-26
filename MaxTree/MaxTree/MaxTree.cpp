#include "MaxTree.h"


MaxTree::MaxTree(cv::Mat image)
{
	tree.push_back(std::vector<Node*>());

	this->image = image;

	//case for zero-th layer
	Node* zeroth_background = new Node(0, 0);
	Node* zeroth = new Node(0, 1);
	zeroth->setRepresentant(cv::Point(0, 0));
	tree[0].push_back(zeroth_background);
	tree[0].push_back(zeroth);
	root = zeroth;
	labels.push_back(cv::Mat(image.cols, image.rows, CV_32SC1, CvScalar(1)));

	cv::Mat curr_lvl;
	cv::Mat curr_labels;
	std::map<int, cv::Point> label_representants;
	std::map<int, std::vector<int>> labels_conjunction;

	for (int i = 1; i < 256; i++)
	{

		curr_lvl = image >= i;
		if (cv::countNonZero(curr_lvl) == 0){
			return;
		}

		int labels_count = cv::connectedComponents(curr_lvl, curr_labels, 4);
		labels.push_back(curr_labels.clone());
		getLabelRepresentants(labels_count, curr_labels, label_representants);
		
		std::vector<Node*> level_nodes;
		level_nodes.push_back(new Node(i, 0));
		for (int k = 1; k < labels_count; k++) {
			Node* n = new Node(i, k);
			n->setRepresentant(label_representants[k]);
			level_nodes.push_back(n);
		}
		tree.push_back(level_nodes);

		retrieveLabelConjunction(i, label_representants, labels_conjunction);
		for (auto prev_label : labels_conjunction) 
		{
			std::vector<int>& joints = prev_label.second;
			for (int k : joints) {
				level_nodes[k]->setPredecessor(tree[i - 1][prev_label.first]);
				tree[i - 1][prev_label.first]->addSuccessor(level_nodes[k]);
			}
		}
		labels_conjunction.clear();
		label_representants.clear();
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

//TODO odtestovat ze to tak funguje
void MaxTree::getLabelRepresentants(int labels, cv::Mat labelled_image, std::map<int, cv::Point>& representants)
{
	int curr_label = 1;
	for (int y = 0; y < labelled_image.rows; y++) 
	{
		unsigned* row = (unsigned*)(labelled_image.ptr(y));
		for (int x = 0; x < labelled_image.cols; x++) 
		{
			if (row[x] == curr_label) 
			{
				representants[curr_label] = cv::Point(x,y);
				curr_label++;
			}
			if (curr_label >= labels) return;
		}
	}	

}

void MaxTree::retrieveLabelConjunction(int curr_lvl, std::map<int, cv::Point>& representants, std::map<int, std::vector<int>>& labels_conjuction)
{
	cv::Mat prev_lvl = labels[curr_lvl - 1];

	for (auto curr_label : representants) 
	{
		int value = prev_lvl.at<int>(curr_label.second);
		if (labels_conjuction.find(value) != labels_conjuction.end()) {
			labels_conjuction[value].push_back(curr_label.first);
		} 
		else 
		{
			labels_conjuction[value] = std::vector<int>{ curr_label.first };
		}	
	}

}

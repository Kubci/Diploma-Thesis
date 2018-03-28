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

	//1-255
	cv::Mat curr_lvl;
	cv::Mat curr_labels;
	cv::Mat curr_stats;
	cv::Mat centroids;
	std::map<int, cv::Point> label_representants;
	std::map<int, std::vector<int>> labels_conjunction;

	for (int i = 1; i < 256; i++)
	{

		//Threshold
		curr_lvl = image >= i;
		if (cv::countNonZero(curr_lvl) == 0){
			return;
		}

		//Labelling
		int labels_count = cv::connectedComponentsWithStats(curr_lvl, curr_labels, curr_stats, centroids, 4);
		labels.push_back(curr_labels.clone());
		getLabelRepresentants(labels_count, curr_labels, label_representants);
		
		//Creating nodes of a level
		std::vector<Node*> level_nodes;
		level_nodes.push_back(new Node(i, 0)); // dummy node to allign labels with array positions
		for (int k = 1; k < labels_count; k++) {
			Node* n = new Node(i, k);
			n->setRepresentant(label_representants[k]);
			int* row = (int*)curr_stats.ptr(k);
			n->setBB_width(row[2]);			//CC_STAT_WIDTH
			n->setBB_height(row[3]);		//CC_STAT_HEIGHT
			n->setArea(row[4]);				//CC_STAT_AREA
			cv::Mat roi = cv::Mat(curr_labels, cv::Rect(row[0], row[1], row[2], row[3])) == k;
			double hu[7] = {0};
			cv::HuMoments(cv::moments(curr_labels, true), hu);
			n->setHuMoments(hu);
			level_nodes.push_back(n);
		}
		tree.push_back(level_nodes);

		//Linking tree
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

void MaxTree::pruneAbove(Node * node)
{
	Node* parrent = node->getPredecessor();
	node->setPredecessor(nullptr);
	parrent->removeSuccessor(node);
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

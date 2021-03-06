#pragma once
#include "ParamsGT.h"
#include "MaxTreeBerger.h"

void compareToGT(MaxTreeBerger& m_tree, ParamsGT& gt);
cv::Mat findBestGlobalJaccard(MaxTreeBerger& m_tree, ParamsGT& gt, std::string& path, std::string& name, std::ofstream& file);
void findBestSingleJaccard(MaxTreeBerger& m_tree, ParamsGT& gt, std::string& path, std::string& name);
cv::Mat exportBestRois(MaxTreeBerger& m_tree, ParamsGT & gt, std::string& path, std::string& name, std::ofstream& file);



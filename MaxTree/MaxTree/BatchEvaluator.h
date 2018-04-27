#pragma once
#include <string>
#include <vector>
#include "ParamsGT.h"
#include "MaxTreeBerger.h"
#include "TreeEvalToGT.h"
#include "Utils.h"
#include "opencv2\opencv.hpp"
#include "dirent.h"
#include <iostream>
#include <fstream>

void contentsOFFolder(std::vector<std::string>& names, std::string& path) {
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path.c_str())) != NULL)
	{
		readdir(dir);
		readdir(dir);
		while ((ent = readdir(dir)) != NULL)
		{
			names.push_back(ent->d_name);
		}
		closedir(dir);
	}
}

cv::Mat rutine(std::string& gt_path, std::string& image_path, std::string& name, std::string& path2, std::ofstream& file, std::string& folder) {

	cv::Mat r1;
	cv::Mat gt = cv::imread(gt_path, CV_LOAD_IMAGE_ANYDEPTH);
	cv::Mat image = cv::imread(image_path, CV_LOAD_IMAGE_ANYDEPTH);
	convertTo8BitImage(gt);
	convertTo8BitImage(image);
	if (folder.compare("Angiogenesis") == 0) 
	{
		cv::dilate(gt, gt, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));
	}
	if (image.cols == 0 || image.rows == 0) return r1;

	ParamsGT gtp(gt);
	MaxTreeBerger m_tree(image);

	findBestSingleJaccard(m_tree, gtp, path2 + "\\res3\\", name);
	r1 = exportBestRois(m_tree, gtp, path2 + "\\res3\\", name, file);
	cv::Mat r2 = findBestGlobalJaccard(m_tree, gtp, path2 + "\\res2\\", name, file);
	r1 = r1 + r2;
	return r1;
}

void evalueteInFolder(std::string& path) {
	DIR *dir;

	std::vector <std::string> sub_dirs;
	contentsOFFolder(sub_dirs, path);

	for (std::string s : sub_dirs)
	{
		std::vector <std::string> files;
		std::string path2 = path + s + "\\";
		contentsOFFolder(files, path2);

		std::ofstream file;
		file.open(path2+s+".txt");
		file << s << std::endl;
		file << "label;" << "jaccard;" << "global max jaccard" << std::endl;

		for (std::string f : files)
		{

			std::stringstream ss; ss.str(f);
			std::string name;
			std::string suff;
			std::getline(ss, name, '.');
			std::getline(ss, suff, '.');
			std::transform(suff.begin(), suff.end(), suff.begin(), tolower);

			if (suff.compare("tif") != 0) continue;

			file << name << std::endl;
			std::cout << name << std::endl;

			std::string gt_path = path2 + "GT\\" + name + "_gt.tif";
			std::string image_path = path2 + name + ".tif";

			cv::Mat r1 = rutine(gt_path, image_path, name, path2, file, s);

			cv::imwrite(path2 + "\\res3\\" + name + "_overlay.png", r1);
		}
		file.close();
	}
}
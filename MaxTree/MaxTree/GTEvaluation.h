#pragma once
#include <string>
#include <vector>
#include "dirent.h"

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

void evalueteInFolder(std::string& path) {
	DIR *dir;

	std::vector <std::string> sub_dirs;
	contentsOFFolder(sub_dirs, path);
	
	for (std::string s : sub_dirs)
	{
		std::vector <std::string> files;
		std::string path2 = path + s + "\\";
		contentsOFFolder(files, path2);

		for (std::string f : files)
		{
			if ((dir = opendir((path + f).c_str())) != NULL)
			{
				closedir(dir);
				continue;
			}

			std::stringstream ss; ss.str(f);
			std::string name;
			std::getline(ss, name, '.');

			std::cout << name << std::endl;

			std::string string_path = path2 + "GT\\" + name + "_gt.tif";
			std::string image_path = path2 + name + ".tif";
			cv::Mat gt = cv::imread(string_path, CV_LOAD_IMAGE_ANYDEPTH);
			cv::Mat image = cv::imread(image_path, CV_LOAD_IMAGE_ANYDEPTH);
			convertTo8BitImage(gt);
			convertTo8BitImage(image);
			if (image.cols == 0 || image.rows == 0) continue;

			GTParams gtp(gt);
			GTParams gtp2(gt);
			MaxTreeBerger m_tree(image);
			m_tree.findBestSingleJaccard(gtp, path2 + "\\res3\\", name);
			cv::Mat r2 = m_tree.findBestJaccard(gtp2, path2 + "\\res2\\", name);
			//m_tree.compareToGT(gtp);
			cv::Mat r1 = m_tree.exportBestRois(gtp, path2 + "\\res3\\", name);
			cv::imwrite(path2 + "\\res3\\" + name + "_overlay.png", r1 + r2);
		}
	}
}
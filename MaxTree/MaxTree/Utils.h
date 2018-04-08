#pragma once
#include "opencv2/opencv.hpp"

void AddComponentToImage(cv::Mat& reconstructed, cv::Mat& labelled_level, int label);
void AddComponentsToImage(cv::Mat& reconstructed, cv::Mat& labelled_level, std::vector<bool>& labels);
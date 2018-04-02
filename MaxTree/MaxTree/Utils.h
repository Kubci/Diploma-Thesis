#pragma once
#include "opencv2/opencv.hpp"

void AddComponentToImage(cv::Mat reconstructed, cv::Mat labelled_level, int label);

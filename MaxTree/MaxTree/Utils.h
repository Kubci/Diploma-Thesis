#pragma once
#include "opencv2/opencv.hpp"
#include "SetUF.h"
//For native tree
void AddComponentToImage(cv::Mat& reconstructed, cv::Mat& labelled_level, int label);
void AddComponentsToImage(cv::Mat& reconstructed, cv::Mat& labelled_level, std::vector<bool>& labels);

//for berger tree
void convertTo8BitImage(cv::Mat & image);
float distance(cv::Point2f& first, cv::Point2f& second);
void centroid(cv::Mat& roi, SetUFPix* p, cv::Point2f& centroid);
void addRoiToImage(cv::Mat& image, cv::Mat& roi, SetUFPix* p);

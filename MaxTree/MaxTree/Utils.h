#pragma once
#include "opencv2/opencv.hpp"
#include "SetUF.h"

//for berger tree
void convertTo8BitImage(cv::Mat & image);
float distance(cv::Point2f& first, cv::Point2f& second);
void centroid(cv::Mat& roi, SetUFPix* p, cv::Point2f& centroid);
void addRoiToImage(cv::Mat& image, cv::Mat& roi, SetUFPix* p);

// blends 2 mask into rgb image
cv::Mat blend(cv::Mat& image, cv::Mat overlay);

void fillHoles(cv::Mat image);
void removeEdgeObjs(cv::Mat image);
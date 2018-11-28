#pragma once
#include "opencv2/opencv.hpp"

using namespace cv;

vector<Mat*> DetectBody(Mat* imageGray, Mat* image);
vector<Mat*> DetectMouth(Mat* imageGray, Mat* image, int sliderValue);
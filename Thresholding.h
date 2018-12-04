#pragma once
#include "opencv2/opencv.hpp"

using namespace cv;

Mat* OtsuThresholding(Mat* imageGray);
Mat* DynamicThresholding(Mat* imageGray, int sliderValue);
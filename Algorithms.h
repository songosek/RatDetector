#pragma once
#include "opencv2/opencv.hpp"

using namespace cv;

Mat* Harris(Mat* imageGray);
Mat* NowyHarris(Mat* imageGray);
void ShiTomasi();
Mat* OtsuThresholding(Mat* imageGray);
Mat* DynamicThresholding(Mat* imageGray);
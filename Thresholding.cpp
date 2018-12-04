#include "Thresholding.h"
#include "stdafx.h"
#include "opencv2/opencv.hpp"
using namespace cv;

Mat* OtsuThresholding(Mat* imageGray) {
	
	Mat* binaryImage = new Mat();
	double border = 200;
	threshold(*imageGray, *binaryImage, border, 255, THRESH_BINARY | THRESH_OTSU);

	return binaryImage;
}

Mat* DynamicThresholding(Mat* imageGray, int sliderValue) {

	Mat* binaryImage = new Mat();
	double percent = (double)sliderValue / 10000;
	vector<float> brightnessVector = (*imageGray).reshape(0, 1);
	cv::sort(brightnessVector, brightnessVector, CV_SORT_DESCENDING);
	double border = (double)brightnessVector[brightnessVector.size() * percent];
	threshold(*imageGray, *binaryImage, border, 255, THRESH_BINARY);

	return binaryImage;
}


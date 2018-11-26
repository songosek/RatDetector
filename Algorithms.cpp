#include "Algorithms.h"
#include "stdafx.h"
#include "opencv2/opencv.hpp"
using namespace cv;

Mat* Harris(Mat* imageGray) {

	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros((*imageGray).size(), CV_32FC1);
	Mat* output = new Mat();
	*output = (*imageGray).clone();

	/// Detector parameters
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;
	int thresh = 150;


	/// Detecting corners
	cornerHarris(*imageGray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

	/// Normalizing
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);

	/// Drawing a circle around corners
	//Size *size = new Size(20, 1);
	//Mat list= Mat::zeros(*size, CV_32FC1);
	for (int j = 0; j < dst_norm.rows; j++)
	{
		for (int i = 0; i < dst_norm.cols; i++)
		{
			//for (int p = 0; p < list.cols; p++) {
			//	if ((int)dst_norm.at<float>(j, i) > (int)list.at<float>(0, p)) {
			//		
			//	}
			//}
			if ((int)dst_norm.at<float>(j, i) > thresh)
			{
				circle(*output, Point(i, j), 3, Scalar(0), 1, 8, 0);
			}
		}
	}

	return output;
}

//DEBUG STUFF
Mat* NowyHarris(Mat* imageGray) {

	Mat dst, dst_norm, dst_norm_scaled;
	dst = Mat::zeros((*imageGray).size(), CV_32FC1);
	Mat* output = new Mat();
	*output = (*imageGray).clone();

	/// Detector parameters
	int blockSize = 5;
	int apertureSize = 1;
	double k = 0.04;
	int thresh = 150;

	/// Detecting corners
	cornerHarris(*imageGray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

	/// Normalizing
	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
	convertScaleAbs(dst_norm, dst_norm_scaled);
	
	*output = dst_norm_scaled.clone();

	return output;
}

void ShiTomasi() {

}

Mat* OtsuThresholding(Mat* imageGray) {
	
	Mat* binaryImage = new Mat();
	double border = 200;
	threshold(*imageGray, *binaryImage, border, 255, THRESH_BINARY | THRESH_OTSU);

	return binaryImage;
}

Mat* DynamicThresholding(Mat* imageGray) {

	Mat* binaryImage = new Mat();
	vector<float> brightnessVector = (*imageGray).reshape(0, 1);
	cv::sort(brightnessVector, brightnessVector, CV_SORT_DESCENDING);
	double percent = 0.004;
	double border = (double)brightnessVector[brightnessVector.size() * percent];
	threshold(*imageGray, *binaryImage, border, 255, THRESH_BINARY);

	return binaryImage;
}


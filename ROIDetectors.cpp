#include "ROIDetectors.h"
#include "stdafx.h"
#include "opencv2/opencv.hpp"
#include "Thresholding.h"

using namespace cv;

vector<Mat*> DetectBody(Mat* imageGray, Mat* image) {
	
	vector<Mat*> output;
	Mat* binaryImage = OtsuThresholding(imageGray);
	output.push_back(binaryImage);

	Mat* edges = new Mat();
	Canny(*binaryImage, *edges, 100, 200); 
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(*edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	Mat* result = new Mat();
	*result = (*image).clone();
	
	for (int i = 0; i < contours.size(); i++)
	{
		if(contours[i].size()>50)
			drawContours(*result, contours, i, Scalar(0, 0, 255), 2, 8, hierarchy, 0);
	}
	output.push_back(result);

	return output;
}

vector<Mat*> DetectMouth(Mat* imageGray, Mat* image, int sliderValue) {

	vector<Mat*> output;

	Mat* binaryImage = DynamicThresholding(imageGray, sliderValue);
	output.push_back(binaryImage);

	Mat* blobBinaryImage = new Mat();
	Mat structuringElement = getStructuringElement(MORPH_ELLIPSE, Size(40, 40));	morphologyEx(*binaryImage, *blobBinaryImage, MORPH_CLOSE, structuringElement);

	Mat* edges = new Mat();
	Canny(*blobBinaryImage, *edges, 100, 200); 
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(*edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	Mat* circles = new Mat();
	*circles = Mat::zeros((*image).size(), CV_8U);
	for (int i = 0; i < contours.size(); i++) {
		if (contours[i].size() > 5) {
			Rect rect = boundingRect(contours[i]);
			Point center = Point(rect.x + rect.width / 2, rect.y + rect.height / 2);
			circle(*circles, center, 20, Scalar(255), CV_FILLED);
		}
	}
	Mat* bodyMask = OtsuThresholding(imageGray);
	Mat* mouthRegion = new Mat();
	(*circles).copyTo(*mouthRegion, *bodyMask);

	Canny(*mouthRegion, *edges, 100, 200);
	findContours(*edges, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

	Mat* result = new Mat();
	*result = (*image).clone();
	for (int i = 0; i < contours.size(); i++)
	{
		drawContours(*result, contours, i, Scalar(255, 0, 0), 2, 8, hierarchy, 0);
	}
	output.push_back(result);
	return output;
}
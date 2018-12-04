#pragma once
// CImgWnd
#include <vfw.h>
#include "opencv2/opencv.hpp"

#define DEF_IMG_WIDTH	320
#define DEF_IMG_HEIGHT	240

using namespace cv;

class CImgWnd : public CWnd
{
	DECLARE_DYNAMIC(CImgWnd)

public:
	CImgWnd();
	virtual ~CImgWnd();

protected:
	BYTE*	imageBytes;
	BYTE*	imageGrayBytes;
	BITMAPINFOHEADER bmih;
	BITMAPINFOHEADER blackBmih;
	HDRAWDIB hdd;
	int		sourceWidth;
	int		sourceHeight;

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL Create(const RECT& rect, CWnd*  pParentWnd, UINT nID);
	BOOL SetImage(Mat* pmImage);
	afx_msg void OnPaint();
};


#pragma once
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
	BYTE*	m_pImage;
	BYTE*	m_pBlackImg;
	BITMAPINFOHEADER bmih;
	BITMAPINFOHEADER blackBmih;
	HDRAWDIB m_hdd;
	int		m_iSrcWidth;
	int		m_iSrcHeight;

protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL Create(const RECT& rect, CWnd*  pParentWnd, UINT nID);
	BOOL SetImage(Mat* pmImage);
	afx_msg void OnPaint();
};


#pragma once
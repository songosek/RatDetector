#pragma once
#include "ImgWnd.h"
#include "opencv2/opencv.hpp"
#include "ROIDetectors.h"

using namespace cv;

class CDetectorDlg : public CDialogEx
{
// Construction
public:
	CDetectorDlg(CWnd* pParent = nullptr);	

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DETECTOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	CImgWnd displayedImage; 
	CImgWnd displayedImage2;
	CImgWnd displayedImage3;
	CStatic fileName;
	CSliderCtrl percentSlider;
	CStatic percentSliderValue;

	Mat* image; 
	Mat* imageGray;

	vector<string> filePaths;
	int currentFile;
	int currentDetector;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedNextimage();
	afx_msg void OnBnClickedPreviousimage();
	afx_msg void OnBnClickedBody();
	afx_msg void OnBnClickedMouth();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	
};


// DetectorDlg.h : header file
//

#pragma once
#include "ImgWnd.h"
#include "Algorithms.h"
#include "opencv2/opencv.hpp"
#include "ROIDetectors.h"

using namespace cv;

// CDetectorDlg dialog
class CDetectorDlg : public CDialogEx
{
// Construction
public:
	CDetectorDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DETECTOR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	Mat* image; // zmienna, do której jest wczytywane zdjecie po wcisnieciu Otworz
	Mat* imageGray;
	CImgWnd displayedImage; // zmienna wyswietlajaca zdjecie
	CImgWnd displayedImage2;
	CImgWnd displayedImage3;
	vector<string> filePaths;
	int currentFile;
	int currentDetector;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedBody();
	afx_msg void OnBnClickedWykryjpyszczek();
	CSliderCtrl percentSlider;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CStatic percentSliderValue;
	afx_msg void OnBnClickedNextimage();
	afx_msg void OnBnClickedPreviousimage();
	CStatic fileName;
};

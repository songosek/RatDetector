#include "stdafx.h"
#include "Detector.h"
#include "DetectorDlg.h"
#include "afxdialogex.h"
#include <filesystem>
namespace fs = std::filesystem;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDetectorDlg::CDetectorDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DETECTOR_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDetectorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, PERCENT_SLIDER, percentSlider);
	DDX_Control(pDX, PERCENTSLIDERVALUE, percentSliderValue);
	DDX_Control(pDX, FILENAME, fileName);
}

BEGIN_MESSAGE_MAP(CDetectorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOPEN, &CDetectorDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDNEXTIMAGE, &CDetectorDlg::OnBnClickedNextimage)
	ON_BN_CLICKED(IDPREVIOUSIMAGE, &CDetectorDlg::OnBnClickedPreviousimage)
	ON_BN_CLICKED(IDBODY, &CDetectorDlg::OnBnClickedBody)
	ON_BN_CLICKED(IDMOUTH, &CDetectorDlg::OnBnClickedMouth)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

// CDetectorDlg message handlers

BOOL CDetectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);		
	SetIcon(m_hIcon, FALSE);	

	percentSlider.SetRange(1, 50);
	percentSlider.SetPos(4);
	
	currentDetector = 0;

	CRect r(25, 15, DEF_IMG_WIDTH + 25, DEF_IMG_HEIGHT + 15);
	displayedImage.Create(r, this, 100);
	CRect r2(25, DEF_IMG_HEIGHT + 30, DEF_IMG_WIDTH + 25, 2 * DEF_IMG_HEIGHT + 30);
	displayedImage2.Create(r2, this, 101);
	CRect r3(25, 2*DEF_IMG_HEIGHT + 45, DEF_IMG_WIDTH + 25, 3 * DEF_IMG_HEIGHT + 45);
	displayedImage3.Create(r3, this, 102);

	return TRUE; 
}

void CDetectorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CDetectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDetectorDlg::OnBnClickedOpen()
{
	const TCHAR extensionFilter[] = _T("JPG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, extensionFilter, this);
	if (dlg.DoModal() == IDOK)
	{
		currentFile = 0;
		filePaths.clear(); 
		image = new Mat();
		imageGray = new Mat();

		CT2CA pszConvertedAnsiString(dlg.GetPathName());
		string path(pszConvertedAnsiString);
		string folder = path.substr(0, path.find_last_of("\\/"));
		for (auto & p : fs::directory_iterator(folder)) {
			filePaths.push_back(p.path().u8string());
		}
		for (int i = 0; i < filePaths.size() - 1; i++) {
			int a = path.compare(filePaths[i]);
			if (path.compare(filePaths[i]) == 0) {
				currentFile = i;
				break;
			}
		}

		string tempName = path;
		CA2T t(tempName.erase(0, path.find_last_of("\\/") + 1).c_str());
		LPCTSTR name = t;
		fileName.SetWindowText(name);

		*image = imread(filePaths[currentFile], CV_LOAD_IMAGE_COLOR);
		cvtColor(*image, *imageGray, CV_RGB2GRAY);

		displayedImage.SetImage(image);
	}
}

void CDetectorDlg::OnBnClickedNextimage()
{
	if (currentFile < (filePaths.size() - 1)) {
		*image = imread(filePaths[++currentFile], CV_LOAD_IMAGE_COLOR);
		cvtColor(*image, *imageGray, CV_RGB2GRAY);

		displayedImage.SetImage(image);

		string tempName = filePaths[currentFile];
		CA2T t(tempName.erase(0, tempName.find_last_of("\\/") + 1).c_str());
		LPCTSTR name = t;
		fileName.SetWindowText(name);

		switch (currentDetector) {
		case 1:
			CDetectorDlg::OnBnClickedBody();
			break;
		case 2:
			CDetectorDlg::OnBnClickedMouth();
			break;
		}
	}
}


void CDetectorDlg::OnBnClickedPreviousimage()
{
	if (currentFile > 0) {
		*image = imread(filePaths[--currentFile], CV_LOAD_IMAGE_COLOR);
		cvtColor(*image, *imageGray, CV_RGB2GRAY);

		displayedImage.SetImage(image);

		string tempName = filePaths[currentFile];
		CA2T t(tempName.erase(0, tempName.find_last_of("\\/") + 1).c_str());
		LPCTSTR name = t;
		fileName.SetWindowText(name);

		switch (currentDetector) {
		case 1:
			CDetectorDlg::OnBnClickedBody();
			break;
		case 2:
			CDetectorDlg::OnBnClickedMouth();
			break;
		}
	}
}


void CDetectorDlg::OnBnClickedBody()
{
	if (imageGray != NULL && image != NULL) {
		currentDetector = 1;

		vector<Mat*> outputImages = DetectBody(imageGray, image);
		displayedImage2.SetImage(outputImages[0]);
		displayedImage3.SetImage(outputImages[1]);
	}
}


void CDetectorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode != 8)
	{
		CString value;
		double calculated = (double)nPos / 100;
		value.Format(_T("%.2f%%"), calculated);
		percentSliderValue.SetWindowText(value);
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}


void CDetectorDlg::OnBnClickedMouth()
{
	if (image != NULL && imageGray != NULL) {
		currentDetector = 2;

		vector<Mat*> outputImages = DetectMouth(imageGray, image, percentSlider.GetPos());
		displayedImage2.SetImage(outputImages[0]);
		displayedImage3.SetImage(outputImages[1]);
	}
}

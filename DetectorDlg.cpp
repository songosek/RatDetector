
// DetectorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Detector.h"
#include "DetectorDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDetectorDlg dialog



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
}

BEGIN_MESSAGE_MAP(CDetectorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDHARRIS, &CDetectorDlg::OnBnClickedHarris)
	ON_BN_CLICKED(IDOPEN, &CDetectorDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDBODY, &CDetectorDlg::OnBnClickedBody)
	ON_BN_CLICKED(IDNOWYHARRIS, &CDetectorDlg::OnBnClickedNowyharris)
	ON_BN_CLICKED(IDWYKRYJPYSZCZEK, &CDetectorDlg::OnBnClickedWykryjpyszczek)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CDetectorDlg message handlers

BOOL CDetectorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	percentSlider.SetRange(1, 100);
	percentSlider.SetPos(4);

	CRect r(25, 15, DEF_IMG_WIDTH + 25, DEF_IMG_HEIGHT + 15);
	displayedImage.Create(r, this, 100);
	CRect r2(25, DEF_IMG_HEIGHT + 30, DEF_IMG_WIDTH + 25, 2 * DEF_IMG_HEIGHT + 30);
	displayedImage2.Create(r2, this, 101);
	CRect r3(25, 2*DEF_IMG_HEIGHT + 45, DEF_IMG_WIDTH + 25, 3 * DEF_IMG_HEIGHT + 45);
	displayedImage3.Create(r3, this, 102);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDetectorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDetectorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDetectorDlg::OnBnClickedHarris()
{
	if(imageGray!=NULL)
		displayedImage.SetImage(Harris(imageGray));
}

void CDetectorDlg::OnBnClickedOpen()
{
	const TCHAR extensionFilter[] = _T("JPG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"); // tablica do filtrowania rozszerzen plikow
	CFileDialog dlg(TRUE, _T("jpg"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, extensionFilter, this);
	if (dlg.DoModal() == IDOK)
	{
		image = new Mat();
		imageGray = new Mat();
		CT2CA pszConvertedAnsiString(dlg.GetPathName());
		string path(pszConvertedAnsiString);

		*image = imread(path, CV_LOAD_IMAGE_COLOR);
		cvtColor(*image, *imageGray, CV_RGB2GRAY);

		displayedImage.SetImage(image);
	}
}


void CDetectorDlg::OnBnClickedBody()
{
	if (imageGray != NULL && image != NULL) {
		vector<Mat*> outputImages = DetectBody(imageGray, image);
		displayedImage2.SetImage(outputImages[0]);
		displayedImage3.SetImage(outputImages[1]);
	}
}


void CDetectorDlg::OnBnClickedNowyharris()
{
	if (imageGray != NULL)
		displayedImage2.SetImage(NowyHarris(imageGray));
		
}


void CDetectorDlg::OnBnClickedWykryjpyszczek()
{
	if (imageGray != NULL) {
		vector<Mat*> outputImages = DetectMouth(imageGray, image, percentSlider.GetPos());
		displayedImage2.SetImage(outputImages[0]);
		displayedImage3.SetImage(outputImages[1]);
	}
}


void CDetectorDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (nSBCode != 8)
	{
		CString value;
		double calculated = (double)nPos / 10000;
		value.Format(_T("%f"), calculated);
		percentSliderValue.SetWindowText(value);
		CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}

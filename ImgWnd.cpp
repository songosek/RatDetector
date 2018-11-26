// ImgWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ImgWnd.h"

// CImgWnd

IMPLEMENT_DYNAMIC(CImgWnd, CWnd)

CImgWnd::CImgWnd()
{
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biClrUsed = 0;
	bmih.biWidth = DEF_IMG_WIDTH;
	bmih.biHeight = DEF_IMG_HEIGHT;

	blackBmih = bmih;

	int iBitmapWidth = (DEF_IMG_WIDTH * 3 % 4 == 0) ? DEF_IMG_WIDTH * 3 : DEF_IMG_WIDTH * 3 + 4 - (DEF_IMG_WIDTH * 3 % 4);

	m_pBlackImg = new BYTE[iBitmapWidth*DEF_IMG_HEIGHT];
	for (int i = 0; i < iBitmapWidth*DEF_IMG_HEIGHT; i++)
		m_pBlackImg[i] = 0;

	m_pImage = NULL;

	m_hdd = DrawDibOpen();
}

CImgWnd::~CImgWnd()
{
	if (m_pImage)
		delete m_pImage;

	if (m_pBlackImg)
		delete m_pBlackImg;

	DrawDibClose(m_hdd);
}


BEGIN_MESSAGE_MAP(CImgWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CImgWnd message handlers


BOOL CImgWnd::Create(const RECT& rect, CWnd*  pParentWnd, UINT nID)
{
	BOOL bRes;

	bRes = CWnd::Create(NULL, NULL,
		WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL, rect, pParentWnd, nID, NULL);

	ShowScrollBar(SB_BOTH, FALSE);

	return bRes;
}

//BOOL CImgWnd::SetImage(IplImage* pImage)
BOOL CImgWnd::SetImage(Mat* image)
{
	BOOL bRes = TRUE;
	Mat* pmImage = new Mat(cvSize(DEF_IMG_WIDTH, DEF_IMG_HEIGHT), CV_8UC1);
	resize(*image, *pmImage, cvSize(DEF_IMG_WIDTH, DEF_IMG_HEIGHT));

	if (pmImage)
	{
		if (m_pImage)
			delete m_pImage;

		if ((pmImage->type() != CV_8UC3) && (pmImage->type() != CV_8UC1))
			return FALSE;

		m_iSrcWidth = pmImage->cols;
		m_iSrcHeight = pmImage->rows;
		int nChannels = 3;
		int iLineSize = m_iSrcWidth * nChannels;
		int iBitmapWidth = (iLineSize % 4 == 0) ? iLineSize : iLineSize + 4 - (iLineSize % 4);

		m_pImage = new BYTE[iBitmapWidth*m_iSrcHeight];

		for (int i = 0; i < iBitmapWidth*m_iSrcHeight; i++)
			m_pImage[i] = 0;

		for (int i = 0; i < pmImage->rows; i++)
		{
			if (pmImage->type() == CV_8UC3)
				memcpy(m_pImage + i * iBitmapWidth, pmImage->data + (pmImage->rows - 1 - i)*m_iSrcWidth*nChannels, m_iSrcWidth*nChannels);
			else
			{
				for (int j = 0; j < pmImage->cols; j++)
				{
					m_pImage[i*iBitmapWidth + 3 * j] = m_pImage[i*iBitmapWidth + 3 * j + 1] = m_pImage[i*iBitmapWidth + 3 * j + 2] = pmImage->at<uchar>(m_iSrcHeight - 1 - i, j);
				}
			}
		}

		bmih.biWidth = pmImage->cols;
		bmih.biHeight = pmImage->rows;

		InvalidateRect(NULL);
	}
	else
		bRes = FALSE;

	return bRes;
}

void CImgWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// Do not call CWnd::OnPaint() for painting messages

	DrawDibDraw(m_hdd, dc.m_hDC, 0, 0, DEF_IMG_WIDTH, DEF_IMG_HEIGHT, &blackBmih, m_pBlackImg, 0, 0, DEF_IMG_WIDTH, DEF_IMG_HEIGHT, NULL);

	if (m_pImage)
	{
		CRect r;
		GetClientRect(r);

		double aspect2 = ((double)(r.Width())) / ((double)(m_iSrcWidth));
		double aspect3 = ((double)(r.Height())) / ((double)(m_iSrcHeight));

		int iWys, iSzer;
		int iOffX = 0;
		int iOffY = 0;

		double dHeight = (double)(m_iSrcHeight);
		if (dHeight*aspect2 > r.Height())
		{
			iWys = r.Height();
			iSzer = (int)((double)(m_iSrcWidth)* aspect3);
			iOffX = (int)(r.Width() - iSzer) / 2;
		}
		else
		{
			iWys = (int)(dHeight*aspect2);
			iSzer = r.Width();
			iOffY = (int)(r.Height() - iWys) / 2;
		}

		DrawDibDraw(m_hdd, dc.m_hDC, iOffX, iOffY, iSzer, iWys, &bmih, m_pImage, 0, 0, m_iSrcWidth, m_iSrcHeight, NULL);
	}
}
#include "stdafx.h"
#include "ImgWnd.h"

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

	imageGrayBytes = new BYTE[iBitmapWidth*DEF_IMG_HEIGHT];
	for (int i = 0; i < iBitmapWidth*DEF_IMG_HEIGHT; i++)
		imageGrayBytes[i] = 0;

	imageBytes = NULL;

	hdd = DrawDibOpen();
}

CImgWnd::~CImgWnd()
{
	if (imageBytes)
		delete imageBytes;

	if (imageGrayBytes)
		delete imageGrayBytes;

	DrawDibClose(hdd);
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

BOOL CImgWnd::SetImage(Mat* sourceImage)
{
	BOOL bRes = TRUE;
	Mat* pmImage = new Mat(cvSize(DEF_IMG_WIDTH, DEF_IMG_HEIGHT), CV_8UC1);
	resize(*sourceImage, *pmImage, cvSize(DEF_IMG_WIDTH, DEF_IMG_HEIGHT));

	if (pmImage)
	{
		if (imageBytes)
			delete imageBytes;

		if ((pmImage->type() != CV_8UC3) && (pmImage->type() != CV_8UC1))
			return FALSE;

		sourceWidth = pmImage->cols;
		sourceHeight = pmImage->rows;
		int nChannels = 3;
		int iLineSize = sourceWidth * nChannels;
		int iBitmapWidth = (iLineSize % 4 == 0) ? iLineSize : iLineSize + 4 - (iLineSize % 4);

		imageBytes = new BYTE[iBitmapWidth*sourceHeight];

		for (int i = 0; i < iBitmapWidth*sourceHeight; i++)
			imageBytes[i] = 0;

		for (int i = 0; i < pmImage->rows; i++)
		{
			if (pmImage->type() == CV_8UC3)
				memcpy(imageBytes + i * iBitmapWidth, pmImage->data + (pmImage->rows - 1 - i)*sourceWidth*nChannels, sourceWidth*nChannels);
			else
			{
				for (int j = 0; j < pmImage->cols; j++)
				{
					imageBytes[i*iBitmapWidth + 3 * j] = imageBytes[i*iBitmapWidth + 3 * j + 1] = imageBytes[i*iBitmapWidth + 3 * j + 2] = pmImage->at<uchar>(sourceHeight - 1 - i, j);
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
	CPaintDC dc(this);

	DrawDibDraw(hdd, dc.m_hDC, 0, 0, DEF_IMG_WIDTH, DEF_IMG_HEIGHT, &blackBmih, imageGrayBytes, 0, 0, DEF_IMG_WIDTH, DEF_IMG_HEIGHT, NULL);

	if (imageBytes)
	{
		CRect r;
		GetClientRect(r);

		double aspect2 = ((double)(r.Width())) / ((double)(sourceWidth));
		double aspect3 = ((double)(r.Height())) / ((double)(sourceHeight));

		int height, width;
		int iOffX = 0;
		int iOffY = 0;

		double dHeight = (double)(sourceHeight);
		if (dHeight*aspect2 > r.Height())
		{
			height = r.Height();
			width = (int)((double)(sourceWidth)* aspect3);
			iOffX = (int)(r.Width() - width) / 2;
		}
		else
		{
			height = (int)(dHeight*aspect2);
			width = r.Width();
			iOffY = (int)(r.Height() - height) / 2;
		}

		DrawDibDraw(hdd, dc.m_hDC, iOffX, iOffY, width, height, &bmih, imageBytes, 0, 0, sourceWidth, sourceHeight, NULL);
	}
}
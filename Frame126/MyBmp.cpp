#include "stdafx.h"
#include "MyBmp.h"


CMyBmp::CMyBmp()
{
}


CMyBmp::~CMyBmp()
{
	Release();
}
void CMyBmp::Load_Bmp(const TCHAR * pFilePath)
{
	HDC		hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);	// ���ڰ��� �ش��ϴ� dc�� ȣȯ�� �� �ִ� ȭ�� dc�� �������ִ� �Լ�
	ReleaseDC(g_hWnd, hDC);				// ���۷��� ī��Ʈ ����

										// ��ο� �ִ� ��Ʈ�� �̹��� �о�鿩���� �Լ�
	m_hBitMap = (HBITMAP)LoadImage(NULL,		// ���α׷� �ν��Ͻ� �ڵ�, �̹����� ���α׷����� �о�� ���� �ƴ϶� ���Ϸ� �о�� ���̱� ������ null
		pFilePath,	// ������ ���
		IMAGE_BITMAP,	// � Ÿ������ �о���� �����ϴ� �÷���
		0,			// 4,5 ���� : ����, ���� ũ�⸦ ����, ���Ͽ��� �о�� ���̱� ������ ���� ������ �ʿ䰡 ����
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION); // LR_LOADFROMFILE : ���Ͽ��� �̹����� �ҷ����ڴ�, LR_CREATEDIBSECTION : �о�� ������ DIB���·� ��ȯ

												// �غ��� dc�� gdi������Ʈ�� �ҷ��� ��Ʈ���� �����ϱ� ���� ����ϴ� �Լ�
	m_hOldMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);
	m_hRotBitMap = (HBITMAP)GetRotatedBitmap(m_hMemDC, m_hBitMap, PI, 0xffffff);
	m_hRotOldBitMap = (HBITMAP)SelectObject(m_hRotMemDC, m_hRotBitMap);
}

void CMyBmp::Release(void)
{
	SelectObject(m_hMemDC, m_hOldMap);
	DeleteObject(m_hBitMap);		// loadimage �ҷ��� ������Ʈ�� �����ϴ� �Լ�, �� ���� dc�� ���� ���õ� gdi������Ʈ�� ������ ���� ����.
	DeleteDC(m_hMemDC);				// ���� ���ǰ� �ִ� ��ü�� �Ժη� �����ϵ��� �� �� ���� ������ ���� ���� ��ġ ������ �ϴ� ����

	SelectObject(m_hRotMemDC, m_hRotOldBitMap);
	DeleteObject(m_hRotBitMap);
	DeleteDC(m_hRotMemDC);

}

HBITMAP CMyBmp::GetRotatedBitmap(HDC hdc, HBITMAP hBitmap, float radians, COLORREF clrBack)
{
	// Create a memory DC compatible with the display
	HDC sourceDC, destDC;
	BITMAP bm;

	HBITMAP hbmResult;
	HBITMAP hbmOldSource;
	HBITMAP hbmOldDest;
	HBRUSH hbrBack;
	HBRUSH hbrOld;

	XFORM xform;

	float cosine, sine;
	int x1, y1, x2, y2, x3, y3, minx, miny, maxx, maxy, w, h;


	sourceDC = CreateCompatibleDC(hdc);
	destDC = CreateCompatibleDC(hdc);



	// Get logical coordinates
	GetObject(hBitmap, sizeof(bm), &bm);

	cosine = cos(radians);
	sine = sin(radians);



	//ȸ���� �̹����� ������ ���Ѵ�.
	// Compute dimensions of the resulting bitmap
	// First get the coordinates of the 3 corners other than origin
	x1 = (int)(bm.bmHeight * sine);
	y1 = (int)(bm.bmHeight * cosine);
	x2 = (int)(bm.bmWidth * cosine + bm.bmHeight * sine);
	y2 = (int)(bm.bmHeight * cosine - bm.bmWidth * sine);
	x3 = (int)(bm.bmWidth * cosine);
	y3 = (int)(-bm.bmWidth * sine);

	minx = min(0, min(x1, min(x2, x3)));
	miny = min(0, min(y1, min(y2, y3)));
	maxx = max(0, max(x1, max(x2, x3)));
	maxy = max(0, max(y1, max(y2, y3)));

	w = maxx - minx;
	h = maxy - miny;



	// Create a bitmap to hold the result
	hbmResult = CreateCompatibleBitmap(hdc, w, h);

	hbmOldSource = (HBITMAP)SelectObject(sourceDC, hBitmap);
	hbmOldDest = (HBITMAP)SelectObject(destDC, hbmResult);



	// Draw the background color before we change mapping mode
	hbrBack = CreateSolidBrush(clrBack);
	hbrOld = (HBRUSH)SelectObject(destDC, hbrBack);
	PatBlt(destDC, 0, 0, w, h, PATCOPY);
	DeleteObject(SelectObject(destDC, hbrOld));



	// We will use world transform to rotate the bitmap
	SetGraphicsMode(destDC, GM_ADVANCED);
	xform.eM11 = cosine;
	xform.eM12 = -sine;
	xform.eM21 = sine;
	xform.eM22 = cosine;
	xform.eDx = (float)-minx;
	xform.eDy = (float)-miny;

	SetWorldTransform(destDC, &xform);



	// Now do the actual rotating - a pixel at a time
	BitBlt(destDC, 0, 0, bm.bmWidth, bm.bmHeight, sourceDC, 0, 0, SRCCOPY);



	// Restore DCs
	SelectObject(sourceDC, hbmOldSource);
	SelectObject(destDC, hbmOldDest);

	DeleteDC(sourceDC);
	DeleteDC(destDC);



	return hbmResult;
}

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

	m_hMemDC = CreateCompatibleDC(hDC);	// 인자값에 해당하는 dc를 호환할 수 있는 화면 dc를 생성해주는 함수
	ReleaseDC(g_hWnd, hDC);				// 레퍼런스 카운트 감소

										// 경로에 있는 비트맵 이미지 읽어들여오는 함수
	m_hBitMap = (HBITMAP)LoadImage(NULL,		// 프로그램 인스턴스 핸들, 이미지는 프로그램에서 읽어올 것이 아니라 파일로 읽어올 것이기 때문에 null
		pFilePath,	// 파일의 경로
		IMAGE_BITMAP,	// 어떤 타입으로 읽어올지 전달하는 플래그
		0,			// 4,5 인자 : 가로, 세로 크기를 전달, 파일에서 읽어올 것이기 때문에 따로 설정할 필요가 없음
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION); // LR_LOADFROMFILE : 파일에서 이미지를 불러오겠다, LR_CREATEDIBSECTION : 읽어온 파일을 DIB형태로 변환

												// 준비한 dc의 gdi오브젝트에 불러온 비트맵을 선택하기 위해 사용하는 함수
	m_hOldMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);
	m_hRotBitMap = (HBITMAP)GetRotatedBitmap(m_hMemDC, m_hBitMap, PI, 0xffffff);
	m_hRotOldBitMap = (HBITMAP)SelectObject(m_hRotMemDC, m_hRotBitMap);
}

void CMyBmp::Release(void)
{
	SelectObject(m_hMemDC, m_hOldMap);
	DeleteObject(m_hBitMap);		// loadimage 불러온 오브젝트를 삭제하는 함수, 단 현재 dc에 의해 선택된 gdi오브젝트는 해제할 수가 없다.
	DeleteDC(m_hMemDC);				// 현재 사용되고 있는 객체를 함부로 삭제하도록 둘 수 없기 때문에 생긴 안전 장치 역할을 하는 규정

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



	//회전된 이미지의 영역을 구한다.
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

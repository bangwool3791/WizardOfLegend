#pragma once

#include "Include.h"

class CMyBmp
{
private :
public :
	HBITMAP		GetRotatedBitmap(HDC hdc, HBITMAP hBitmap, float radians, COLORREF clrBack);
public:
	HDC			Get_MemDC() { return m_hMemDC; }
	HDC			Get_RotDC() { return m_hRotMemDC; }
public:
	void		Load_Bmp(const TCHAR* pFilePath);
	void		Release(void);

private:
	HDC			m_hMemDC;
	HDC			m_hRotMemDC;
	HBITMAP		m_hBitMap;
	HBITMAP		m_hOldMap;
	HBITMAP		m_hRotOldBitMap;
	HBITMAP		m_hRotBitMap;


public:
	CMyBmp();
	~CMyBmp();
};


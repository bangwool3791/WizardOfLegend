#pragma once
class CChildEditer
{
private :
	const TCHAR* m_Path;
	RECT	m_tRect;
	HPEN	m_hPen;
	HPEN	m_hOldPen;
public :
	void Initialize();
	void Update();
	void Render();
	const POINT&	Get_Point()const
	{
		return m_tPoint;
	}
private :
	POINT m_tPoint;
	HDC m_hMemDC;
	HDC m_DC;
	HBITMAP m_hBitMap;
	HBITMAP	m_hOldMap;
	HWND m_ChildhWnd;
	void Process_Image();
public:
	CChildEditer();
	virtual ~CChildEditer();
};


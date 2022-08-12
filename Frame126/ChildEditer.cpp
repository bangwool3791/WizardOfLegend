#include "stdafx.h"
#include "ChildEditer.h"
#include "FileMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"

LRESULT CALLBACK WndChildProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

CChildEditer::CChildEditer()
{
	ZeroMemory(&m_tPoint, sizeof(POINT));
}

void CChildEditer::Initialize()
{
	m_Path = L"../Image/Edit/FireTileSet_1024.bmp";

	WNDCLASSA wnd_class = { 0 };
	wnd_class.lpfnWndProc = WndChildProc;
	wnd_class.hInstance = GetModuleHandle(NULL);
	wnd_class.lpszClassName = "actwnd";

	RegisterClassA(&wnd_class);
	CFileMgr::Get_Instance()->Add_FileInfo(m_Path, L"ChildEditor");
	m_ChildhWnd = CreateWindowA(wnd_class.lpszClassName, " ", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 
		CFileMgr::Get_Instance()->Get_Info(L"ChildEditor").width, CFileMgr::Get_Instance()->Get_Info(L"ChildEditor").height, NULL, 0, wnd_class.hInstance, NULL);

	MSG msg = { 0 };

	m_DC = GetDC(m_ChildhWnd);

	m_hMemDC = CreateCompatibleDC(m_DC);

	m_hBitMap = (HBITMAP)LoadImage(NULL,
		m_Path,
		IMAGE_BITMAP,
		0,
		0,
		LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_hOldMap = (HBITMAP)SelectObject(m_hMemDC, m_hBitMap);
	m_hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	m_hOldPen = (HPEN)::SelectObject(m_DC, m_hPen);

}
void CChildEditer::Process_Image()
{
}

void CChildEditer::Update()
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(m_ChildhWnd, &pt);

		if (0 <= pt.x && CFileMgr::Get_Instance()->Get_Info(L"ChildEditor").width >= pt.x
			&& 0 <= pt.y && CFileMgr::Get_Instance()->Get_Info(L"ChildEditor").height >= pt.y)

		{
			m_tRect.left	= CTILECX * (pt.x / CTILECX);
			m_tRect.top		=  CTILECY * (pt.y / CTILECY);
			m_tRect.right	= m_tRect.left + CTILECX;
			m_tRect.bottom	= m_tRect.top + CTILECY;
			memcpy_s(&m_tPoint, sizeof(POINT), &pt, sizeof(POINT));
		}
	}
}

void CChildEditer::Render()
{
	BitBlt(m_DC,
		0,
		0,
		CFileMgr::Get_Instance()->Get_Info(L"ChildEditor").width, CFileMgr::Get_Instance()->Get_Info(L"ChildEditor").height,
		m_hMemDC,
		0,
		0,
		SRCCOPY);
	
	MoveToEx(m_DC, m_tRect.left, m_tRect.top, nullptr);
	LineTo(m_DC, m_tRect.left, m_tRect.bottom);
	LineTo(m_DC, m_tRect.right, m_tRect.bottom);
	LineTo(m_DC, m_tRect.right, m_tRect.top);
	LineTo(m_DC, m_tRect.left, m_tRect.top);
}

CChildEditer::~CChildEditer()
{
	m_hPen = (HPEN)::SelectObject(m_DC, m_hOldPen);
	DeleteObject(m_hPen);
	SelectObject(m_hMemDC, m_hOldMap);
	DeleteObject(m_hBitMap);		
	DeleteDC(m_hMemDC);				
	ReleaseDC(m_ChildhWnd, m_DC);
	CFileMgr::Get_Instance()->Destroy_Instance();
	DestroyWindow(m_ChildhWnd);
}

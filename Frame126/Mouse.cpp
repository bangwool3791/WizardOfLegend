#include "stdafx.h"
#include "Mouse.h"
#include "ImageDefine.h"
#include "BmpMgr.h"
#include "UserHeader.h"

CMouse::CMouse()
{
}


CMouse::~CMouse()
{
	Release();
}

void CMouse::Initialize(void)
{
	m_tInfo.fCX = (float)FCX_UI_MOUSE;
	m_tInfo.fCY = (float)FCX_UI_MOUSE;

	m_pFrameKey = L"UI_MOUSE";
}

int CMouse::Update(void)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);	// ���콺 Ŀ���� ��ġ ��ǥ�� ������ �Լ�,
	ScreenToClient(g_hWnd, &ptMouse);	// ��ü ��ũ�� ������ ��ǥ�� �츮�� ������ Ŭ���̾�Ʈ ��ǥ�� ��ȯ
	float fX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fY = CScrollMgr::Get_Instance()->Get_ScrollY();
#if true
#ifdef _USER_DEBUG
	static DWORD dDebugTick = GetTickCount();
	if (dDebugTick + _DEBUG_ACITVE_TICK < GetTickCount())
	{
		dDebugTick = GetTickCount();
	}
#endif
#endif
	m_tInfo.fX = (float)ptMouse.x;
	m_tInfo.fY = (float)ptMouse.y;

	Update_Rect();
	ShowCursor(true);	// ���콺 Ŀ���� �׸� ������ �����ϴ� �Լ�

	return OBJ_NOEVENT;
}

void CMouse::Late_Update(void)
{
	
}

void CMouse::Render(HDC hDC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		(int)m_tRect.left,
		(int)m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));
}

void CMouse::Release(void)
{
	
}

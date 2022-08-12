#include "stdafx.h"
#include "BottomHole.h"
#include "UserHeader.h"

CBottomHole::CBottomHole()
{
}

CBottomHole::CBottomHole(float _fX, float _fY) 
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CBottomHole::~CBottomHole()
{
	Release();
}

void CBottomHole::Initialize(void)
{
	m_tInfo.fCX = FCX_UI_BOTTOM_HOLE;
	m_tInfo.fCY = FCY_UI_BOTTOM_HOLE;

	m_dTick = GetTickCount();

	Set_Frame(0, 0xFFFF, 0, 0);

	m_pFrameKey = L"BOTTOM_HOLE";

	m_eGroup = BACKGROUND;

	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	Update_Rect();
}

int	CBottomHole::Update(void)
{
	InCrease_X_Frame(m_tFrame);
	
	if (m_dTick + BOTTOM_HOLE_TICK < GetTickCount())
		return OBJ_DEAD;

	Update_Rect();
	return OBJ_NOEVENT;
}
void CBottomHole::Late_Update(void)
{
}
void CBottomHole::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		(int)(m_tRect.left + iScrollX),
		(int)(m_tRect.top + iScrollY),
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		m_hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));
}
void CBottomHole::Release(void)
{

}

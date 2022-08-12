#include "stdafx.h"
#include "IceCrystal.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
CIceCrystal::CIceCrystal():m_eState(ICE_STATE_END)
{
}

CIceCrystal::CIceCrystal(float _x, float _y, float _fAngle):m_eState(ICE_STATE_END)
{
	m_tInfo.fX = (float)_x;
	m_tInfo.fY = (float)_y;
	m_fAngle = _fAngle;
}
CIceCrystal::~CIceCrystal()
{
}

void CIceCrystal::Initialize(void)
{
	ZeroMemory(&m_tPoint, sizeof(POINT));

	m_eState = ICE_STATE_FRONT;

	m_tPoint.x = (LONG)m_tInfo.fX;
	m_tPoint.y = (LONG)m_tInfo.fY;

	m_pFrameKey = L"Skill_ICECRYSTAL";
	m_tInfo.fCX = 43.f;
	m_tInfo.fCY = 64.f;

	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameMotionEnd = 2;
	m_tFrame.iFrameEnd = 1;
	m_tFrame.dwFrameSpeed = 100;
	m_dTick = GetTickCount();
	m_RadiusIncreaseTick = GetTickCount();
	m_fRadius = 1.f;

	Update_Rect();
}
int CIceCrystal::Update(void)
{
	switch (m_eState)
	{
	case ICE_STATE_FRONT:
		if (m_RadiusIncreaseTick + 100 < GetTickCount())
		{
			++m_fRadius;
			m_RadiusIncreaseTick = GetTickCount();
		}
		m_tInfo.fX = m_tInfo.fX + m_fRadius * cosf(m_fAngle);
		m_tInfo.fY = m_tInfo.fY - m_fRadius * sinf(m_fAngle);
		if (m_dTick + 600 < GetTickCount())
		{
			m_eState = ICE_STATE_BACK;
			m_dTick = GetTickCount();
		}
		break;
	case ICE_STATE_BACK:
		if (m_RadiusIncreaseTick + 100 < GetTickCount())
		{
			--m_fRadius;
			m_RadiusIncreaseTick = GetTickCount();
		}
		m_tInfo.fX = m_tInfo.fX - m_fRadius * cosf(m_fAngle);
		m_tInfo.fY = m_tInfo.fY + m_fRadius * sinf(m_fAngle);
		if (m_dTick + 600 < GetTickCount())
		{
			return OBJ_DEAD;
		}
		break;
	}

	Update_Rect();

	if (InCrease_X_FrameWithY(m_tFrame))
	{
		switch (m_eState)
		{
		case ICE_STATE_FRONT:
			break;
		case ICE_STATE_BACK:
			break;
		}
	}

	return OBJ_NOEVENT;
}
void CIceCrystal::Late_Update(void)
{

}
void CIceCrystal::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	//BitBlt(hDC, m_tRect.left + iScrollX, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hMemDC, 0, 0, SRCCOPY);
	// 1인자 : 복사 받을 최종적으로 그림을 그릴 DC
	// 2, 3인자 : 복사 받을 위치의 좌표를 전달(L, T 좌표)
	// 4, 5인자 : 복사 받을 텍스처의 X, Y 사이즈
	// 6 인자 : 비트맵을 가지고 있는 DC
	// 7, 8인자 : 비트맵을 출력할 시작 좌표(L, T좌표)
	// 9인자 : 출력 효과를 설정 SRCCOPY : 그대로 출력
	GdiTransparentBlt(hDC,							// 1인자 : 복사 받을 최종적으로 그림을 그릴 DC
		(int)m_tRect.left,	// 2,3인자 : 2, 3인자 : 복사 받을 위치의 좌표를 전달(L, T 좌표)
		(int)m_tRect.top,
		(int)m_tInfo.fCX,				// 4,5 인자 : 복사 받을 텍스처의 X, Y 사이즈
		(int)m_tInfo.fCY,
		hMemDC,							// 6인자 : 비트맵을 가지고 있는 DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,			// 7, 8인자 : 비트맵을 출력할 시작 좌표
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,		// 9, 10인자 : 복사할 비트맵의 X,Y 사이즈	
		(int)m_tInfo.fCY,
		RGB(0, 0, 0));			// 11인자 : 제거하고자 하는 색상
}
void CIceCrystal::Release(void)
{

}

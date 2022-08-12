#include "stdafx.h"
#include "Thunder.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
CThunder::CThunder()
{
}

CThunder::CThunder(float _x, float _y, float _fAngle)
{
	m_tInfo.fX = (float)_x;
	m_tInfo.fY = (float)_y;
	m_fAngle = _fAngle;
}

CThunder::~CThunder()
{
}

void CThunder::Initialize(void)
{
	m_pFrameKey = L"Skill_THUNDER";
	m_tInfo.fCX = 74.f;
	m_tInfo.fCY = 403.f;

	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iFrameStart = 4;
	m_tFrame.iFrameEnd = 15;
	m_tFrame.iFrameMotionEnd = 1;
	m_tFrame.dwFrameSpeed = 25;

	m_fRadius = 10;

	Update_Rect();
}
int CThunder::Update(void)
{
	m_fRadius+= (float)0.01;

	m_tInfo.fX = m_tInfo.fX + m_fRadius * cosf(m_fAngle);
	m_tInfo.fY = m_tInfo.fY - m_fRadius * sinf(m_fAngle);

	Update_Rect();
	if (InCrease_X_FrameWithY(m_tFrame))
	{
		return OBJ_DEAD;
	}
	return OBJ_NOEVENT;
}
void CThunder::Late_Update(void)
{

}
void CThunder::Render(HDC hDC)
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
		(int)m_tRect.top - 190,
		(int)m_tInfo.fCX,				// 4,5 인자 : 복사 받을 텍스처의 X, Y 사이즈
		(int)m_tInfo.fCY,
		hMemDC,							// 6인자 : 비트맵을 가지고 있는 DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,			// 7, 8인자 : 비트맵을 출력할 시작 좌표
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,		// 9, 10인자 : 복사할 비트맵의 X,Y 사이즈	
		(int)m_tInfo.fCY,
		RGB(0, 0, 0));			// 11인자 : 제거하고자 하는 색상
}
void CThunder::Release(void)
{

}

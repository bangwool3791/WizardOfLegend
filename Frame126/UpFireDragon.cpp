#include "stdafx.h"
#include "UpFireDragon.h"
#include "FireParticle.h"
#include "UserHeader.h"

CUpFireDragon::CUpFireDragon()
{
}

CUpFireDragon::CUpFireDragon(float fX, float fY, float _fAngle)
{
	m_tPoint.x = (LONG)fX;
	m_tPoint.y = (LONG)fY;
	m_tInfo.fX = fX;
	m_tInfo.fY = fY;
	m_fAngle = _fAngle;
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
}

void CUpFireDragon::Initialize()
{
	m_pFrameKey = L"FIREDRAGON";
	Set_Frame(0, 25, 1, 11);

	m_tInfo.fCX = (float)FCX_FIREDRAGON;
	m_tInfo.fCY = (float)FCY_FIREDRAGON;

	m_fSpeed = 20.f;
	m_fRadius = 100.f;
	m_fDraonAngle = 0.f;
	Update_Rect();

	m_dTick = GetTickCount();
}

int CUpFireDragon::Update()
{
	if (InCrease_X_Frame(m_tFrame))
	{
	}

	float fX = Displacement(m_tInfo.fX, m_pPlayer->Get_Info().fX);
	float fY = Displacement(m_pPlayer->Get_Info().fY, m_tInfo.fY);
	float fR = Disatance(fX, fY);

	if (RANGE_FIRE_DRAGON < fR)
		return OBJ_DEAD;

	m_fDraonAngle += 0.15f;
	
	m_tPoint.x += (LONG)(m_fSpeed * cosf(m_fAngle));
	m_tPoint.y -= (LONG)(m_fSpeed * sinf(m_fAngle));

	m_tInfo.fX = m_tPoint.x + m_fRadius * cosf(m_fDraonAngle);
	m_tInfo.fY = m_tPoint.y - m_fRadius * sinf(m_fDraonAngle);
	Update_Rect();

	if (m_dTick + 50 < GetTickCount())
	{
		CObjMgr::Get_Instance()->AddObject(STAGE_KEY, OBJ_FIRE_PARTICLE, CAbstactFactory<CFireParticle>::Create(m_tInfo.fX, m_tInfo.fY));
		m_dTick = GetTickCount();
	}
	return OBJ_NOEVENT;
}

//void CUpFireDragon::Late_Update()
//{
//}
//
//void CUpFireDragon::Render(HDC hDC)
//{
//	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
//	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
//
//	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
//
//	//BitBlt(hDC, m_tRect.left + iScrollX, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hMemDC, 0, 0, SRCCOPY);
//	// 1인자 : 복사 받을 최종적으로 그림을 그릴 DC
//	// 2, 3인자 : 복사 받을 위치의 좌표를 전달(L, T 좌표)
//	// 4, 5인자 : 복사 받을 텍스처의 X, Y 사이즈
//	// 6 인자 : 비트맵을 가지고 있는 DC
//	// 7, 8인자 : 비트맵을 출력할 시작 좌표(L, T좌표)
//	// 9인자 : 출력 효과를 설정 SRCCOPY : 그대로 출력
//	GdiTransparentBlt(hDC,							// 1인자 : 복사 받을 최종적으로 그림을 그릴 DC
//		(int)(m_tRect.left + iScrollX),	// 2,3인자 : 2, 3인자 : 복사 받을 위치의 좌표를 전달(L, T 좌표)
//		(int)(m_tRect.top +  iScrollY),
//		(int)m_tInfo.fCX,				// 4,5 인자 : 복사 받을 텍스처의 X, Y 사이즈
//		(int)m_tInfo.fCY,
//		hMemDC,							// 6인자 : 비트맵을 가지고 있는 DC
//		m_tFrame.iFrameStart * (int)m_tInfo.fCX,			// 7, 8인자 : 비트맵을 출력할 시작 좌표
//		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
//		(int)m_tInfo.fCX,		// 9, 10인자 : 복사할 비트맵의 X,Y 사이즈	
//		(int)m_tInfo.fCY,
//		RGB(255, 0, 255));			// 11인자 : 제거하고자 하는 색상
//}
//void CUpFireDragon::Release()
//{
//
//}

CUpFireDragon::~CUpFireDragon()
{

}

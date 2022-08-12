#include "stdafx.h"
#include "FireParticle.h"
#include "UserHeader.h"

CFireParticle::CFireParticle()
{
}

CFireParticle::CFireParticle(float fX, float fY)
{
	m_tInfo.fX = fX;
	m_tInfo.fY = fY;
}

CFireParticle::~CFireParticle()
{
}

void CFireParticle::Initialize(void)
{
	Set_Frame(0, 2, 0, 5);
	m_tFrame.iFrameMotionEnd = 4;
	m_pFrameKey = L"FIRE_PARTICLE";
	m_tInfo.fCX = FCX_FIREPARTICLE;
	m_tInfo.fCY = FCY_FIREPARTICLE;
	Update_Rect();
	m_dTick = GetTickCount();
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
}
int CFireParticle::Update(void)
{
	if (InCrease_X_FrameWithY(m_tFrame))
	{
		return OBJ_DEAD;
	}
	Update_Rect();
	return OBJ_NOEVENT;
}
void CFireParticle::Late_Update(void)
{

}
void CFireParticle::Render(HDC hDC)
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

void CFireParticle::Release(void)
{

}
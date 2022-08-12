#include "stdafx.h"
#include "HitEffect.h"
#include "UserHeader.h"

CHitEffect::CHitEffect()
{
}

CHitEffect::CHitEffect(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CHitEffect::~CHitEffect()
{
	Release();
}

void CHitEffect::Initialize(void)
{
	m_tInfo.fCX = FCX_HIT_EFFECT;
	m_tInfo.fCY = FCY_HIT_EFFECT;

	m_pFrameKey = L"HITEFFECT";

	Set_Frame(0, 100, 3, 4);

	Update_Rect();
}

int	CHitEffect::Update(void)
{
	if (InCrease_X_Frame(m_tFrame))
		return OBJ_DEAD;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CHitEffect::Late_Update(void)
{

}
void CHitEffect::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,							
		(int)(m_tRect.left + iScrollX),	
		(int)(m_tRect.top + iScrollY),
		(int)m_tInfo.fCX,			
		(int)m_tInfo.fCY,
		hMemDC,							
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,			
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,	
		(int)m_tInfo.fCY,
		RGB(0xFF, 0xFA, 0xFF));			
}

void CHitEffect::Release(void)
{

}

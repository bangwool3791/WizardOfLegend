#include "stdafx.h"
#include "SoilEffect.h"
#include "UserHeader.h"

CSoilEffect::CSoilEffect()
{
}

CSoilEffect::CSoilEffect(float _fX, float _fY):m_fYFrame(0.f)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CSoilEffect::~CSoilEffect()
{
	Release();
}

void CSoilEffect::Initialize(void)
{
	m_tInfo.fCX = FCX_UI_SOIL_EFFECT;
	m_tInfo.fCY = FCY_UI_SOIL_EFFECT;

	m_dTick = GetTickCount();

	m_fYFrame = float(rand() % SOIL_MAX_YFRAME);

	Set_Frame(0, 200, (int)m_fYFrame, 3);

	m_pFrameKey = L"SOIL_EFFECT";

	m_eGroup = EFFECT;

	Update_Rect();
}

int	CSoilEffect::Update(void) 
{
	if (InCrease_X_Frame(m_tFrame))
		return OBJ_DEAD;

	m_tInfo.fY -= (float)0.6;
	Update_Rect();
	return OBJ_NOEVENT;
}
void CSoilEffect::Late_Update(void)
{

}
void CSoilEffect::Render(HDC hDC)
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
		RGB(255, 0, 255));
}
void CSoilEffect::Release(void)
{

}

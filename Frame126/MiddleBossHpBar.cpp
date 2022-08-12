#include "stdafx.h"
#include "MiddleBossHpBar.h"
#include "UserHeader.h"

CMiddleBossHpBar::CMiddleBossHpBar()
{
	ZeroMemory(&m_tHpInfo, sizeof(INFO));
	ZeroMemory(&m_tHpRect, sizeof(RECT));
}


CMiddleBossHpBar::~CMiddleBossHpBar()
{
	Release();
}

void CMiddleBossHpBar::Initialize()
{
	m_tInfo.fX = FX_BOSS_NAME_BAR;
	m_tInfo.fY = FY_BOSS_NAME_BAR;
	m_tInfo.fCX = FCX_BOSS_NAME_BAR;
	m_tInfo.fCY = FCY_BOSS_NAME_BAR;

	m_tHpInfo.fX = FX_BOSS_HP_BAR + FCX_BOSS_HP_BAR * 0.5;
	m_tHpInfo.fY = FY_BOSS_HP_BAR + FCY_BOSS_HP_BAR * 0.5;
	m_tHpInfo.fCX = FCX_BOSS_HP_BAR;
	m_tHpInfo.fCY = FCY_BOSS_HP_BAR;
	m_iHp = (int)m_tHpInfo.fCX;

	m_pFrameKey = L"MIDDLEBOSS_NAMEBAR";
	m_pHpKey = L"BOSS_HPBAR";

	m_hHpMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pHpKey);
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	Update_Rect();
	Update_HpMana_UI();
}

int CMiddleBossHpBar::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CMiddleBossHpBar::Late_Update()
{

}

void CMiddleBossHpBar::Render(HDC hDC)
{
	//HP
	GdiTransparentBlt(hDC,
		(int)m_tHpRect.left,
		(int)m_tHpRect.top,
		(int)m_iHp,
		(int)m_tHpInfo.fCY,
		m_hHpMemDC,
		m_tFrame.iFrameStart * (int)m_tHpInfo.fCX,
		m_tFrame.iFrameMotion * (int)m_tHpInfo.fCY,
		(int)m_tHpInfo.fCX,
		(int)m_tHpInfo.fCY,
		RGB(255, 0, 255));

	GdiTransparentBlt(hDC,
		(int)m_tRect.left,
		(int)m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		m_hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));
}

void CMiddleBossHpBar::Release()
{

}

void CMiddleBossHpBar::Update_HpMana_UI()
{
	m_tHpRect.left = long(m_tHpInfo.fX - (m_tHpInfo.fCX / 2.f));
	m_tHpRect.top = long(m_tHpInfo.fY - (m_tHpInfo.fCY / 2.f));
	m_tHpRect.right = long(m_tHpInfo.fX + (m_tHpInfo.fCX / 2.f));
	m_tHpRect.bottom = long(m_tHpInfo.fY + (m_tHpInfo.fCY / 2.f));
}


void CMiddleBossHpBar::Process_HpBar(int _iHp)
{
	if (0 > _iHp)
		return;

	m_iHp = (long)((float)_iHp / MIDDLE_BOSS_HP * m_tHpInfo.fCX);
}
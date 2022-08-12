#include "stdafx.h"
#include "BossHpBar.h"
#include "UserHeader.h"

CBossHpBar::CBossHpBar()
{
	ZeroMemory(&m_tHpInfo, sizeof(INFO));
	ZeroMemory(&m_tHpRect, sizeof(RECT));
}


CBossHpBar::~CBossHpBar()
{
	Release();
}

void CBossHpBar::Initialize()
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

	m_pFrameKey = L"BOSS_NAMEBAR";
	m_pHpKey = L"BOSS_HPBAR";

	Update_Rect();
	Update_HpMana_UI();
}

int CBossHpBar::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}

void CBossHpBar::Late_Update()
{

}

void CBossHpBar::Render(HDC hDC)
{
	HDC hHpMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pHpKey);
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	//HP
	GdiTransparentBlt(hDC,
		(int)m_tHpRect.left,
		(int)m_tHpRect.top,
		(int)m_iHp,
		(int)m_tHpInfo.fCY,
		hHpMemDC,
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
		hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));
}

void CBossHpBar::Release()
{

}

void CBossHpBar::Update_HpMana_UI()
{
	m_tHpRect.left = long(m_tHpInfo.fX - (m_tHpInfo.fCX / 2.f));
	m_tHpRect.top = long(m_tHpInfo.fY - (m_tHpInfo.fCY / 2.f));
	m_tHpRect.right = long(m_tHpInfo.fX + (m_tHpInfo.fCX / 2.f));
	m_tHpRect.bottom = long(m_tHpInfo.fY + (m_tHpInfo.fCY / 2.f));
}


void CBossHpBar::Process_HpBar(int _iHp)
{
	if (0 > _iHp || BOSS_HP < _iHp)
		return;

	m_iHp = (long)((float)_iHp / BOSS_HP * m_tHpInfo.fCX);
}
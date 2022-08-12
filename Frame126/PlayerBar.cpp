#include "stdafx.h"
#include "PlayerBar.h"
#include "UserHeader.h"

CPlayerBar::CPlayerBar()
{
	ZeroMemory(&m_tHpInfo, sizeof(INFO));
	ZeroMemory(&m_tManaInfo, sizeof(INFO));
	ZeroMemory(&m_tHpRect, sizeof(RECT));
	ZeroMemory(&m_tManaRect, sizeof(RECT));
}


CPlayerBar::~CPlayerBar()
{
	Release();
}

void CPlayerBar::Initialize()
{
	m_tInfo.fX = FX_UI_PLAYER_BAR;
	m_tInfo.fY = FY_UI_PLAYER_BAR;
	m_tInfo.fCX = FCX_UI_PLAYER_BAR;
	m_tInfo.fCY = FXY_UI_PLAYER_BAR;

	m_tHpInfo.fX = FX_UI_PLAYER_HP + FCX_UI_PLAYER_HP * 0.5;
	m_tHpInfo.fY = FY_UI_PLAYER_HP + FCY_UI_PLAYER_HP * 0.5;
	m_tHpInfo.fCX = FCX_UI_PLAYER_HP;
	m_tHpInfo.fCY = FCY_UI_PLAYER_HP;
	m_iHp = (int)m_tHpInfo.fCX;
	m_tManaInfo.fX = FX_UI_PLAYER_MANA + FCX_UI_PLAYER_MANA * 0.5;
	m_tManaInfo.fY = FY_UI_PLAYER_MANA + FCY_UI_PLAYER_MANA * 0.5;
	m_tManaInfo.fCX = FCX_UI_PLAYER_MANA;
	m_tManaInfo.fCY = FCY_UI_PLAYER_MANA;

	m_pFrameKey = L"UI_PLAYER_BAR";
	m_pHpKey = L"UI_PLAYER_HP_BAR";
	m_pManaKey = L"UI_PLAYER_MANA_BAR";
	Update_Rect();
	Update_HpMana_UI();
}

int CPlayerBar::Update()
{
	return OBJ_NOEVENT;
}

void CPlayerBar::Late_Update()
{

}

void CPlayerBar::Render(HDC hDC)
{
	HDC hHpMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pHpKey);
	HDC hManaMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pManaKey);
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
	//MANA
	GdiTransparentBlt(hDC,
		(int)m_tManaRect.left,
		(int)m_tManaRect.top,
		(int)m_tManaInfo.fCX,
		(int)m_tManaInfo.fCY,
		hManaMemDC,
		m_tFrame.iFrameStart * (int)m_tManaInfo.fCX,
		m_tFrame.iFrameMotion * (int)m_tManaInfo.fCY,
		(int)m_tManaInfo.fCX,
		(int)m_tManaInfo.fCY,
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

void CPlayerBar::Release()
{

}

void CPlayerBar::Update_HpMana_UI()
{
	m_tHpRect.left = long(m_tHpInfo.fX - (m_tHpInfo.fCX / 2.f));
	m_tHpRect.top = long(m_tHpInfo.fY - (m_tHpInfo.fCY / 2.f));
	m_tHpRect.right = long(m_tHpInfo.fX + (m_tHpInfo.fCX / 2.f));
	m_tHpRect.bottom = long(m_tHpInfo.fY + (m_tHpInfo.fCY / 2.f));

	m_tManaRect.left = long(m_tManaInfo.fX - (m_tManaInfo.fCX / 2.f));
	m_tManaRect.top = long(m_tManaInfo.fY - (m_tManaInfo.fCY / 2.f));
	m_tManaRect.right = long(m_tManaInfo.fX + (m_tManaInfo.fCX / 2.f));
	m_tManaRect.bottom = long(m_tManaInfo.fY + (m_tManaInfo.fCY / 2.f));
}


void CPlayerBar::Process_HpBar(int _iHp)
{
	if (0 >= _iHp || PLAYER_HP < _iHp)
		return;

	m_iHp = (long)((float)_iHp / PLAYER_HP * m_tHpInfo.fCX);
}
#include "stdafx.h"
#include "SoilColumn.h"
#include "UserHeader.h"
#include "SoilEffect.h"
#include "BottomHole.h"

CSoilColumn::CSoilColumn()
{

}

CSoilColumn::CSoilColumn(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CSoilColumn::~CSoilColumn()
{
	Release();
}

void CSoilColumn::Initialize()
{
	m_tInfo.fCX = FCX_UI_SOIL_COLUMN;
	m_tInfo.fCY = FCY_UI_SOIL_COLUMN;

	m_pPlayer = m_pObjMgr->Get_Player();
	Set_Frame(0, 700, 0, 2);

	m_pFrameKey = L"BOSS_SKILL";

	m_eGroup = EFFECT;

	Set_State(STATE_CREATE);
	m_pObjMgr->AddObject(UI_KEY, EFFECT_BOTTOM_HOLE, CAbstactFactory<CBottomHole>::Create(m_tInfo.fX, float(m_tInfo.fY + FCY_UI_SOIL_COLUMN * 0.5 - 15)));
	
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	Update_Rect();
}

int CSoilColumn::Update()
{
	if (InCrease_X_Frame(m_tFrame))
	{
		if (STATE_CREATE == Get_State())
		{
			Set_Frame(2, 1000, 0, 2);
			m_dTick = GetTickCount();
			Set_State(STATE_DEAD);
		}
	}

	switch (Get_State())
	{
	case STATE_DEAD:
		if (m_dTick + 3000 < GetTickCount())
		{
			m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_tInfo.fX, m_tInfo.fY - DISTANCE_SOIL_COLUM_EFFECT));
			m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_tInfo.fX, m_tInfo.fY - 0));
			m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_tInfo.fX, m_tInfo.fY + DISTANCE_SOIL_COLUM_EFFECT));
			m_dTick = GetTickCount();
			return OBJ_DEAD;
		}
		break;
	}
	return OBJ_NOEVENT;
}

void CSoilColumn::Late_Update()
{
	//플레이어와 충돌 처리 
	switch (Get_State())
	{
	case STATE_DEAD:
		CCollisionMgr::Collision_Sphere(m_pPlayer, this);
		break;
	}
}

void CSoilColumn::Render(HDC hDC)
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

void CSoilColumn::Release()
{

}

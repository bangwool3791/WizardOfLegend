#include "stdafx.h"
#include "BossSton.h"
#include "UserHeader.h"
#include "SoilEffect.h"
#include "BottomHole.h"

CBossSton::CBossSton()
{

}

CBossSton::CBossSton(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
	m_tPoint.fX = _fX;
	m_tPoint.fY = _fY;
}

CBossSton::~CBossSton()
{
	Release();
}

void CBossSton::Initialize()
{
	m_tInfo.fCX = FCX_UI_SOIL_BOX;
	m_tInfo.fCY = FCY_UI_SOIL_BOX;

	Set_Frame(0, 1000, 0, 0);

	m_pFrameKey = L"BOSS_BOXATTACK";

	Set_State(STATE_CREATE);
	m_pObjMgr->AddObject(UI_KEY, EFFECT_BOTTOM_HOLE, CAbstactFactory<CBottomHole>::Create(m_tInfo.fX, float(m_tInfo.fY + FCY_UI_SOIL_COLUMN * 0.5)));
	m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_tInfo.fX - FCX_UI_SOIL_BOX , m_tInfo.fY + DISTANCE_SOIL_BOX_EFFECT));
	m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_tInfo.fX, m_tInfo.fY + DISTANCE_SOIL_BOX_EFFECT));
	m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_tInfo.fX + FCX_UI_SOIL_BOX, m_tInfo.fY + DISTANCE_SOIL_BOX_EFFECT));
	m_dTick = GetTickCount();

	m_pPlayer = m_pObjMgr->Get_Player();

	m_fSpeed = 30.f;

	m_eGroup = RENDER_SKILL;

	Update_Rect();
}

int CBossSton::Update()
{
	InCrease_X_Frame(m_tFrame);

	float fR = 0.f, fAngle = 0.f;

	switch (Get_State())
	{
	case STATE_CREATE:
		if (m_dTick + BOSS_STON_TICK > GetTickCount())
		{
			m_tInfo.fY -= 0.8f;
		}
		else
		{
			Set_State(STATE_READY);
		}
		break;
	case STATE_READY:
		break;
	case STATE_ATTACK:
		m_tInfo.fX += m_fSpeed * cosf(m_fAngle);
		m_tInfo.fY -= m_fSpeed * sinf(m_fAngle);
		break;
	case STATE_DEAD:
		return OBJ_DEAD;
	}
	Update_Rect();
	return OBJ_NOEVENT;
}

void CBossSton::Late_Update()
{
	float fR = 0.f, fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);
	if (DISTANCE_SOIL_BOX < fR)
		Set_State(STATE_DEAD);
	//플레이어와 충돌 처리 
	switch (m_eState)
	{
	case STATE_ATTACK:
		Collision_Sphere(this, m_pPlayer);
		Collision_Sphere(this, m_pObjMgr->Get_TargetList(SKILL_KEY, SKILL_GAIA_ARMOR));
		break;
	}
	
}

void CBossSton::Render(HDC hDC)
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

void CBossSton::Release()
{

}

void CBossSton::Get_PlayerInfo(float * _fR, float* _fAngle)
{
	float fX = Displacement<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX);
	float fY = Displacement<float>(m_pPlayer->Get_Info().fY, m_tInfo.fY);
	*_fR = Distance<float>(fX, fY);
	*_fAngle = Angle<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX, m_tInfo.fY, m_pPlayer->Get_Info().fY);
	*_fAngle = *_fAngle * 180 / PI;
	*_fAngle = (float)((int)*_fAngle % 360);
}

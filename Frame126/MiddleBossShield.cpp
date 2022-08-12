#include "stdafx.h"
#include "MiddleBossShield.h"
#include "Player.h"
#include "UserHeader.h"

CMiddleBossShield::CMiddleBossShield() :m_eState(SHEILD_STATE_END)
{
}

CMiddleBossShield::CMiddleBossShield(float _x, float _y, float _fAngle) : m_eState(SHEILD_STATE_END), m_fDistance (50.f)
{
	ZeroMemory(&m_tCenter, sizeof(INFO));
	m_fAngle = _fAngle;
	m_tInfo.fX = _x + m_fDistance * cosf(m_fAngle);
	m_tInfo.fY = _y - m_fDistance * sinf(m_fAngle);
	m_tCenter.fX = _x;
	m_tCenter.fY = _y;
	Update_Rect();
}

CMiddleBossShield::~CMiddleBossShield()
{
	Release();
}

void CMiddleBossShield::Initialize(void)
{
	m_pFrameKey = L"GRAND_CRYSTAL";

	m_eState = SHEILD_STATE_CREATE;

	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	m_dTick = GetTickCount();

	m_tInfo.fCX = (float)FCX_GRANDSHIELD;
	m_tInfo.fCY = (float)FCY_GRANDSHIELD;
	m_eGroup = RENDER_SKILL;
	m_fSpeed = 0;

	Set_Frame(0, 100, 0, 3);

	m_pListMonster = m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN);
}

int CMiddleBossShield::Update(void)
{
	switch (m_eState)
	{
	case SHEILD_STATE_CREATE:
	case SHEILD_STATE_HOLD:
		if (100 > m_fDistance)
			++m_fDistance;
		if (0.15 > m_fSpeed)
			m_fSpeed += (float)0.0005;
		m_fAngle += m_fSpeed;
		break;
	case SHEILD_STATE_DESTROY:
		if (0 < m_fSpeed)
			m_fSpeed -= (float)0.1;
		if (0 < m_fDistance)
			--m_fDistance;
		else
			return OBJ_DEAD;

		m_fAngle -= m_fSpeed;
		break;
	}

	if (m_dTick + 5000 < GetTickCount())
	{
		switch (m_eState)
		{
		case SHEILD_STATE_CREATE:
			m_eState = SHEILD_STATE_HOLD;
			break;
		case SHEILD_STATE_HOLD:
			m_eState = SHEILD_STATE_DESTROY;
			break;
		case SHEILD_STATE_DESTROY:
			return OBJ_DEAD;
		}
		m_dTick = GetTickCount();
	}

	m_tInfo.fX =  m_tCenter.fX + m_fDistance * cosf(m_fAngle);
	m_tInfo.fY = m_tCenter.fY  - m_fDistance * sinf(m_fAngle);
	InCrease_X_Frame(m_tFrame);
	Update_Rect();

	return OBJ_NOEVENT;
}

void CMiddleBossShield::Late_Update(void)
{
	//for (int i = SWORDMAN; i < MONSTER_END; ++i)
	//{
	//	//CCollisionMgr::Collision_Sphere(this, &m_pListMonster[i]);
	//}
}

void CMiddleBossShield::Render(HDC hDC)
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
		RGB(0x37, 0x19, 0x05));		
}

void CMiddleBossShield::Release(void)
{

}

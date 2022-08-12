#include "stdafx.h"
#include "DownFireDragon.h"
#include "FireParticle.h"
#include "UserHeader.h"

CDownFireDragon::CDownFireDragon()
{
}

CDownFireDragon::CDownFireDragon(float fX, float fY, float _fAngle) 
{
	ZeroMemory(&m_tPoint, sizeof(POINT));
	m_tPoint.x = (LONG)fX;
	m_tPoint.y = (LONG)fY;
	m_tInfo.fX = fX;
	m_tInfo.fY = fY;
	m_fAngle = _fAngle;
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
}

void CDownFireDragon::Initialize()
{
	m_pFrameKey = L"FIREDRAGON";
	Set_Frame(0, 50, 1, 1);

	m_tInfo.fCX = (float)FCX_FIREDRAGON;
	m_tInfo.fCY = (float)FCY_FIREDRAGON;

	m_fSpeed = 20.f;
	m_fRadius = 100.f;
	m_fDraonAngle = 0.f;
	Update_Rect();

	m_dTick = GetTickCount();
}

int CDownFireDragon::Update()
{
	float fX = Displacement(m_tInfo.fX, m_pPlayer->Get_Info().fX);
	float fY = Displacement(m_pPlayer->Get_Info().fY, m_tInfo.fY);
	float fR = Disatance(fX, fY);

	if (RANGE_FIRE_DRAGON * 2 < fR)
		return OBJ_DEAD;

	m_fDraonAngle += 6;

	m_tPoint.x += (LONG)(m_fSpeed * cosf(m_fAngle));
	m_tPoint.y -= (LONG)(m_fSpeed * sinf(m_fAngle));

	m_tInfo.fX = m_tPoint.x + m_fRadius * cosf(m_fDraonAngle * PI / 180);
	m_tInfo.fY = m_tPoint.y - m_fRadius * sinf(m_fDraonAngle * PI / 180);
	Update_Rect();

	if (m_dTick + 50 < GetTickCount())
	{
		CObjMgr::Get_Instance()->AddObject(STAGE_KEY, OBJ_FIRE_PARTICLE, CAbstactFactory<CFireParticle>::Create(m_tInfo.fX, m_tInfo.fY));
		m_dTick = GetTickCount();
	}
	return OBJ_NOEVENT;
}

CDownFireDragon::~CDownFireDragon()
{

}

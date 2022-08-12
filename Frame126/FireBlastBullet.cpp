#include "stdafx.h"
#include "FireBlastBullet.h"
#include "UserHeader.h"
#include "FireBlast.h"

CFireBlastBullet::CFireBlastBullet()
{
}

CFireBlastBullet::CFireBlastBullet(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CFireBlastBullet::~CFireBlastBullet()
{
	Release();
}

void CFireBlastBullet::Initialize()
{
	m_fSpeed = 2.f;
	m_pPlayer = m_pObjMgr->Get_Player();
	m_dAttackTick = GetTickCount();
	m_dTick = GetTickCount();
}

int	 CFireBlastBullet::Update() 
{
	if (m_bDead)
		return OBJ_DEAD;

	float fR = 0.f;
	float fAngle = 0.f;

	Get_PlayerInfo(&fR, &fAngle);

	m_tInfo.fX += m_fSpeed * cosf(fAngle * PI / 180);
	m_tInfo.fY -= +m_fSpeed * sinf(fAngle * PI / 180);

	if (m_dTick + CREATE_FIRE_BLAST_TICK < GetTickCount())
	{
		m_listFireBlast.push_back(CAbstactFactory<CFireBlast>::Create(m_tInfo.fX, m_tInfo.fY));
		m_dTick = GetTickCount();
	}

	list<CObj*>::iterator iter = m_listFireBlast.begin();

	for (; iter != m_listFireBlast.end();)
	{
		if (OBJ_DEAD == (*iter)->Update())
		{
			Safe_Delete(*iter);
			iter = m_listFireBlast.erase(iter);
		}
		else
			++iter;
	}

	return OBJ_NOEVENT;
}

void CFireBlastBullet::Late_Update()
{
	if (m_dAttackTick + FIRE_BLAST_BULLET_TICK < GetTickCount())
		Set_Dead();

	for (auto& elem : m_listFireBlast)
		elem->Late_Update();
}
void CFireBlastBullet::Render(HDC hDC)
{
	for (auto& elem : m_listFireBlast)
		elem->Render(hDC);
}
void CFireBlastBullet::Release()
{
	for (auto& elem : m_listFireBlast)
	{
		if (m_listFireBlast.empty())
			break;

		elem->Release();
	}		
}

void CFireBlastBullet::Get_PlayerInfo(float * _fR, float* _fAngle)
{
	float fX = Displacement<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX);
	float fY = Displacement<float>(m_pPlayer->Get_Info().fY, m_tInfo.fY);
	*_fR = Distance<float>(fX, fY);
	*_fAngle = Angle<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX, m_tInfo.fY, m_pPlayer->Get_Info().fY);
	*_fAngle = *_fAngle * 180 / PI;
	*_fAngle = (float)((int)*_fAngle % 360);
}
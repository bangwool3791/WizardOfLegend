#include "stdafx.h"
#include "MiddleBossSton.h"
#include "UserHeader.h"

CMiddleBossSton::CMiddleBossSton()
{
}

CMiddleBossSton::CMiddleBossSton(float _fX, float _fY, float _fAngle)
{
	ZeroMemory(&m_tMonsterInfo, sizeof(INFO));
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
	m_tMonsterInfo.fX = _fX;
	m_tMonsterInfo.fY = _fY;
	m_fAngle = _fAngle;
}


CMiddleBossSton::~CMiddleBossSton()
{
	Release();
}

void CMiddleBossSton::Initialize()
{
	m_tInfo.fCX = FCX_STATUE;
	m_tInfo.fCY = FCY_STATUE;

	m_fSpeed = 15.f;

	m_eState = STATE_CREATE;
	m_eGroup = RENDER_SKILL;
	m_pFrameKey = L"STATUE";
	m_pPlayer = m_pObjMgr->Get_Player();
	Set_Frame(0, 1000, 0, 0);

	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	Update_Rect();
}

int CMiddleBossSton::Update()
{
	InCrease_X_Frame(m_tFrame);

	if (m_bDead)
		return OBJ_DEAD;

	switch (m_eState)
	{
	case STATE_ATTACK:
		m_tInfo.fX += m_fSpeed*cosf(m_fAngle);
		m_tInfo.fY -= m_fSpeed*sinf(m_fAngle);
		break;
	}

	Update_Rect();

	return OBJ_NOEVENT;
}

void CMiddleBossSton::Late_Update()
{
	//느려지는 문제 발생할 수도
	Collision_Sphere(this, m_pPlayer);
	Collision_Sphere(this, m_pObjMgr->Get_TargetList(SKILL_KEY, SKILL_GAIA_ARMOR));
	float fX = Displacement<float>(m_tInfo.fX, m_tMonsterInfo.fX);
	float fY = Displacement<float>(m_tMonsterInfo.fY, m_tInfo.fY);
	float fR = Distance<float>(fX, fY);

	if (MIDDLE_BOSS_STATUE_DISTNACE < fR)
		Set_Dead();
}

void CMiddleBossSton::Render(HDC hDC)
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

void CMiddleBossSton::Release()
{

}

void CMiddleBossSton::Collision_Sphere(CObj* _Dest, list<CObj*>* Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;
	if (0 >= Sour->size())
		return;
	for (auto& _Sour : *Sour)
	{
		float fX = Displacement<float>(m_tInfo.fX, _Sour->Get_Info().fX);
		float fY = Displacement<float>(_Sour->Get_Info().fY, m_tInfo.fY);
		float fR = Distance<float>(fX, fY);

		if (100 <= fR)
			continue;
		if (CCollisionMgr::Check_Sphere(_Sour, _Dest, &fR, &fAngle))
		{
			this->Set_Dead();
		}
	}
}
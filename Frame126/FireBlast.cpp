#include "stdafx.h"
#include "FireBlast.h"
#include "UserHeader.h"

CFireBlast::CFireBlast()
{
}

CFireBlast::CFireBlast(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CFireBlast::~CFireBlast()
{
	Release();
}

void CFireBlast::Initialize()
{
	m_tInfo.fCX = FCX_FIRE_BLAST;
	m_tInfo.fCY = FCY_FIRE_BLAST;

	m_pFrameKey = L"FIRE_BLAST";
	
	Set_Frame(0, 100, 0, 7);
	m_eGroup = RENDER_SKILL;
	m_pPlayer = m_pObjMgr->Get_Player();
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	Update_Rect();
}

int CFireBlast::Update()
{
	if (InCrease_X_Frame(m_tFrame))
		return OBJ_DEAD;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CFireBlast::Late_Update()
{
	Collision_Sphere(this, m_pPlayer);
	Collision_Sphere(this, m_pObjMgr->Get_TargetList(SKILL_KEY, SKILL_GAIA_ARMOR));
}

void CFireBlast::Render(HDC hDC)
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

void CFireBlast::Release()
{

}

void CFireBlast::Collision_Sphere(CObj* _Dest, list<CObj*>* Sour)
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




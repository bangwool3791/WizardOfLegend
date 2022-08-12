#include "stdafx.h"
#include "ColumnSkill.h"
#include "SoilColumn.h"
#include "UserHeader.h"

CColumnSkill::CColumnSkill()
{
}

CColumnSkill::CColumnSkill(float _fX, float _fY, float _fAngle) :m_fDistance(0.f)
{
	ZeroMemory(&m_tPoint, sizeof(INFO));
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
	m_fAngle = _fAngle;
	m_tPoint.fX = _fX;
	m_tPoint.fY = _fY;
}

CColumnSkill::~CColumnSkill()
{
	Release();
}

void CColumnSkill::Initialize()
{
	//미사일의 사이즈는 기둥의 면적과 동일

	m_tInfo.fCX = FCX_UI_SOIL_COLUMN;
	m_tInfo.fCY = FCY_UI_SOIL_COLUMN;

	//m_fSpeed = 40 / 1.414 + 10.f;
	m_fSpeed = float(40 / 1.414 + 20.f);
	m_pPlayer = m_pObjMgr->Get_Player();
	m_dAttackTick = GetTickCount();
	Update_Rect();
}

int CColumnSkill::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_dAttackTick + 30 < GetTickCount())
	{
		m_pObjMgr->AddObject(SKILL_KEY, SOIL_COLUMN, CAbstactFactory<CSoilColumn>::Create(m_tInfo.fX, m_tInfo.fY));
		m_tInfo.fX += m_fSpeed * cosf(m_fAngle * PI / 180);
		m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * PI / 180);
		m_dAttackTick = GetTickCount();
	}


	//if (0 == ((int)fR % (int)(FCX_UI_SOIL_COLUMN * 0.5)))
	return OBJ_NOEVENT;
}

void CColumnSkill::Late_Update()
{
	//플레이어와 충돌 처리 
	Collision_Sphere(this, m_pPlayer);
	Collision_Sphere(this, m_pObjMgr->Get_TargetList(SKILL_KEY, SKILL_GAIA_ARMOR));
	float fX = Displacement<float>(m_tPoint.fX, m_tInfo.fX);
	float fY = Displacement<float>(m_tInfo.fY, m_tPoint.fY);
	float fR = Distance<float>(fX, fY);

	if (fR > DISTANCE_COLUM_SKILL)
		Set_Dead();
}

void CColumnSkill::Render(HDC hDC)
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

void CColumnSkill::Release()
{

}

void CColumnSkill::Collision_Sphere(CObj* _Dest, list<CObj*>* Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;

	for (auto& _Sour : *Sour)
	{
		if (0 >= Sour->size())
			break;

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

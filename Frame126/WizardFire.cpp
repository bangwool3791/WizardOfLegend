#include "stdafx.h"
#include "WizardFire.h"
#include "UserHeader.h"

CWizardFire::CWizardFire()
{
}

CWizardFire::CWizardFire(float _fX, float _fY, float _fAngle) 
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
	m_fAngle = _fAngle;
}


CWizardFire::~CWizardFire()
{
	Release();
}

void CWizardFire::Initialize()
{
	m_tInfo.fCX = FCX_WIZARD_FIRE;
	m_tInfo.fCY = FCY_WIZARD_FIRE;

	m_fSpeed = 5.f;

	m_eState = STATE_CREATE;

	m_pFrameKey = L"WIZARD_FIRE";
	m_pPlayer = m_pObjMgr->Get_Player();
	m_eGroup = RENDER_SKILL;
	m_dAttackTick = GetTickCount();

	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	Update_Rect();
}

int CWizardFire::Update()
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

void CWizardFire::Late_Update()
{
	//느려지는 문제 발생할 수도
	Collision_Sphere(this, m_pPlayer);
	Collision_Sphere(this, m_pObjMgr->Get_TargetList(SKILL_KEY, SKILL_GAIA_ARMOR));
	float fX = Displacement<float>(m_tInfo.fX, m_pMonster->Get_Info().fX);
	float fY = Displacement<float>(m_pMonster->Get_Info().fY, m_tInfo.fY);
	float fR = Distance<float>(fX, fY);

	if (WIZARD_FIRE_DISTNACE < fR)
		Set_Dead();
}

void CWizardFire::Render(HDC hDC)
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
		RGB(0x6E, 0x46, 0xD2));
}

void CWizardFire::Release()
{

}

void CWizardFire::Collision_Sphere(CObj* _Dest, CObj* _Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;

	if (CCollisionMgr::Check_Sphere(_Sour, _Dest, &fR, &fAngle))
	{
		if (
			CObj::STATE_CREATE != _Sour->Get_State() &&
			CObj::STATE_STUN != _Sour->Get_State() &&
			CObj::STATE_DEAD != _Sour->Get_State() &&
			CObj::STATE_SKILL != _Sour->Get_State() &&
			CObj::STATE_CREATE != _Dest->Get_State())
		{
			_Sour->Set_State(CObj::STATE_STUN);
			_Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
			_Sour->Plus_Hp(-_Dest->Get_Attack());

			fAngle = fAngle * 180 / PI;
			fAngle = (float)((int)fAngle % 360);
			CSoundMgr::Get_Instance()->Set_Sound(L"PLAYER_HITED_1.mp3", SOUND_PLAYER_BACK, PLAYER_VOLUME_BACK);
		}
	}
}

void CWizardFire::Collision_Sphere(CObj* _Dest, list<CObj*>* Sour)
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
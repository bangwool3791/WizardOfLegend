#include "stdafx.h"
#include "Slash.h"
#include "UserHeader.h"

CSlash::CSlash()
{
}

CSlash::CSlash(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}


CSlash::~CSlash()
{
	Release();
}


void CSlash::Initialize()
{
	m_tInfo.fCX = FCX_UI_SWORDMAN_SLASH;
	m_tInfo.fCY = FCY_UI_SWORDMAN_SLASH;

	m_pFrameKey = L"SWORDMAN_ATTACK";

	m_pPlayer = m_pObjMgr->Get_Player();
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	m_eGroup = RENDER_SKILL;
	Update_Rect();
}

int CSlash::Update()
{
	if (InCrease_X_Frame(m_tFrame))
		return OBJ_DEAD;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CSlash::Late_Update()
{
	CSoundMgr::Get_Instance()->Set_Sound(L"PLAYER_HITED_1.mp3", SOUND_PLAYER, PLAYER_VOLUME);
	Collision_Sphere(this, m_pPlayer);
	Collision_Sphere(this, m_pObjMgr->Get_TargetList(SKILL_KEY, SKILL_GAIA_ARMOR));
}

void CSlash::Render(HDC hDC)
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

void CSlash::Release()
{

}


void CSlash::Collision_Sphere(CObj* _Dest, CObj* _Sour)
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


void CSlash::Collision_Sphere(CObj* _Dest, list<CObj*>* Sour)
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
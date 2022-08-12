#pragma once
#include "Obj.h"
#include "UserHeader.h"

class CIceBlastSkill :
	public CObj
{
private:
	float  m_fRadius = 0.f;
private:
	INFO	m_tPoint;
	float	m_fDistance;
public:
	CIceBlastSkill();
	CIceBlastSkill(float _fX, float _fY, float _fAngle);
	virtual ~CIceBlastSkill();
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private:
	void Collision_Sphere(CObj* _Dest, CObj* _Sour)
	{
		float fR = 0.f;
		float fAngle = 0.f;

		if (CCollisionMgr::Check_Sphere(_Sour, _Dest, &fR, &fAngle))
		{
			if (
				CObj::STATE_CREATE != _Sour->Get_State() &&
				CObj::STATE_STUN != _Sour->Get_State() &&
				CObj::STATE_DEAD != _Sour->Get_State() &&
				CObj::STATE_CREATE != _Dest->Get_State())
			{
				_Sour->Set_State(CObj::STATE_STUN);
				_Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
				_Sour->Plus_Hp(-_Dest->Get_Attack());

				CSoundMgr::Get_Instance()->Set_Sound(L"PLAYER_HITED_1.mp3", SOUND_PLAYER_BACK, PLAYER_VOLUME_BACK);
			}
		}
	}
	void Collision_Sphere(CObj* _Dest, list<CObj*>* Sour);
};


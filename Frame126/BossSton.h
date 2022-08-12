#pragma once
#include "Obj.h"
#include "UserHeader.h"
class CBossSton :
	public CObj
{
private:
	DWORD m_dTick;
	INFO  m_tPoint;
private :
	void Get_PlayerInfo(float * _fR, float* _fAngle);
public:
	CBossSton();
	CBossSton(float _fX, float _fY);
	virtual ~CBossSton();
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

				_Dest->Set_Dead();

				CSoundMgr::Get_Instance()->Set_Sound(L"PLAYER_HITED_1.mp3", SOUND_PLAYER_BACK, PLAYER_VOLUME_BACK);
			}
		}
	}
	void Collision_Sphere(CObj* _Dest, list<CObj*>* Sour)
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
};


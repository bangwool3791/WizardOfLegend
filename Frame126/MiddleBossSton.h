#pragma once
#include "Obj.h"
#include "UserHeader.h"

class CMiddleBossSton :
	public CObj
{
private :
	HDC m_hMemDC;
private:
	INFO m_tMonsterInfo;
public:
	CMiddleBossSton();
	CMiddleBossSton(float _fX, float _fY, float _fAngle);
	virtual ~CMiddleBossSton();
public:
	void Initialize();
	int	 Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();
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
				CObj::STATE_SKILL != _Sour->Get_State() &&
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
	void Collision_Sphere(CObj* _Dest, list<CObj*>* Sour);
};



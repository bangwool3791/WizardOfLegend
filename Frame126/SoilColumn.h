#pragma once
#include "Obj.h"
#include "CollisionMgr.h"
class CSoilColumn :
	public CObj
{

private :
	DWORD m_dTick;
public:
	CSoilColumn();
	CSoilColumn(float _fX, float _fY);
	virtual ~CSoilColumn();
	HDC m_hMemDC;
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private :
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
				_Sour->Set_PosX(fR * cosf(fAngle));
				_Sour->Set_PosY(-fR * sinf(fAngle));
			}
		}
	}
};


#pragma once
#include "Obj.h"
#include "CollisionMgr.h"
class CSlash :
	public CObj
{
private:
	HDC m_hMemDC;
public:
	CSlash();
	CSlash(float _fX, float _fY);
	virtual ~CSlash();
public:
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private:
	void Collision_Sphere(CObj* _Dest, CObj* _Sour);
	void Collision_Sphere(CObj* _Dest, list<CObj*>* Sour);
};

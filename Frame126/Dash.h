#pragma once
#include "Obj.h"
#include "HitEffect.h"
#include "AbstactFactory.h"
#include "CollisionMgr.h"

class CDash :
	public CObj
{
public:
	CDash();
	CDash(float _fX, float _fY);
	virtual ~CDash();
public:
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private :
	list<CObj*>*		m_pListMonster;
	HDC					m_hMemDC;
	void Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour);
	void CDash::Collision_Sphere(CObj* _Dest, CObj* Sour);
private :

};


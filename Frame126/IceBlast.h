#pragma once
#include "Obj.h"
class CIceBlast :
	public CObj
{
private :
	HDC m_hMemDC;
public:
	CIceBlast();
	CIceBlast(float _fX, float _fY);
	CIceBlast(float _fX, float _fY, float _fAngle);
	virtual ~CIceBlast();
public :
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private :
	void Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour);
	void Collision_Sphere(CObj* _Dest, CObj* Sour);
	list<CObj*>*		m_pListMonster;
};


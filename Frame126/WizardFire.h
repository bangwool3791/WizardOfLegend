#pragma once
#include "Obj.h"

class CWizardFire :
	public CObj
{
private :
	HDC m_hMemDC;
public:
	void Set_Monster(CObj* _pMonster) { m_pMonster = _pMonster; }
private:
	CObj* m_pMonster;
public:
	CWizardFire();
	CWizardFire(float _fX, float _fY, float _fAngle);
	virtual ~CWizardFire();
public:
	void Initialize();
	int	 Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();
private :
	void CWizardFire::Collision_Sphere(CObj* _Dest, CObj* _Sour);
	void Collision_Sphere(CObj* _Dest, list<CObj*>* Sour);
};


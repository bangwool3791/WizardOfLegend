#pragma once
#include "Obj.h"
class CGaiaArmor : public CObj
{
	DWORD m_dIntervalState = 0;
private:
	DWORD m_dTick;
public:
	enum SHEILD_STATE { SHEILD_STATE_CREATE, SHEILD_STATE_HOLD, SHEILD_STATE_DESTROY, SHEILD_STATE_END };
private:
	SHEILD_STATE m_eState;
private:
	HDC m_hMemDC;
	float m_fDistance = 150.f;
public:
	CGaiaArmor();
	CGaiaArmor(float _x, float _y, float _fAngle);
	virtual ~CGaiaArmor();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:
	void Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour);
	void Collision_Sphere(CObj* _Dest, CObj* Sour);
};


#pragma once
#include "Obj.h"
class CMiddleBossShield : public CObj
{
private:
	DWORD m_dTick;
public:
	enum SHEILD_STATE { SHEILD_STATE_CREATE, SHEILD_STATE_HOLD, SHEILD_STATE_DESTROY, SHEILD_STATE_END };
private:
	SHEILD_STATE m_eState;
public:
	CMiddleBossShield();
	CMiddleBossShield(float _x, float _y, float _fAngle);
	virtual ~CMiddleBossShield();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:
	float		m_fDistance = 0.f;
private:
	list<CObj*>*		m_pListMonster;
	//중점 포인트
private:
	INFO m_tCenter;

};


#pragma once
#include "Obj.h"
class CIceCrystal :
	public CObj
{
public :
	enum ICECRYSTAL_STATE{ICE_STATE_FRONT, ICE_STATE_BACK, ICE_STATE_END};
	ICECRYSTAL_STATE m_eState;
private:
	float m_fRadius;
	POINT m_tPoint;
	DWORD m_dTick;
	DWORD m_RadiusIncreaseTick;
public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:
	CIceCrystal();
	CIceCrystal(float _x, float _y, float _fAngle);
	virtual ~CIceCrystal();
};


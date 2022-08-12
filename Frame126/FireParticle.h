#pragma once
#include "Obj.h"
class CFireParticle :
	public CObj
{
private :
	HDC m_hMemDC;
private :
	DWORD m_dTick;
public:
	CFireParticle();
	CFireParticle(float fX, float fY);
	virtual ~CFireParticle();
public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
};


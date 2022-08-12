#pragma once
#include "Obj.h"
class CMiddleShadow :
	public CObj
{
private:
	HDC m_hMemDC;
public:
	CMiddleShadow();
	CMiddleShadow(float _fX, float _fY);
	virtual ~CMiddleShadow();
public:
	virtual void Initialize(void) override;
	virtual int	 Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

};


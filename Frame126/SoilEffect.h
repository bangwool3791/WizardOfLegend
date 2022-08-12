#pragma once
#include "Obj.h"
class CSoilEffect :
	public CObj
{
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
public:
	CSoilEffect();
	CSoilEffect(float _fX, float _fY);
	virtual ~CSoilEffect();
private :
	DWORD m_dTick;
	float m_fYFrame;
};


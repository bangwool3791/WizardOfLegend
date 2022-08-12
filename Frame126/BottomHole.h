#pragma once
#include "Obj.h"
class CBottomHole :
	public CObj
{
private :
	HDC m_hMemDC;
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
public:
	CBottomHole();
	CBottomHole(float _fX, float _fY);
	virtual ~CBottomHole();
private:
	DWORD m_dTick;
};


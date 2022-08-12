#pragma once
#include "Obj.h"
class CHitEffect :
	public CObj
{
public:
	CHitEffect();
	CHitEffect(float _fX, float _fY);
	virtual ~CHitEffect();
public:
	void		Initialize(void);
	int			Update(void);
	void		Late_Update(void);
	void		Render(HDC hDC);
	void		Release(void);
	//충돌처리
};

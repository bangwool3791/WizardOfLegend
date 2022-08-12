#pragma once
#include "Obj.h"
class CSummon :
	public CObj
{
public :
	void		Initialize(void);
	int			Update(void);
	void		Late_Update(void);
	void		Render(HDC hDC);
	void		Release(void);
public:
	CSummon();
	CSummon(float _x, float _y);
	virtual ~CSummon();
};


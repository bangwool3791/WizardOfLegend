#pragma once
#include "Obj.h"
class CGameFont :
	public CObj
{
public :
	void Set_Font(TCHAR* _prim);
public:
	CGameFont();
	CGameFont(float _fX, float _fY);
	virtual ~CGameFont();
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private :
	HDC m_hMemDC;
};


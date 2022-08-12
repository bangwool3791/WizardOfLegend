#pragma once
#include "Obj.h"
class CShop :
	public CObj
{
public:
	CShop();
	CShop(float _fX, float _fY);
	virtual ~CShop();
public :
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private :
	HDC m_hMemDC;
	HDC m_hMemTableDC;
};


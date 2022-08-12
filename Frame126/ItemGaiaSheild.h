#pragma once
#include "Obj.h"
#include "Struct.h"
class CPlayer;
class CItemGaiaSheild :
	public CObj
{
private:
	ITEM m_tItem;
	CPlayer* m_DownCastingPlayer;
	HDC m_hMemDC;
public:
	CItemGaiaSheild();
	CItemGaiaSheild(float _fX, float _fY);
	virtual ~CItemGaiaSheild();
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);

public:
	void Collision_Rect();
};


#pragma once
#include "Obj.h"
#include "Struct.h"
class CPlayer;
class CItemFDragon :
	public CObj
{
private : 
	ITEM m_tItem;
	CPlayer* m_DownCastingPlayer;
public:
	CItemFDragon();
	CItemFDragon(float _fX, float _fY);
	virtual ~CItemFDragon();
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);

public :
	void Collision_Rect();
};


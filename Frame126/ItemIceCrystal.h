#pragma once
#include "Obj.h"
#include "Struct.h"
class CPlayer;
class CItemIceCrystal :
	public CObj
{
private : 
	ITEM m_tItem;
	CPlayer* m_DownCastingPlayer;
public:
	CItemIceCrystal();
	CItemIceCrystal(float _fX, float _fY);
	virtual ~CItemIceCrystal();
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);

public :
	void Collision_Rect();
};


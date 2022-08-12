#pragma once
#include "Obj.h"
#include "Struct.h"
class CPlayer;
class CItemPotion :
	public CObj
{
private:
	ITEM m_tItem;
	CPlayer* m_DownCastingPlayer;
public:
	CItemPotion();
	CItemPotion(float _fX, float _fY);
	virtual ~CItemPotion();
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);

public:
	void Collision_Rect();
};


#pragma once
#include "Obj.h"
class CColisionTile :
	public CObj
{
private :
	list<CObj*> * m_pMonsterList;
	list<CObj*> * m_pSkillList;
private :
	void Collision_Tile(CObj* _Sour, list<CObj*>* _Dest);
public:
	CColisionTile();
	CColisionTile(float _fX, float _fY);
	CColisionTile(INFO& _info);
	virtual ~CColisionTile();

public:
	virtual void Initialize(void) override;
	virtual int  Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
};


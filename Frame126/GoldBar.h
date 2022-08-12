#pragma once
#include "Obj.h"

class CGameFont;
class CGoldBar :
	public CObj
{
public :
	enum GOLD_INDEX{GOLD_INDEX_0, GOLD_INDEX_1
	, GOLD_INDEX_2};
public:
	CGoldBar();
	CGoldBar(float _fX, float _fY);
	virtual ~CGoldBar();
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private:
	vector<CGameFont*>       m_vecFont;
private:
	HDC m_hMemDC;
};


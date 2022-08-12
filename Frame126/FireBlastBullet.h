#pragma once
#include "Obj.h"
class CFireBlastBullet :
	public CObj
{
private :
	void Get_PlayerInfo(float * _fR, float* _fAngle);
private :
	DWORD m_dTick;
	list<CObj*> m_listFireBlast;
public:
	CFireBlastBullet();
	CFireBlastBullet(float _fX, float _fY);
	~CFireBlastBullet();
public :
	void	Initialize();
	int		Update();
	void	Late_Update();
	void	Render(HDC hDC);
	void	Release();
};


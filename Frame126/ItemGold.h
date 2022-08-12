#pragma once
#include "Obj.h"
#include "Struct.h"
class CPlayer;
class CItemGold :
	public CObj
{
private :
	HDC m_hMemDC;
private:
	CPlayer* m_DownCastingPlayer;
public:
	CItemGold();
	CItemGold(float _fX, float _fY);
	virtual ~CItemGold();
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);

public:
	void Collision_Rect();
private :
	void Get_PlayerInfo(float * _fR, float* _fAngle)
	{
		float fX = Displacement<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX);
		float fY = Displacement<float>(m_pPlayer->Get_Info().fY, m_tInfo.fY);
		*_fR = Distance<float>(fX, fY);
		*_fAngle = Angle<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX, m_tInfo.fY, m_pPlayer->Get_Info().fY);
	}
};


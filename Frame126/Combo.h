#pragma once
#include "Obj.h"
class CCombo :
	public CObj
{

public :
	enum COMOBO_STATE{COMBO_STATE_LEFT, COMBO_STATE_UP, COMBO_STATE_RIGHT, COMBO_STATE_DOWN, COMBO_STATE_END};
private :
	COMOBO_STATE m_eComboState = COMBO_STATE_LEFT;
	int	m_iComboCount = 0;
	BLENDFUNCTION m_bf;
public:
	CCombo();
	CCombo(float _fX, float _fY);
	virtual ~CCombo();
private :
	HDC m_hMemDC;
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private :
	void Collision_Sphere(CObj* _Dest, CObj* _Sour);
};


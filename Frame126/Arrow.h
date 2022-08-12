#pragma once
#include "Obj.h"
class CArrow :
	public CObj
{
private :
	HDC m_hMemDC;
public :
	void Set_Monster(CObj* _pMonster) { m_pMonster = _pMonster; }
private :
	INFO	m_tInitInfo;
private:
	HPEN	m_hPen;
	HPEN	m_hOldPen;
	HDC		m_hDC;
private :
	CObj* m_pMonster;
private :
	int	m_iYFrame;
public :
	const int& Get_YFrame() { return m_iYFrame;}
	void Set_YFrame(const int& _XFrame) { m_iYFrame = _XFrame; }
public:
	CArrow();
	CArrow(float _fX, float _fY, float _fAngle);
	virtual ~CArrow();
public :
	void Initialize();
	int	 Update();
	void Late_Update();
	void Render(HDC hDC);
	void Release();
private :
	void CArrow::Collision_Sphere(CObj* _Dest, CObj* _Sour);
	void CArrow::Collision_Sphere(CObj* _Dest, list<CObj*>* Sour);
};


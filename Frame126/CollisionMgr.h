#pragma once

class CObj;
class CCollisionMgr
{
public :
	//충돌 시 사라지는 스킬을 위한 enum 변수
	enum COL_OPTION { COLLISION, DEAD };
public:
	CCollisionMgr();
	~CCollisionMgr();

public:
	static  void    Set_SourPoint(float fX, float fY, CObj* _Dest, CObj* _Sour);
	static	void	Collision_Rect(list<CObj*> _Dest, list<CObj*> _Sour);
	static  void	Collision_RectEx(CObj* _Dest, CObj* _Sour);
	static  void    Collision_RectEx(CObj* _Dest, list<CObj*>* _Sour);
	static	void	Collision_RectEx(list<CObj*> _Dest, list<CObj*> _Sour);
	//객체 안에서 충돌검사
	static void		Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour);
	static void     Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour, COL_OPTION _COL_OPTION);
	static void     Collision_Sphere(CObj* _Dest, CObj* _Sour);
	static void     Collision_Sphere(CObj* _Dest, CObj* _Sour, COL_OPTION _COL_OPTION);
	static	void	Collision_Sphere(list<CObj*> _Dest, list<CObj*> _Sour);
	static bool		Check_Sphere(CObj* pDest, CObj* pSour, float* _fR, float* _fAngle);
	static bool		Check_Rect(CObj* pDest, CObj* pSour, float* _fX, float* _fY);

};


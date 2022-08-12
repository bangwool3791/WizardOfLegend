#pragma once

#include "Obj.h"
#include "Struct.h"
// 추상 팩토리 패턴 : 생성 패턴의 한 방법으로 인스턴스를 만드는 절차를 추상화하였다하여 추상 팩토리 패턴이라 부른다.
// 즉, 객체 생성 시 동반되는 공통적인 작업을 추상화하는 과정이라 말할 수 있다.

template<typename T>
class CAbstactFactory
{
public:
	CAbstactFactory() {}
	~CAbstactFactory() {}

public:
	static		CObj*		Create(void)
	{
		CObj*		pObj = new T;
		pObj->Initialize();

		return pObj;
	}

	static		CObj*		Create(const T& Rhs)
	{
		CObj*		pObj = new T(Rhs);
		pObj->Initialize();

		return pObj;
	}

	static		CObj*		Create(float fX, float fY)
	{
		CObj*		pObj = new T(fX, fY);
		pObj->Initialize();
		return pObj;
	}

	/*static		CObj*		Create(float fX, float fY, DIR eDir = DIR_END)
	{
		CObj*		pObj = new T;
		pObj->Initialize();

		pObj->Set_Pos(fX, fY);
		pObj->Set_Direction(eDir);		

		return pObj;
	}*/

	static		CObj*		Create(float fX, float fY, float _fAngle)
	{
		CObj*		pObj = new T(fX, fY, (float)_fAngle);
		pObj->Initialize();
		return pObj;
	}

	static		CObj*		Create(INVEN* _pInven)
	{
		CObj*		pObj = new T(_pInven);
		pObj->Initialize();
		return pObj;
	}
};


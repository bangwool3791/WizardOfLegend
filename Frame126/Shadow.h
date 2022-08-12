#pragma once
#include "Obj.h"
class CShadow :
	public CObj
{
public :
	void Set_Shawdow(TCHAR* _key, float _fCX, float _fCY);
private :
	HDC m_hMemDC;
public:
	CShadow();
	CShadow(float _fX, float _fY);
	virtual ~CShadow();
public :
	virtual void Initialize(void) override;
	virtual int	 Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

};


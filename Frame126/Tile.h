#pragma once
#include "Obj.h"
#include "Enum.h"
class CTile :
	public CObj
{
private :
	HDC m_hMemDC;
public:
	CTile();
	CTile(float fX, float fY);
	CTile(const CTile& Rhs);
	CTile(const INFO& Rhs);
	virtual ~CTile();

public:
private:
public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;


};


#pragma once
#include "Obj.h"
class CThunder :
	public CObj
{
private :
	float m_fRadius;
public :	
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:
	CThunder();
	CThunder(float _x, float _y, float _fAngle);
	virtual ~CThunder();
};


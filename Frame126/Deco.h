#pragma once
#include "Obj.h"
#include "Enum.h"
class CDeco :
	public CObj
{
private :
	HDC m_hMemDC;
public:
	CDeco();
	CDeco(float fX, float fY);
	CDeco(const CDeco& Rhs);
	CDeco(const INFO& Rhs);
	virtual ~CDeco();

public:
private:
public:
	virtual void Initialize(void) override;
	virtual int  Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;


};


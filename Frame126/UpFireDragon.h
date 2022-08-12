#pragma once
#include "FireDragon.h"
class CUpFireDragon :public CFireDragon
{
public:
	CUpFireDragon();
	CUpFireDragon(float fX, float fY, float _fAngle);
	virtual ~CUpFireDragon();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
};


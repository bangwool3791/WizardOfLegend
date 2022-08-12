#pragma once
#include "FireDragon.h"
class CDownFireDragon :
	public CFireDragon
{
public:
	CDownFireDragon();
	CDownFireDragon(float fX, float fY, float _fAngle);
	virtual ~CDownFireDragon();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
};


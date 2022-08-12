#pragma once

#include "Include.h"

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual	void	Initialize(void)  = 0;
	virtual void	Update(void)      = 0;
	virtual void	Late_Update(void) = 0;
	virtual void	Render(HDC _DC)   = 0;
	virtual void	Release(void)     = 0;
};


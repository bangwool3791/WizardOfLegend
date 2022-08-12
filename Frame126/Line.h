#pragma once

#include "Include.h"

class CLine
{
public:
	CLine();
	CLine(LINE& _tLine);
	CLine(LINEPOINT& _Left, LINEPOINT& _Right);
	~CLine();

public:
	const LINE&		Get_Info(void)const { return m_tLineInfo; }

public:
	void		Render(HDC _DC);

private:
	LINE			m_tLineInfo;

};


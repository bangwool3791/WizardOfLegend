#include "stdafx.h"
#include "Line.h"
#include "ScrollMgr.h"


CLine::CLine()
{
}


CLine::CLine(LINEPOINT& _Left, LINEPOINT& _Right)
	: m_tLineInfo(_Left, _Right)
{

}

CLine::CLine(LINE& _tLine)
	: m_tLineInfo(_tLine)
{

}

CLine::~CLine()
{
}

void CLine::Render(HDC _DC)
{
	int	iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();

	MoveToEx(_DC, (int)m_tLineInfo.tLPoint.fX + iScrollX, (int)m_tLineInfo.tLPoint.fY, NULL);
	LineTo(_DC, (int)m_tLineInfo.tRPoint.fX + iScrollX, (int)m_tLineInfo.tRPoint.fY);
}

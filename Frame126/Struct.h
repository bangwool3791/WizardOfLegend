#pragma once
#include "Enum.h"
#include <vector>

typedef struct tagInfo
{
	float		fX;
	float		fY;
	float		fCX;
	float		fCY;
	int			iDrawXID;
	int			iDrawYID;
	int	iOption;

}INFO;

typedef		struct tagLinePoint
{
	float		fX;
	float		fY;

	tagLinePoint() { ZeroMemory(this, sizeof(tagLinePoint)); }
	tagLinePoint(float _fX, float _fY)
		: fX(_fX), fY(_fY) {	}

}LINEPOINT;

typedef	struct tagLineInfo
{
	LINEPOINT		tLPoint;
	LINEPOINT		tRPoint;

	tagLineInfo(void) { ZeroMemory(this, sizeof(tagLineInfo)); }
	tagLineInfo(LINEPOINT& tLeft, LINEPOINT& tRight)
		: tLPoint(tLeft), tRPoint(tRight)
	{

	}

}LINE;


typedef	struct tagFrame
{
	int		iFrameStart;
	int		iFrameInit;
	int		iFrameEnd;
	int		iFrameMotion;
	int		iFrameMotionEnd;
	DWORD	dwFrameSpeed;
	DWORD	dwFrameTime;
}FRAME;

typedef	struct tagDragonFrame
{
	int		iFrameMid;
	int		iFrameMax;
	int		iFrameMin;
	int		iFrameMotion;
	DWORD	dwFrameSpeed;
	DWORD	dwFrameTime;

}DRAGONFRAME;

typedef struct tagSkillElem
{
	int iCount;
	int iMaxCount;
	DWORD dTick;
	DWORD dMaxTick;
	SKILLID eSkillID;
	float	fSkillRadius;
	float	fSkillAngle;
}SkillElem;

typedef struct tagINVEN
{
	PRO_SKILL eSkillState;
	vector<SkillElem> VecSkillElem;
	float fX;
	float fY;
	TCHAR* InvenImageKey;
	RECT tRect;
	TCHAR * SKillImagekey;
	TCHAR* CoolTimeImgekey;
	TCHAR* ImageEX;
	RECT tSkillRect;
	float fCY;
	SKILL   eSKillMotion;
	TCHAR* pPreSKillImage;
}INVEN;

typedef struct tagItem
{
	 INVENTORY eInven;
	 PRO_SKILL eProSKill;
	 TCHAR* pSkillImage;
	 TCHAR* pInvenImage;
	 TCHAR* pInvenEx;
}ITEM;
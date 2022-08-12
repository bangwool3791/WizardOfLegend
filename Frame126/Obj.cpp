#include "stdafx.h"
#include "Obj.h"
#include "UserHeader.h"
float CObj::g_fSound = 0.1f;
CObj::CObj()
	: m_fSpeed(0.f), m_bDead(false)
	, m_eDir(DIR(DIR_END)), m_fAngle(0.f)
	, m_pTarget(nullptr), m_eGroup(EFFECT), m_pPlayer(nullptr), m_pObjMgr(nullptr)
	, m_iHp(100), m_iAttack(1)
{
	//INFO
	ZeroMemory(&m_tInfo, sizeof(INFO));
	ZeroMemory(&m_tRect, sizeof(RECT));
	Set_Option(TILE_OPT_END);
	//FRAME
	ZeroMemory(&m_tFrame, sizeof(FRAME));
	m_pObjMgr = CObjMgr::Get_Instance();
}


CObj::~CObj()
{
}

void CObj::Update_Rect(void)
{
	m_tRect.left	= long(m_tInfo.fX - (m_tInfo.fCX / 2.f));
	m_tRect.top		= long(m_tInfo.fY - (m_tInfo.fCY / 2.f));
	m_tRect.right	= long(m_tInfo.fX + (m_tInfo.fCX / 2.f));
	m_tRect.bottom	= long(m_tInfo.fY + (m_tInfo.fCY / 2.f));
}

bool CObj::InCrease_X_Frame(FRAME& _tFrame)
{
	if (_tFrame.dwFrameTime + _tFrame.dwFrameSpeed < GetTickCount())
	{
		++_tFrame.iFrameStart;
		_tFrame.dwFrameTime = GetTickCount();

		if (_tFrame.iFrameStart > _tFrame.iFrameEnd)
		{
			_tFrame.iFrameStart = 0;
			return true;
		}
	}
	return false;
}

bool CObj::InCrease_Init_X_Frame(FRAME& _tFrame)
{
	if (_tFrame.dwFrameTime + _tFrame.dwFrameSpeed < GetTickCount())
	{
		++_tFrame.iFrameStart;
		_tFrame.dwFrameTime = GetTickCount();

		if (_tFrame.iFrameStart > _tFrame.iFrameEnd)
		{
			_tFrame.iFrameStart = _tFrame.iFrameInit;
			return true;
		}
	}
	return false;
}

bool CObj::DeCrease_X_Frame(FRAME& _tFrame)
{
	if (_tFrame.dwFrameTime + _tFrame.dwFrameSpeed < GetTickCount())
	{
		--_tFrame.iFrameStart;
		_tFrame.dwFrameTime = GetTickCount();

		if (_tFrame.iFrameStart < _tFrame.iFrameEnd)
		{
			_tFrame.iFrameStart = _tFrame.iFrameInit;
			return true;
		}
	}
	return false;
}

bool CObj::InCrease_X_FrameWithY(FRAME& _tFrame)
{
	if (_tFrame.dwFrameTime + _tFrame.dwFrameSpeed < GetTickCount())
	{
		++_tFrame.iFrameStart;
		_tFrame.dwFrameTime = GetTickCount();

		if (_tFrame.iFrameStart*_tFrame.iFrameMotion >= _tFrame.iFrameEnd* _tFrame.iFrameMotionEnd)
		{
			_tFrame.iFrameStart =	0;
			_tFrame.iFrameMotion =	0;
			return true;
		}else if (_tFrame.iFrameStart > _tFrame.iFrameEnd)
		{
			++_tFrame.iFrameMotion;
			_tFrame.iFrameStart = 0;
		}
	}
	return false;
}

void CObj::Set_State(const STATE& _eState)
{ 
	switch (_eState)
	{
	case STATE_STUN:
		Set_StunTick(GetTickCount());
		break;
	}
	m_eState = _eState; 
}
#include "stdafx.h"
#include "ScrollMgr.h"
#include "Define.h"
#include "ImageDefine.h"
CScrollMgr*	CScrollMgr::m_pInstance = nullptr;

CScrollMgr::CScrollMgr()
{

}


CScrollMgr::~CScrollMgr()
{
}

void CScrollMgr::Update()
{
	Process_Shake();
}
void CScrollMgr::Scroll_Lock()
{
	if (0.f < m_fScrollX)	// 플레이어가 왼쪽 벽에 닿았을 때
		m_fScrollX = 0.f;

	if (WINCX - TILEX * TILECX > m_fScrollX)	// 플레이어가 오른쪽 벽에 닿았을 때
		m_fScrollX = WINCX - TILEX * TILECX;

	if (0.f < m_fScrollY)	// 플레이어가 위쪽 벽에 닿았을 때
		m_fScrollY = 0.f;

	if (WINCY - TILEY * TILECY > m_fScrollY)	// 플레이어가 아래쪽 벽에 닿았을 때
		m_fScrollY = WINCY - TILEY * TILECY;
}

void CScrollMgr::Process_Shake()
{
	static int iCount = 0;

	if (!m_dShakeTick)
		return;

	if (m_dShakeTick + 50 < GetTickCount())
	{
		if (m_bShake)
		{
			m_fScrollY += 7.f;
			m_bShake = false;
		}
		else
		{
			m_fScrollY -= 7.f;
			m_bShake = true;
		}
		if (6 < iCount)
		{
			iCount = 0;
			m_dShakeTick = 0;
		}
		else
		{
			++iCount;
			m_dShakeTick = GetTickCount();
		}
	}

}
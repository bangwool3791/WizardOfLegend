#include "stdafx.h"
#include "KeyMgr.h"

CKeyMgr*		CKeyMgr::m_pInstance = nullptr;

CKeyMgr::CKeyMgr()
{
	ZeroMemory(m_bKeyState, sizeof(m_bKeyState));
}


CKeyMgr::~CKeyMgr()
{
}

bool CKeyMgr::Key_Pressing(int _Key)
{
	if (GetAsyncKeyState(_Key) & 0x8000)
		return true;

	return false;
}

// 이전에는 눌림이 없고 현재 눌린 경우
bool CKeyMgr::Key_Down(int _Key)
{
	if (!m_bKeyState[_Key] && GetAsyncKeyState(_Key) & 0x8000)
	{
		m_bKeyState[_Key] = !m_bKeyState[_Key];
		return true;
	}

	if (m_bKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000))
		m_bKeyState[_Key] = !m_bKeyState[_Key];
	
	return false;
}
// 이런에 눌린 상태 하지만 현재는 눌림이 없는 상태
bool CKeyMgr::Key_Up(int _Key)
{
	if (m_bKeyState[_Key] && !(GetAsyncKeyState(_Key) & 0x8000))
	{
		m_bKeyState[_Key] = !m_bKeyState[_Key];
		return true;
	}

	if (!m_bKeyState[_Key] && (GetAsyncKeyState(_Key) & 0x8000))
		m_bKeyState[_Key] = !m_bKeyState[_Key];

	return false;
}
// key값 복원
void CKeyMgr::Update()
{
	for (int i = 0; i < VK_MAX; ++i)
	{
		

		
	}
}

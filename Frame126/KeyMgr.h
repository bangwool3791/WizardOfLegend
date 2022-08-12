#pragma once

#include "Include.h"

class CKeyMgr
{
private:
	CKeyMgr();
	~CKeyMgr();

public:
	bool	Key_Pressing(int _Key);
	bool	Key_Down(int _Key);	// 한 번만 눌렀을 때
	bool	Key_Up(int _Key); // 눌렀다가 뗐을 때 true 리턴
	void	Update();

public:
	static CKeyMgr*		Get_Instance(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new CKeyMgr;
		}

		return m_pInstance;
	}

	static void	Destroy_Instance(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static	CKeyMgr*		m_pInstance;

private:
	bool			m_bKeyState[VK_MAX];
};


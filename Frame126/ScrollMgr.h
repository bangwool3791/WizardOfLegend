#pragma once

class CScrollMgr
{
public :
	void CScrollMgr::Update();
	void Set_ShakeTick()
	{
		m_dShakeTick = GetTickCount();
	}
private :
	bool m_bShake = false;
	DWORD m_dShakeTick = 0;
	void Process_Shake();
private:
	CScrollMgr();
	~CScrollMgr();

public:
	float			Get_ScrollX()			{ return m_fScrollX; }
	float			Get_ScrollY()			{ return m_fScrollY; }

	void			Set_ScrollX(float _fX) { m_fScrollX += _fX; }
	void			Set_ScrollY(float _fY) { m_fScrollY += _fY; }

	void			Update_ScrollX(float _fX) { m_fScrollX += _fX; }
	void			Update_ScrollY(float _fY) { m_fScrollY += _fY; }

	void			Scroll_Lock();

public:
	static CScrollMgr*		Get_Instance(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new CScrollMgr;
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
	static	CScrollMgr*		m_pInstance;
	float					m_fScrollX = 0.f;
	float					m_fScrollY = 0.f;

};


#pragma once

#include "MyBmp.h"

class CBmpMgr
{
private:
	CBmpMgr();
	~CBmpMgr();

public:
	static CBmpMgr*		Get_Instance(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new CBmpMgr;
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

public:
	void	Insert_Bmp(const TCHAR* pFilePath, const TCHAR* pImgKey);
	HDC		Find_Image(const TCHAR* pImgKey);
	HDC     Find_Rotation_Image(const TCHAR* pImgKey);
	void	Release();

private:
	static	CBmpMgr*		m_pInstance;
	map<const TCHAR*, CMyBmp*>			m_MapBmp;


};


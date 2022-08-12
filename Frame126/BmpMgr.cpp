#include "stdafx.h"
#include "BmpMgr.h"

CBmpMgr*	CBmpMgr::m_pInstance = nullptr;

CBmpMgr::CBmpMgr()
{
}

CBmpMgr::~CBmpMgr()
{
	Release();
}

void CBmpMgr::Insert_Bmp(const TCHAR * pFilePath, const TCHAR * pImgKey)
{
	auto iter = find_if(m_MapBmp.begin(), m_MapBmp.end(), CTagFinder(pImgKey));

	if (iter == m_MapBmp.end())
	{ 
		CMyBmp*		pBmp = new CMyBmp;
		pBmp->Load_Bmp(pFilePath);

		m_MapBmp.emplace(pImgKey, pBmp);
	}
}

HDC CBmpMgr::Find_Image(const TCHAR* pImgKey)
{
	// auto	iter = m_MapBmp.find(pImgKey);

	auto iter = find_if(m_MapBmp.begin(), m_MapBmp.end(), CTagFinder(pImgKey));

	if (iter == m_MapBmp.end())
		return nullptr; 

	return iter->second->Get_MemDC();
}
HDC CBmpMgr::Find_Rotation_Image(const TCHAR* pImgKey)
{
	// auto	iter = m_MapBmp.find(pImgKey);

	auto iter = find_if(m_MapBmp.begin(), m_MapBmp.end(), CTagFinder(pImgKey));

	if (iter == m_MapBmp.end())
		return nullptr;

	return iter->second->Get_RotDC();
}

void CBmpMgr::Release()
{
	for_each(m_MapBmp.begin(), m_MapBmp.end(), CDeleteMap());
	m_MapBmp.clear();
}


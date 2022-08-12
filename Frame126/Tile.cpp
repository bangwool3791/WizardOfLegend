#include "stdafx.h"
#include "Tile.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "UserHeader.h"


CTile::CTile()
{
}

CTile::CTile(float fX, float fY)
{
	m_tInfo.fX = fX;
	m_tInfo.fY = fY;
}

CTile::CTile(const CTile& Rhs)
{
	memcpy_s(&m_tInfo, sizeof(INFO), &Rhs.m_tInfo, sizeof(INFO));
	m_tInfo.iDrawXID = Rhs.Get_Info().iDrawXID;
	m_tInfo.iDrawYID = Rhs.Get_Info().iDrawYID;
	m_tInfo.iOption = Rhs.Get_Info().iOption;
}

CTile::CTile(const INFO& Rhs)
{
	memcpy_s(&m_tInfo, sizeof(INFO), &Rhs, sizeof(INFO));
	m_tInfo.iOption = Rhs.iOption;
}

CTile::~CTile()
{
	Release();
}

void CTile::Initialize(void)
{
	m_tInfo.fCX = TILECX;
	m_tInfo.fCY = TILECY;
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/FireTileSet_1024.bmp", L"Tile");
	m_pFrameKey = L"Tile";
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	Update_Rect();
}

int CTile::Update(void)
{
	Update_Rect();

	return OBJ_NOEVENT;
}

void CTile::Late_Update(void)
{
	
}

void CTile::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	BitBlt(hDC, (int)m_tRect.left + iScrollX, (int)m_tRect.top + iScrollY, (int)m_tInfo.fCX, (int)m_tInfo.fCY, m_hMemDC, TILECX * m_tInfo.iDrawXID, TILECY * m_tInfo.iDrawYID, SRCCOPY);
}

void CTile::Release(void)
{
	
}

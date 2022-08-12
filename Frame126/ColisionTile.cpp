#include "stdafx.h"
#include "ColisionTile.h"
#include "UserHeader.h"
#include "HitEffect.h"

CColisionTile::CColisionTile()
{
}

CColisionTile::CColisionTile(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
	Update_Rect();
}

CColisionTile::CColisionTile(INFO& _info)
{
	m_tInfo.fX = _info.fX;
	m_tInfo.fY = _info.fY;
	Update_Rect();
}

CColisionTile::~CColisionTile()
{
}

void CColisionTile::Initialize(void)
{
	m_tInfo.fCX = COLLISION_TILECX;
	m_tInfo.fCY = COLLISION_TILECX;
	m_pPlayer = m_pObjMgr->Get_Player();
	Update_Rect();
}
int CColisionTile::Update(void)
{
	Update_Rect();
	return OBJ_NOEVENT;
}

void CColisionTile::Late_Update(void)
{
}

void CColisionTile::Render(HDC hDC)
{
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_TAB))
	{
		int fX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
		int fY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
		Rectangle(hDC, m_tRect.left+ fX, m_tRect.top+ fY, m_tRect.right+ fX, m_tRect.bottom+ fY);
	}
}

void CColisionTile::Release(void)
{

}

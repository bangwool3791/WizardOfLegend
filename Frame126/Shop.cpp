#include "stdafx.h"
#include "Shop.h"
#include "UserHeader.h"
#include "ItemGaiaSheild.h"
#include "ItemPotion.h"

CShop::CShop()
{
}

CShop::CShop(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CShop::~CShop()
{
}

void CShop::Initialize(void)
{
	m_tInfo.fCX = FCX_UI_SHOP;
	m_tInfo.fCY = FCY_UI_SHOP;

	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"ITEMSHOP_NPC");
	m_hMemTableDC = CBmpMgr::Get_Instance()->Find_Image(L"SHOP_TABLE");

	m_pObjMgr->AddObject(UI_KEY, UI_ITEM, CAbstactFactory<CItemGaiaSheild>::Create((float)(m_tInfo.fX - m_tInfo.fCX * 0.5 - FCY_UI_TABLE * 0.5 - FCX_GAIA_ARMOR_CARD * 0.5), (float)(m_tInfo.fY + m_tInfo.fCY * 0.5)));
	m_pObjMgr->AddObject(UI_KEY, UI_ITEM, CAbstactFactory<CItemPotion>::Create((float)(m_tInfo.fX - m_tInfo.fCX * 0.5 - FCY_UI_TABLE *0.5- FCX_ITEM_POTION * 0.5), (float)(m_tInfo.fY - m_tInfo.fCY * 0.5)));
	Update_Rect();
}
int	CShop::Update(void)
{
	return OBJ_NOEVENT;
 }
void CShop::Late_Update(void)
{

 }
void CShop::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		FCX_UI_SHOP,
		FCY_UI_SHOP,
		m_hMemDC,
		0,
		0,
		FCX_UI_SHOP,
		FCY_UI_SHOP,
		RGB(255, 0, 255));

	GdiTransparentBlt(hDC,
		(int)(m_tRect.left - FCX_UI_SHOP * 0.5 - FCX_UI_POTION * 0.f + iScrollX),
		(int)(m_tRect.top + iScrollY),
		(int)FCX_UI_SHOP,
		(int)FCY_UI_SHOP,
		m_hMemTableDC,
		0,
		0,
		(int)FCX_UI_SHOP,
		(int)FCY_UI_SHOP,
		RGB(255, 0, 255));
 }
void CShop::Release(void)
{

 }
#include "stdafx.h"
#include "GoldBar.h"
#include "GameFont.h"
#include "UserHeader.h"

CGoldBar::CGoldBar()
{
}

CGoldBar::CGoldBar(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CGoldBar::~CGoldBar()
{
}

void CGoldBar::Initialize(void)
{
	m_tInfo.fCX = FCX_UI_GOLD;
	m_tInfo.fCY = FCY_UI_GOLD;

	m_pFrameKey = L"UI_MONEY";

	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	m_pObjMgr->AddObject(UI_KEY, UI_GOLD, CAbstactFactory<CGameFont>::Create(m_tInfo.fX +48, m_tInfo.fY));
	m_vecFont.push_back(dynamic_cast<CGameFont*>(m_pObjMgr->Get_Target(UI_KEY, UI_GOLD, nullptr)));
	m_pObjMgr->AddObject(UI_KEY, UI_GOLD, CAbstactFactory<CGameFont>::Create(m_tInfo.fX + 32, m_tInfo.fY));
	m_vecFont.push_back(dynamic_cast<CGameFont*>(m_pObjMgr->Get_Target(UI_KEY, UI_GOLD, nullptr)));
	m_pObjMgr->AddObject(UI_KEY, UI_GOLD, CAbstactFactory<CGameFont>::Create(m_tInfo.fX + 16, m_tInfo.fY));
	m_vecFont.push_back(dynamic_cast<CGameFont*>(m_pObjMgr->Get_Target(UI_KEY, UI_GOLD, nullptr)));

	Update_Rect();
}

int	CGoldBar::Update(void)
{
	if ((m_iGold) % 10 == 0)
		m_vecFont[GOLD_INDEX_0]->Set_Font(L"FONT_0");
	else if ((m_iGold) % 10 == 1)
		m_vecFont[GOLD_INDEX_0]->Set_Font(L"FONT_1");
	else if ((m_iGold) % 10 == 2)
		m_vecFont[GOLD_INDEX_0]->Set_Font(L"FONT_2");
	else if ((m_iGold) % 10 == 3)
		m_vecFont[GOLD_INDEX_0]->Set_Font(L"FONT_3");
	else if ((m_iGold) % 10 == 4)
		m_vecFont[GOLD_INDEX_0]->Set_Font(L"FONT_4");
	else if ((m_iGold) % 10 == 5)
		m_vecFont[GOLD_INDEX_0]->Set_Font(L"FONT_5");
	else if ((m_iGold) % 10 == 6)
		m_vecFont[GOLD_INDEX_0]->Set_Font(L"FONT_6");
	else if ((m_iGold) % 10 == 7)
		m_vecFont[GOLD_INDEX_0]->Set_Font(L"FONT_7");
	else if ((m_iGold) % 10 == 8)
		m_vecFont[GOLD_INDEX_0]->Set_Font(L"FONT_8");
	else if ((m_iGold) % 10 == 9)
		m_vecFont[GOLD_INDEX_0]->Set_Font(L"FONT_9");

	if ((m_iGold) % 100 / 10 == 0)
		m_vecFont[GOLD_INDEX_1]->Set_Font(L"FONT_0");
	else if ((m_iGold) % 100 / 10 == 1)
		m_vecFont[GOLD_INDEX_1]->Set_Font(L"FONT_1");
	else if ((m_iGold) % 100 / 10 == 2)
		m_vecFont[GOLD_INDEX_1]->Set_Font(L"FONT_2");
	else if ((m_iGold) % 100 / 10 == 3)
		m_vecFont[GOLD_INDEX_1]->Set_Font(L"FONT_3");
	else if ((m_iGold) % 100 / 10 == 4)
		m_vecFont[GOLD_INDEX_1]->Set_Font(L"FONT_4");
	else if ((m_iGold) % 100 / 10 == 5)
		m_vecFont[GOLD_INDEX_1]->Set_Font(L"FONT_5");
	else if ((m_iGold) % 100 / 10 == 6)
		m_vecFont[GOLD_INDEX_1]->Set_Font(L"FONT_6");
	else if ((m_iGold) % 100 / 10 == 7)
		m_vecFont[GOLD_INDEX_1]->Set_Font(L"FONT_7");
	else if ((m_iGold) % 100 / 10 == 8)
		m_vecFont[GOLD_INDEX_1]->Set_Font(L"FONT_8");
	else if ((m_iGold) % 100 / 10 == 9)
		m_vecFont[GOLD_INDEX_1]->Set_Font(L"FONT_9");

	if ((m_iGold) / 100 == 0)
		m_vecFont[GOLD_INDEX_2]->Set_Font(L"FONT_0");
	else if ((m_iGold) / 100 == 1)
		m_vecFont[GOLD_INDEX_2]->Set_Font(L"FONT_1");
	else if ((m_iGold) / 100 == 2)
		m_vecFont[GOLD_INDEX_2]->Set_Font(L"FONT_2");
	else if ((m_iGold) / 100 == 3)
		m_vecFont[GOLD_INDEX_2]->Set_Font(L"FONT_3");
	else if ((m_iGold) / 100 == 4)
		m_vecFont[GOLD_INDEX_2]->Set_Font(L"FONT_4");
	else if ((m_iGold) / 100 == 5)
		m_vecFont[GOLD_INDEX_2]->Set_Font(L"FONT_5");
	else if ((m_iGold) / 100 == 6)
		m_vecFont[GOLD_INDEX_2]->Set_Font(L"FONT_6");
	else if ((m_iGold) / 100 == 7)
		m_vecFont[GOLD_INDEX_2]->Set_Font(L"FONT_7");
	else if ((m_iGold) / 100 == 8)
		m_vecFont[GOLD_INDEX_2]->Set_Font(L"FONT_8");
	else if ((m_iGold) / 100 == 9)
		m_vecFont[GOLD_INDEX_2]->Set_Font(L"FONT_9");
	return OBJ_NOEVENT;
}
void CGoldBar::Late_Update(void)
{
	
}
void CGoldBar::Render(HDC hDC)
{
	GdiTransparentBlt(hDC,							// 1인자 : 복사 받을 최종적으로 그림을 그릴 DC
		(int)m_tRect.left,	// 2,3인자 : 2, 3인자 : 복사 받을 위치의 좌표를 전달(L, T 좌표)
		(int)m_tRect.top,
		(int)m_tInfo.fCX,				// 4,5 인자 : 복사 받을 텍스처의 X, Y 사이즈
		(int)m_tInfo.fCY,
		m_hMemDC,							// 6인자 : 비트맵을 가지고 있는 DC
		0,								// 7, 8인자 : 비트맵을 출력할 시작 좌표
		0,
		(int)m_tInfo.fCX,								// 9, 10인자 : 복사할 비트맵의 X,Y 사이즈	
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));
}
void CGoldBar::Release(void)
{

}




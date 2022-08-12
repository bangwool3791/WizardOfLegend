#include "stdafx.h"
#include "MyMenu.h"
#include "BmpMgr.h"
#include "AbstactFactory.h"
#include "MyButton.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "UserHeader.h"

CMyMenu::CMyMenu()
{
}


CMyMenu::~CMyMenu()
{
	Release();
}

void CMyMenu::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/MAIN_MENU.bmp", L"MAIN_MENU");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/OPTION_MENU.bmp", L"OPTION_MENU");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Menu/QUIT_MENU.bmp", L"QUIT_MENU");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Start.bmp", L"Start");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Edit.bmp", L"Edit");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Button/Exit.bmp", L"Exit");

	CObjMgr::Get_Instance()->Initialize(OBJ_KEY);
	CObjMgr::Get_Instance()->Initialize(MENU_KEY);
	m_PrimaryKey = L"MAIN_MENU";
	CObjMgr::Get_Instance()->AddObject(OBJ_KEY, OBJ_MOUSE, CAbstactFactory<CMouse>::Create());

	CMyButton*	pObj = dynamic_cast<CMyButton*>(CAbstactFactory<CMyButton>::Create(397.5f, 309.5f));
	CObjMgr::Get_Instance()->AddObject(MENU_KEY, MENU_BUTTON, pObj);
	pObj->Set_FrameKey(L"Start");
	pObj->Set_Parent(this);

	pObj = dynamic_cast<CMyButton*>(CAbstactFactory<CMyButton>::Create(397.5f, 441.f));
	CObjMgr::Get_Instance()->AddObject(MENU_KEY, MENU_BUTTON, pObj);
	pObj->Set_FrameKey(L"Edit");
	pObj->Set_Parent(this);

	pObj = dynamic_cast<CMyButton*>(CAbstactFactory<CMyButton>::Create(397.5f, 522.f));
	CObjMgr::Get_Instance()->AddObject(MENU_KEY, MENU_BUTTON, pObj);
	pObj->Set_FrameKey(L"Exit");
	pObj->Set_Parent(this);
	CSoundMgr::Get_Instance()->Set_Bgm(L"MAIN_MENU_BGM.mp3", 1.f);

	m_pObjMgr = CObjMgr::Get_Instance();
}

void CMyMenu::Update(void)
{
	if (m_pObjMgr)
	{
		CObjMgr::Get_Instance()->Update(MENU_KEY);
		CObjMgr::Get_Instance()->Update(OBJ_KEY);
	}
}

void CMyMenu::Late_Update(void)
{
	m_pObjMgr->Late_Update(MENU_KEY);
	m_pObjMgr->Late_Update(OBJ_KEY);
}

void CMyMenu::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_PrimaryKey);

	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);

	CObjMgr::Get_Instance()->Render(_DC, MENU_KEY);
	CObjMgr::Get_Instance()->Render(_DC, OBJ_KEY);
}

void CMyMenu::Release(void)
{
	CSoundMgr::Get_Instance()->StopAll();
	CObjMgr::Get_Instance()->Release(MENU_KEY);
	CObjMgr::Get_Instance()->Release(OBJ_KEY);
	CObjMgr::Get_Instance()->Clear_RenderSort(EFFECT);
}

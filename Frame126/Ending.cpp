#include "stdafx.h"
#include "Ending.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CEnding::CEnding()
{
}


CEnding::~CEnding()
{
	Release();
}

void CEnding::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/Ending.bmp", L"Ending");
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"Ending");
}

void CEnding::Update(void)
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RETURN))
	{
		DestroyWindow(g_hWnd);
		return;
	}
}

void CEnding::Late_Update(void)
{
}

void CEnding::Render(HDC _DC)
{
	BitBlt(_DC, 0, 0, WINCX, WINCY, m_hMemDC, 0, 0, SRCCOPY);

}

void CEnding::Release(void)
{

}

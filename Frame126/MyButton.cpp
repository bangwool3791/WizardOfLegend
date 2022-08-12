#include "stdafx.h"
#include "MyButton.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "MyMenu.h"
#include "UserHeader.h"

TCHAR* CMyButton::m_pObjectKey = nullptr;
CMyButton::CMyButton()
{
}

CMyButton::CMyButton(float fX, float fY)
{
	m_tInfo.fX = fX;
	m_tInfo.fY = fY;
}
CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize(void)
{
	m_tInfo.fCX = 110.f;
	m_tInfo.fCY = 15.f;

}

int CMyButton::Update(void)
{
	Update_Rect();

	return OBJ_NOEVENT;
}

void CMyButton::Late_Update(void)
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	if (PtInRect(&m_tRect, pt))
	{
		if (lstrcmp(m_pObjectKey, m_pFrameKey))
		{
			CSoundMgr::Get_Instance()->Set_Sound(L"SELECT_MENU.mp3", SOUND_EFFECT, SELECT_MENU);
			m_pObjectKey = m_pFrameKey;
		}
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			if (!lstrcmp(L"Start", m_pFrameKey))
			{
				CSceneMgr::Get_Instance()->Set_ScenID(SCENE_STAGE);
				//CSoundMgr::Get_Instance()->Set_Sound(L"CLICK_MENU.mp3", SOUND_EFFECT, SELECT_MENU);
			}

			else if (!lstrcmp(L"Edit", m_pFrameKey))
			{
				CSceneMgr::Get_Instance()->Set_ScenID(SCENE_EDIT);
				//CSoundMgr::Get_Instance()->Set_Sound(L"CLICK_MENU.mp3", SOUND_EFFECT, SELECT_MENU);
			}

			else if (!lstrcmp(L"Exit", m_pFrameKey))
			{
				DestroyWindow(g_hWnd);
				//CSoundMgr::Get_Instance()->Set_Sound(L"CLICK_MENU.mp3", SOUND_EFFECT, SELECT_MENU);
			}

			return;
		}
		else
		{
			if (!lstrcmp(L"Start", m_pFrameKey))
				m_pParent->Set_PrimaryKey(L"MAIN_MENU");

			else if (!lstrcmp(L"Edit", m_pFrameKey))
				m_pParent->Set_PrimaryKey(L"OPTION_MENU");

			else if (!lstrcmp(L"Exit", m_pFrameKey))
				m_pParent->Set_PrimaryKey(L"QUIT_MENU");
		}
	}

}

void CMyButton::Render(HDC hDC)
{
	//HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	//GdiTransparentBlt(hDC,							// 1인자 : 복사 받을 최종적으로 그림을 그릴 DC
	//	(int)m_tRect.left,	// 2,3인자 : 2, 3인자 : 복사 받을 위치의 좌표를 전달(L, T 좌표)
	//	(int)m_tRect.top,
	//	(int)m_tInfo.fCX,				// 4,5 인자 : 복사 받을 텍스처의 X, Y 사이즈
	//	(int)m_tInfo.fCY,
	//	hMemDC,							// 6인자 : 비트맵을 가지고 있는 DC
	//	(int)m_tInfo.fCX * m_iDrawXID,								// 7, 8인자 : 비트맵을 출력할 시작 좌표
	//	0,
	//	(int)m_tInfo.fCX,								// 9, 10인자 : 복사할 비트맵의 X,Y 사이즈	
	//	(int)m_tInfo.fCY,
	//	RGB(255, 255, 255));
}

void CMyButton::Release(void)
{
	
}

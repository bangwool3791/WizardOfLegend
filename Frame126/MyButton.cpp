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

	//GdiTransparentBlt(hDC,							// 1���� : ���� ���� ���������� �׸��� �׸� DC
	//	(int)m_tRect.left,	// 2,3���� : 2, 3���� : ���� ���� ��ġ�� ��ǥ�� ����(L, T ��ǥ)
	//	(int)m_tRect.top,
	//	(int)m_tInfo.fCX,				// 4,5 ���� : ���� ���� �ؽ�ó�� X, Y ������
	//	(int)m_tInfo.fCY,
	//	hMemDC,							// 6���� : ��Ʈ���� ������ �ִ� DC
	//	(int)m_tInfo.fCX * m_iDrawXID,								// 7, 8���� : ��Ʈ���� ����� ���� ��ǥ
	//	0,
	//	(int)m_tInfo.fCX,								// 9, 10���� : ������ ��Ʈ���� X,Y ������	
	//	(int)m_tInfo.fCY,
	//	RGB(255, 255, 255));
}

void CMyButton::Release(void)
{
	
}

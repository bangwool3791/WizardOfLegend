#include "stdafx.h"
#include "MainGame.h"
#include "Player.h"
#include "Monster.h"
#include "Mouse.h"
#include "LineMgr.h"
#include "BmpMgr.h"
#include "TileMgr.h"
#include "UserHeader.h"
#include "Obj.h"
CMainGame::CMainGame()
	: m_pObjMgr(CObjMgr::Get_Instance())
{
#ifdef _DEBUG
	m_dwTime = GetTickCount();
	m_iFPS = 0;
	ZeroMemory(m_szFPS, sizeof(TCHAR) * 32);
#endif

}

CMainGame::~CMainGame()
{
	Release();
}

void CMainGame::Initialize(void)
{
	m_DC = GetDC(g_hWnd);
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/UI_MOUSE.bmp", L"UI_MOUSE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Ground.bmp", L"Back");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/MiniMap.bmp", L"MiniMap");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/PLAYER_MINIMAP.bmp", L"PLAYER_MINIMAP");
	CSceneMgr::Get_Instance()->Set_ScenID(SCENE_MENU);
	CSoundMgr::Get_Instance()->Initialize();
	//CSceneMgr::Get_Instance()->Set_ScenID(SCENE_EDIT);

	srand(unsigned int(0));
}

void CMainGame::Update(void)
{
	CKeyMgr::Get_Instance()->Update();
	CSceneMgr::Get_Instance()->Update();
	CSoundMgr::Get_Instance()->Update();
	CScrollMgr::Get_Instance()->Update();
	KeyInput();

}

void CMainGame::Late_Update(void)
{
	CSceneMgr::Get_Instance()->Late_Update();
	CScrollMgr::Get_Instance()->Scroll_Lock();
}

void CMainGame::Render(void)
{
	
	HDC hBackBuffer = CBmpMgr::Get_Instance()->Find_Image(L"Back");
	
	CSceneMgr::Get_Instance()->Render(hBackBuffer);
	BitBlt(m_DC, 0, 0, WINCX, WINCY, hBackBuffer, 0, 0, SRCCOPY);
	
#ifdef _DEBUG
	// FPS 출력
	++m_iFPS;

	if (m_dwTime + 1000 < GetTickCount())
	{
		wsprintf(m_szFPS, L"FPS : %d", m_iFPS);
		SetWindowText(g_hWnd, m_szFPS);	// 윈도우 타이틀에 문자열을 출력하는 함수

		m_iFPS = 0;
		m_dwTime = GetTickCount();
	}
#endif
}

void CMainGame::Release(void)
{
	CSoundMgr::Get_Instance()->Destroy_Instance();
	CBmpMgr::Get_Instance()->Destroy_Instance();
	CScrollMgr::Get_Instance()->Destroy_Instance();
	CSceneMgr::Get_Instance()->Destroy_Instance();
	CKeyMgr::Get_Instance()->Destroy_Instance();
	CLineMgr::Get_Instance()->Destroy_Instance();
	CTileMgr::Get_Instance()->Destroy_Instance();
	
	m_pObjMgr->Destroy_Instance();
	m_pObjMgr = nullptr;
	ReleaseDC(g_hWnd, m_DC);
}

void CMainGame::KeyInput()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_F1))
	{
		CObj::g_fSound -= 0.1f;

		if (CObj::g_fSound > 1.f)
			CObj::g_fSound = 1.f;

		CSoundMgr::Get_Instance()->SetChannelVolume(SOUND_BGM, CObj::g_fSound);
	}

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
	{
		CObj::g_fSound += 0.1f;

		if (CObj::g_fSound > 1.f)
			CObj::g_fSound = 1.f;

		CSoundMgr::Get_Instance()->SetChannelVolume(SOUND_BGM, CObj::g_fSound);
	}
}

void CMainGame::InitializeSound()
{
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/MAIN_MENU_BGM.mp3", SOUND_BGM, BGM_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/SELECT_MENU.mp3", SOUND_UI, UI_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/teleport.mp3", SOUND_EFFECT, EFFECT_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/DUNGEON_BGM.mp3", SOUND_BGM, BGM_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/CLICK_MENU.mp3", SOUND_UI, UI_VOLUME);
}
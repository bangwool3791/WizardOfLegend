#include "stdafx.h"
#include "MyEdit.h"
#include "TileMgr.h"
#include "BmpMgr.h"
#include "ChildEditer.h"
#include "EventEditor.h"
#include "UserHeader.h"
#include "SceneMgr.h"

CMyEdit::CMyEdit():m_eSys(SYS_TILE_TYPE),m_pMouseRect(nullptr), m_bMousePress(false)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
	m_hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
}


CMyEdit::~CMyEdit()
{
	Release();
}

void CMyEdit::Initialize(void)
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/FireTileSet_1024.bmp", L"Tile");

	CTileMgr::Get_Instance()->Initialize();
	m_pChildEditer = new CChildEditer();
	m_pChildEditer->Initialize();
	m_pEventEditor = new CEventEditor();
	m_pEventEditor->Initialize();

	m_pMouseRect = new RECT;
	ZeroMemory(m_pMouseRect, sizeof(RECT));
	CObjMgr::Get_Instance()->Initialize(OBJ_KEY);
	CObjMgr::Get_Instance()->AddObject(OBJ_KEY, OBJ_MOUSE, CAbstactFactory<CMouse>::Create());
}

void CMyEdit::Update(void)
{
	POINT	ptMouse{};

	GetCursorPos(&ptMouse);	// 마우스 커서의 위치 좌표를 얻어오는 함수,
	ScreenToClient(g_hWnd, &ptMouse);	// 전체 스크린 영역의 좌표를 우리가 생성한 클라이언트 좌표로 변환
	float fX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fY = CScrollMgr::Get_Instance()->Get_ScrollY();
#if true
#ifdef _USER_DEBUG
	static DWORD dDebugTick = GetTickCount();
	if (dDebugTick + _DEBUG_ACITVE_TICK < GetTickCount())
	{
		dDebugTick = GetTickCount();
	}
#endif
#endif

	Key_Input();
	CTileMgr::Get_Instance()->Update();
	m_pChildEditer->Update();
	m_pEventEditor->Update();
}

void CMyEdit::Late_Update(void)
{
	CTileMgr::Get_Instance()->Late_Update();
}

void CMyEdit::Render(HDC _DC)
{
	CTileMgr::Get_Instance()->Render(_DC);
	m_pChildEditer->Render();

	m_DC = _DC;
	switch (m_eSys)
	{
	case SYS_TILE_TYPE:
		szPint = L"1. 타일타입 2. 타일 3. 타입 4. 장식물타입 5. 장식물 6. 충돌 타일 7. 용암타일 [현재]타입 동시 지정";
		break;
	case SYS_TILE:
		szPint = L"1. 타일타입 2. 타일 3. 타입 4. 장식물타입 5. 장식물 6. 충돌 타일 7. 용암타일 [현재]타일 지정";

		break;
	case SYS_TYPE:
		szPint = L"1. 타일타입 2. 타일 3. 타입 4. 장식물타입 5. 장식물 6. 충돌 타일 7. 용암타일 [현재]타입 지정";
		break;
	case SYS_DECO_TYPE:
		szPint = L"1. 타일타입 2. 타일 3. 타입 4. 장식물타입 5. 장식물 6. 충돌 타일 7. 용암타일 [현재]장식물 타입 동시 지정";
		break;
	case SYS_DECO:
		szPint = L"1. 타일타입 2. 타일 3. 타입 4. 장식물타입 5. 장식물 6. 충돌 타일 7. 용암타일 [현재]장식물 지정";
		break;
	case SYS_COL:
		szPint = L"1. 타일타입 2. 타일 3. 타입 4. 장식물타입 5. 장식물 6. 충돌 타일 7. 용암타일 [현재]충돌 지정";
		break;
	case SYS_LAVA:
		szPint = L"1. 타일타입 2. 타일 3. 타입 4. 장식물타입 5. 장식물 6. 충돌 타일 7. 용암타일 [현재]용암지정";
		break;
	}

	int iX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	m_hOldPen = (HPEN)::SelectObject(_DC, m_hPen);
	MoveToEx(_DC, m_tRect.left + iX, m_tRect.top + iY, nullptr);
	LineTo(_DC, m_tRect.left + iX, m_tRect.bottom + iY);
	LineTo(_DC, m_tRect.right + iX, m_tRect.bottom + iY);
	LineTo(_DC, m_tRect.right + iX, m_tRect.top + iY);
	LineTo(_DC, m_tRect.left + iX, m_tRect.top + iY);

	if (m_pMouseRect)
		Rectangle(_DC, m_pMouseRect->left, m_pMouseRect->top, m_pMouseRect->right, m_pMouseRect->bottom);

	TextOut(_DC, 50, 50, szPint, lstrlen(szPint));
}

void CMyEdit::Release(void)
{
	m_hPen = (HPEN)::SelectObject(m_DC, m_hOldPen);
	DeleteObject(m_hPen);
	ReleaseDC(g_hWnd, m_DC);
	Safe_Delete(m_pMouseRect);
	Safe_Delete(m_pChildEditer);
	Safe_Delete(m_pEventEditor);
	CTileMgr::Get_Instance()->Destroy_Instance();
	CObjMgr::Get_Instance()->Release(OBJ_KEY);
	CScrollMgr::Get_Instance()->Destroy_Instance();
}

void CMyEdit::Key_Input()
{
	static DWORD dTick = GetTickCount();

	if (dTick + 100 < GetTickCount())
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing('R'))
		{
			switch (m_eSys)
			{
			case SYS_DECO_TYPE:
			case SYS_DECO:
				if (0 <= CTileMgr::Get_Instance()->Get_DecoSize())
					CTileMgr::Get_Instance()->Delete_Deco();
				break;
			case SYS_COL:
				if (0 <= CTileMgr::Get_Instance()->Get_ColSize())
					CTileMgr::Get_Instance()->Delete_Col();
				break;
			case SYS_LAVA:
				if (0 <= CTileMgr::Get_Instance()->Get_LavaSize())
					CTileMgr::Get_Instance()->Delete_Lava();
				break;
			}
		}
		dTick = GetTickCount();
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('Q'))
	{
		CTileMgr::Get_Instance()->Save_Process();
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('L'))
	{
		CTileMgr::Get_Instance()->Load_Process();
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_F1))
	{
		CTileMgr::Get_Instance()->Load_Process2();
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_F2))
	{
		CTileMgr::Get_Instance()->Save_Process2();
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('1'))
	{
		m_eSys = SYS_TILE_TYPE;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('2'))
	{
		m_eSys = SYS_TILE;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('3'))
	{
		m_eSys = SYS_TYPE;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('4'))
	{
		m_eSys = SYS_DECO_TYPE;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('5'))
	{
		m_eSys = SYS_DECO;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('6'))
	{
		m_eSys = SYS_COL;
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing('7'))
	{
		m_eSys = SYS_LAVA;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('9'))
	{
		CSceneMgr::Get_Instance()->Set_ScenID(SCENE_MENU);
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
		CScrollMgr::Get_Instance()->Set_ScrollX(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
		CScrollMgr::Get_Instance()->Set_ScrollX(-5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
		CScrollMgr::Get_Instance()->Set_ScrollY(5.f);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
		CScrollMgr::Get_Instance()->Set_ScrollY(-5.f);

	if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		POINT	pt;
		GetCursorPos(&pt);
		ScreenToClient(g_hWnd, &pt);

		if (0 <= pt.x && WINCX >= pt.x &&
			0 <= pt.y && WINCY >= pt.y)
		{

			pt.x -= (int)CScrollMgr::Get_Instance()->Get_ScrollX();
			pt.y -= (int)CScrollMgr::Get_Instance()->Get_ScrollY();

			m_tRect.left = TILECX * (pt.x / TILECX);
			m_tRect.top = TILECY * (pt.y / TILECY);
			m_tRect.right = m_tRect.left + TILECX;
			m_tRect.bottom = m_tRect.top + TILECY;

			int iXID = m_pChildEditer->Get_Point().x / CTILECX;
			int iYID = m_pChildEditer->Get_Point().y / CTILECY;
			TILE_OPT iEventIndex = (TILE_OPT)m_pEventEditor->Get_EventIndex();
			switch (m_eSys)
			{
			case SYS_TILE_TYPE:
				CTileMgr::Get_Instance()->Tile_Picking(pt, iXID, iYID, iEventIndex);
				break;
			case SYS_TILE:
				CTileMgr::Get_Instance()->Tile_Picking(pt, iXID, iYID);
				break;
			case SYS_TYPE:
				CTileMgr::Get_Instance()->Tile_Picking(pt, iEventIndex);
				break;
			case SYS_DECO_TYPE:
				CTileMgr::Get_Instance()->Deco_Picking(pt, iXID, iYID, iEventIndex);
				break;
			case SYS_DECO:
				CTileMgr::Get_Instance()->Deco_Picking(pt, iXID, iYID);
				break;
			case SYS_COL:
				CTileMgr::Get_Instance()->Col_Picking(pt);
				break;
			case SYS_LAVA:
				CTileMgr::Get_Instance()->Lava_Picking(pt);
				break;
			}
		}

	}
	
	if (CKeyMgr::Get_Instance()->Key_Pressing('w'))
	{
		switch (m_eSys)
		{
		case SYS_COL:
			POINT	pt;
			GetCursorPos(&pt);
			ScreenToClient(g_hWnd, &pt);
			RECT rc = {};
			rc.left = (LONG)(pt.x - COLLISION_TILECX * 0.5);
			rc.top = (LONG)(pt.y - COLLISION_TILECY * 0.5);
			rc.right = (LONG)(pt.x + COLLISION_TILECX * 0.5);
			rc.bottom = (LONG)(pt.y + COLLISION_TILECY * 0.5);
			CTileMgr::Get_Instance()->Col_Moving(rc, pt);
			break;
		}
	}

	ProcessMouseDrag();
}


void CMyEdit::ProcessMouseDrag()
{
	vector<CObj*>* pvector = nullptr;
	if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
	{ 
		CObjMgr::Get_Instance()->Get_Target(OBJ_KEY, OBJ_MOUSE, nullptr)->Update();
		float fX = CObjMgr::Get_Instance()->Get_Target(OBJ_KEY, OBJ_MOUSE, nullptr)->Get_Info().fX;
		float fY = CObjMgr::Get_Instance()->Get_Target(OBJ_KEY, OBJ_MOUSE, nullptr)->Get_Info().fY;


		if (!m_bMousePress)
		{
			if (!m_pMouseRect)
			{
				m_pMouseRect = new RECT;
			}

			m_pMouseRect->left = (LONG)fX;
			m_pMouseRect->top = (LONG)fY;
			m_bMousePress = true;
		}
		else
		{
			switch (m_eSys)
			{
			case SYS_TILE_TYPE:
			case SYS_TILE:
			case SYS_TYPE:
			case SYS_DECO_TYPE:
			case SYS_DECO:
				m_pMouseRect->right = (LONG)fX;
				m_pMouseRect->bottom = (LONG)fY;
				m_pMouseRect->left -= (LONG)CScrollMgr::Get_Instance()->Get_ScrollX();
				m_pMouseRect->top -= (LONG)CScrollMgr::Get_Instance()->Get_ScrollY();
				m_pMouseRect->right -= (LONG)CScrollMgr::Get_Instance()->Get_ScrollX();
				m_pMouseRect->bottom -= (LONG)CScrollMgr::Get_Instance()->Get_ScrollY();

				for (int i = m_pMouseRect->top; i < m_pMouseRect->bottom; ++i)
				{
					for (int j = m_pMouseRect->left; j < m_pMouseRect->right; ++j)
					{
						int iXID = m_pChildEditer->Get_Point().x / CTILECX;
						int iYID = m_pChildEditer->Get_Point().y / CTILECY;

						TILE_OPT iEventIndex = (TILE_OPT)m_pEventEditor->Get_EventIndex();
						POINT pt = POINT{ j,i };

						switch (m_eSys)
						{
						case SYS_TILE_TYPE:
							CTileMgr::Get_Instance()->Tile_Picking(pt, iXID, iYID, iEventIndex);
							break;
						case SYS_TILE:
							CTileMgr::Get_Instance()->Tile_Picking(pt, iXID, iYID);
							break;
						case SYS_TYPE:
							CTileMgr::Get_Instance()->Tile_Picking(pt, iEventIndex);
							break;
						case SYS_DECO_TYPE:
							CTileMgr::Get_Instance()->Deco_Picking(pt, iXID, iYID, iEventIndex);
							break;
						case SYS_DECO:
							CTileMgr::Get_Instance()->Deco_Picking(pt, iXID, iYID);
							break;
						}
					}
				}
				break;
			case SYS_COL:
			case SYS_LAVA:
				if(SYS_COL == m_eSys)
					pvector = CTileMgr::Get_Instance()->Get_ColVec();
				else if(SYS_LAVA == m_eSys)
					pvector = CTileMgr::Get_Instance()->Get_LavaVec();

				for (vector<CObj*>::iterator iter = (*pvector).begin(); iter != (*pvector).end();)
				{
					if (m_pMouseRect->left- CScrollMgr::Get_Instance()->Get_ScrollX() <= (*iter)->Get_Info().fX &&
						m_pMouseRect->top - CScrollMgr::Get_Instance()->Get_ScrollY() <= (*iter)->Get_Info().fY &&
						m_pMouseRect->right - CScrollMgr::Get_Instance()->Get_ScrollX() >= (*iter)->Get_Info().fX &&
						m_pMouseRect->bottom - CScrollMgr::Get_Instance()->Get_ScrollY() >= (*iter)->Get_Info().fY
						)
					{
						Safe_Delete((*iter));
						iter = (*pvector).erase(iter);
						//float fX = (float)(m_pMouseRect->right - m_pMouseRect->left);
						//float fY = (float)(m_pMouseRect->bottom - m_pMouseRect->top);
						//if (fX < fY)
						//	elem->Set_TilePosX((float)((m_pMouseRect->left + m_pMouseRect->right) * 0.5));
						//else if(fX > fY)
						//	elem->Set_TilePosY(((float)(m_pMouseRect->top + m_pMouseRect->bottom) * 0.5));
					}
					else
						++iter;
				}
				break;
			}
			m_bMousePress = false;
			Safe_Delete(m_pMouseRect);
		}
	}

	if (m_bMousePress)
	{
		CObjMgr::Get_Instance()->Get_Target(OBJ_KEY, OBJ_MOUSE, nullptr)->Update();
		float fX = CObjMgr::Get_Instance()->Get_Target(OBJ_KEY, OBJ_MOUSE, nullptr)->Get_Info().fX;
		float fY = CObjMgr::Get_Instance()->Get_Target(OBJ_KEY, OBJ_MOUSE, nullptr)->Get_Info().fY;
		m_pMouseRect->right = (LONG)fX;
		m_pMouseRect->bottom = (LONG)fY;
	}
	pvector = nullptr;
}


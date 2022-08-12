#include "stdafx.h"
#include "EventEditor.h"
#include "resource.h"
#include "Define.h"
#include <strsafe.h>
#include "UserHeader.h"

HWND g_listBoxhWnd;

LRESULT CALLBACK ListBoxExampleProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
typedef struct
{
	TCHAR achName[MAX_PATH];
	int nEvent;
}TagItem;

TagItem Item[] =
{
	{ TEXT("�̴ϸ�  ǥ�� 1"), 0},
	{ TEXT("������  ����  1"), 1 },
	{ TEXT("���� �̺�Ʈ 1"), 2},
	{ TEXT("���� �̺�Ʈ 2"), 3},
	{ TEXT("���� �̺�Ʈ 3"), 4},
	{ TEXT("���� �̺�Ʈ 4"), 5 },
	{ TEXT("���� �̺�Ʈ 5"), 6 },
	{ TEXT("���� �̺�Ʈ 6"), 7 },
	{ TEXT("���� �̺�Ʈ 7"), 8 },
	{ TEXT("���� �̺�Ʈ 8"), 9 },
	{ TEXT("���� �̺�Ʈ 9"), 10 },
	{ TEXT("���� �̺�Ʈ 10"), 11 },
	{ TEXT("�̺�Ʈ ����"), 12 },
	{ TEXT("���� ����"), 13 },
	{ TEXT("���� ����"), 14 },
	{ TEXT("������ ����"), 15 },
	{ TEXT("�Ʒ��� ����"), 16 },
};

CEventEditor::CEventEditor():m_iSelectedIndex(2)
{

}


CEventEditor::~CEventEditor()
{
	Release();
}

void CEventEditor::Initialize(void)
{
	WNDCLASSA wnd_class = { 0 };
	wnd_class.lpfnWndProc = ListBoxExampleProc;
	wnd_class.hInstance = GetModuleHandle(NULL);
	wnd_class.lpszClassName = "actwnd";

	RegisterClassA(&wnd_class);
	m_ChildhWnd = CreateWindowA(wnd_class.lpszClassName, " ", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0,
		600, 400, NULL, 0, wnd_class.hInstance, NULL);

	m_listBoxhWnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL,
		7, 35, 300, 200, m_ChildhWnd, NULL, wnd_class.hInstance, NULL);

	for (int i = 0; i < ARRAYSIZE(Item); i++)
	{
		int pos = (int)SendMessage(m_listBoxhWnd, LB_ADDSTRING, 0, (LPARAM)Item[i].achName);
		SendMessage(m_listBoxhWnd, LB_SETITEMDATA, pos, (LPARAM)i);
	}

	SetFocus(m_listBoxhWnd);

	m_labelhWnd = CreateWindowExW(WS_EX_CLIENTEDGE,L"STATIC", NULL, WS_CHILD | WS_VISIBLE,
		7, 7, 400, 21, m_ChildhWnd, NULL, wnd_class.hInstance, NULL);

	SetWindowTextW(m_labelhWnd, L"Label:");

	MSG msg = { 0 };
	
	m_DC = GetDC(m_ChildhWnd);
	m_dSysTick = GetTickCount();
}

void CEventEditor::Update()
{
	CObjMgr::Get_Instance()->Update(SYS_KEY);

	m_iSelectedIndex = (int)SendMessage(m_listBoxhWnd, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (m_iSelectedIndex == LB_ERR)
	{
		return;
	}
	else
	{
		if(ARRAYSIZE(Item) > m_iSelectedIndex)
		{
			TCHAR szIndex[100] = {};
			wsprintf(szIndex, L"���� �� �̺�Ʈ : %s", Item[m_iSelectedIndex].achName);
			SetWindowTextW(m_labelhWnd, szIndex);
		}
	}
}

void CEventEditor::Render()
{
	//CObjMgr::Get_Instance()->Render(m_DC, SYS_KEY);
}

void CEventEditor::Release()
{
	DestroyWindow(m_labelhWnd);
	DestroyWindow(m_listBoxhWnd);
	DestroyWindow(m_ChildhWnd);
}
LRESULT CALLBACK ListBoxExampleProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_CREATE:
	{
#if false
		// Add items to list. 
		//HWND hwndList = GetDlgItem(hWnd, IDC_LISTBOX_EXAMPLE);
		for (int i = 0; i < ARRAYSIZE(Roster); i++)
		{
			int pos = (int)SendMessage(g_listBoxhWnd, LB_ADDSTRING, 0,
				(LPARAM)Roster[i].achName);
			// Set the array index of the player as item data.
			// This enables us to retrieve the item from the array
			// even after the items are sorted by the list box.
			SendMessage(g_listBoxhWnd, LB_SETITEMDATA, pos, (LPARAM)i);
		}
		// Set input focus to the list box.
		SetFocus(g_listBoxhWnd);
#endif
		return TRUE;
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hWnd, LOWORD(wParam));
			return TRUE;

		case IDC_LISTBOX_EXAMPLE:
		{
#if false
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				HWND hwndList = GetDlgItem(hWnd, IDC_LISTBOX_EXAMPLE);

				// Get selected index.
				int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

				// Get item data.
				int i = (int)SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0);

				// Do something with the data from Roster[i]
				TCHAR buff[MAX_PATH];
				StringCbPrintf(buff, ARRAYSIZE(buff),
					TEXT("Position: %s\nGames played: %d\nGoals: %d"),
					Roster[i].achPosition, Roster[i].nGamesPlayed,
					Roster[i].nGoalsScored);

				SetDlgItemText(hWnd, IDC_STATIC, buff);
				return TRUE;
			}
			}
#endif
		}
		return TRUE;
		}
	}
	return FALSE;
}

ATOM CEventEditor::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = ListBoxExampleProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_LISTBOX_EXAMPLE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"�̺�Ʈ";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

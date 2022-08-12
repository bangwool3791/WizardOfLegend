#pragma once

class CEventEditor
{
private :

public:
	const int Get_EventIndex() 
	{
		return m_iSelectedIndex;
	}
public : 
	void Initialize(void);
	void Update();
	void Render();
	void Release();
private :
	HDC m_DC;
private :
	HWND m_ChildhWnd;
	HWND m_listBoxhWnd;
	HWND m_labelhWnd;
	HWND m_TextBoxhWnd;
	DWORD m_dSysTick;
	int   m_iSelectedIndex;
	ATOM CEventEditor::MyRegisterClass(HINSTANCE hInstance);
public:
	CEventEditor();
	virtual ~CEventEditor();
};

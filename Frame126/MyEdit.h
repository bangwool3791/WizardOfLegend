#pragma once
#include "Scene.h"

class CEventEditor;
class CChildEditer;
class CMyEdit :
	public CScene
{
private :
	RECT m_tRect;
	HPEN m_hPen;
	HPEN m_hOldPen;
	HDC m_DC;
public :
	enum eEDIT_SYS{SYS_TILE_TYPE, SYS_TILE, SYS_TYPE, SYS_DECO_TYPE, SYS_DECO, SYS_COL, SYS_LAVA};
private:
	eEDIT_SYS	m_eSys;
	TCHAR*		szPint;
public:
	CMyEdit();
	virtual ~CMyEdit();

public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC _DC) override;
	virtual void Release(void) override;
	//¸¶¿ì½º
private :
	void CMyEdit::ProcessMouseDrag();
	bool m_bMousePress;
	RECT* m_pMouseRect;
private:
	void	Key_Input();
	CChildEditer * m_pChildEditer;
	CEventEditor * m_pEventEditor;

};


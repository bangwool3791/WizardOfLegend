#pragma once
#include "Obj.h"
class CMiddleBossHpBar :
	public CObj
{
private :
	HDC m_hHpMemDC;
	HDC m_hMemDC;
public:
	CMiddleBossHpBar();
	virtual ~CMiddleBossHpBar();
public:
	virtual void Initialize(void) override;
	virtual int	 Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
	//HP, MANA
private:
	int			m_iHp;
private:
	const TCHAR* m_pHpKey;
	INFO		m_tHpInfo;
	RECT		m_tHpRect;
	const TCHAR* m_pManaKey;
	void		Update_HpMana_UI();
public:
	void	Process_HpBar(int _iHp);
};


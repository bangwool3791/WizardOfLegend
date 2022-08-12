#pragma once
#include "Obj.h"
class CPlayerBar :
	public CObj
{
public:
	CPlayerBar();
	virtual ~CPlayerBar();
public:
	virtual void Initialize(void) override;
	virtual int	 Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
	//HP, MANA
private :
	int			m_iHp;
private :
	const TCHAR* m_pHpKey;
	INFO		m_tHpInfo;
	RECT		m_tHpRect;
	const TCHAR* m_pManaKey;
	INFO		m_tManaInfo;
	RECT		m_tManaRect;
	void		Update_HpMana_UI();
public :
	void	Process_HpBar(int _iHp);
};


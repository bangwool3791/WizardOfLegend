#pragma once
#include "Obj.h"
class CMouse;
class CInventory :
	public CObj
{
private:
	HDC m_hMemDC;
	HDC m_hMemExDC;
private :
	int m_iMouseCursor = 0;
private :
	DWORD m_dTick = 0;
	int m_iSelectedIndex = 0;
	void SwapInven(INVEN& _Rhs, INVEN& _Lhs);
private :
	INFO m_tIConInfo;
	TCHAR* m_IconPrimaryKey;
private :
	void Update_InvenRect();
public :
	enum STATE_INVEN{SHOW,HIDE, STATE_INVEN_END};
	const STATE_INVEN& Get_InvenState() { return m_eInven; }
	void Set_InvenState(const STATE_INVEN& _eInven) { m_eInven = _eInven; }
private:
	STATE_INVEN m_eInven = STATE_INVEN_END;
private :
	CObj* m_pMouse;
private:
	vector<INVEN>* m_pVecSkill;
public:
	void Set_SkillBar(vector<INVEN>* _vec) 
	{
		m_pVecSkill = _vec;
	}
public:
	virtual void Initialize(void) override;
	virtual int	 Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
public:
	CInventory();
	virtual ~CInventory();
private :
	bool Click_Item();
};


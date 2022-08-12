#pragma once
#include "Obj.h"
#include "Struct.h"
class CIconSKill :
	public CObj
{
public:
	CIconSKill();
	CIconSKill(INVEN* _Inven);
	virtual ~CIconSKill();
private :
	INVEN* m_pInvenIcon;
public :
public:
	virtual void Initialize(void) override;
	virtual int	 Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
private :
	HDC m_hDCIcon;
	HDC m_hDCCoolTime;
public :
	void Set_SKillIcon(PRO_SKILL& _eProSKill, TCHAR* _pSkillImage);
};


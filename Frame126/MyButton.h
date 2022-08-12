#pragma once
#include "Obj.h"
class CMyMenu;
class CMyButton : public CObj
{
public:
	CMyButton();
	CMyButton(float fX, float fY);
	virtual ~CMyButton();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
private :
	static TCHAR* m_pObjectKey;
private :
	CMyMenu * m_pParent;
public :
	void Set_Parent(CMyMenu* _parent) { m_pParent = _parent; }
private:
	int		m_iDrawXID = 0;

};


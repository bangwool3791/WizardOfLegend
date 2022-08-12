#pragma once
#include "Scene.h"
class CObjMgr;
class CMyMenu :
	public CScene
{
private :
	CObjMgr * m_pObjMgr;
public :
	TCHAR* m_PrimaryKey = nullptr;
	void Set_PrimaryKey(TCHAR* _key) { m_PrimaryKey = _key; }
	TCHAR* Get_PrimaryKey() { return m_PrimaryKey; }
public:
	CMyMenu();
	virtual ~CMyMenu();

public:
	virtual void Initialize(void) override;
	virtual void Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC _DC) override;
	virtual void Release(void) override;

};


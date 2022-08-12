#pragma once

#include "Obj.h"
#include "DObjMgr.h"
// 모든 객체를 중재하기 위한 중재자(메디에이터)패턴을 사용 중
// 객체 생성을 제한하여 외부에서 오로지 한 객체 포인터로만 사용 가능한 싱글톤 패턴을 사용 중

class CObjMgr
{
public :
	void CObjMgr::Initialize(const TCHAR * _pkey);
private:
	CObjMgr();
	~CObjMgr();
public:
	CObj*		Get_Player()
	{
		auto elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(OBJ_KEY));

		if (elem != m_ObjMap.end())
		{
			if (elem->second[OBJ_PLAYER].empty())
				return nullptr;

			return elem->second[OBJ_PLAYER].front();
		}
		return nullptr;
	}

	CObj*				Get_Target(const TCHAR* _pkey, int eID, CObj* pObj);
	list<CObj*>*		Get_TargetList(const TCHAR* _pkey, int eID);
public:

	void		AddObject(const TCHAR* _pkey, int eID, CObj* pObj);
	int			Update(const TCHAR* _key);
	void		Late_Update(const TCHAR* _key);
	void		Render(HDC hDC, const TCHAR* _key);
	void		Release(const TCHAR* _key);

	void        Delete_ID(const TCHAR* _key, int eID);
public :
	void		Clear_RenderSort(RENDERID _eRenderID) { m_RenderSort[_eRenderID].clear(); }
private:
	map<const TCHAR* , list<CObj*>*>			m_ObjMap;
	list<CObj*>			m_RenderSort[RENDER_END];
public:
	static CObjMgr*		Get_Instance(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new CObjMgr;
		}

		return m_pInstance;
	}

	static void	Destroy_Instance(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static	CObjMgr*		m_pInstance;


};


#include "stdafx.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "TileMgr.h"

CObjMgr*	CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
{
	//m_ObjMap.emplace(SYS_KEY, new list<CObj*>[SYS_END]);
	//m_ObjMap.emplace(OBJ_KEY, new list<CObj*>[OBJ_END]);
	//m_ObjMap.emplace(MENU_KEY, new list<CObj*>[MENU_END]);
	//m_ObjMap.emplace(SKILL_KEY, new list<CObj*>[SKILLID_END]);
	//m_ObjMap.emplace(UI_KEY, new list<CObj*>[UI_END]);
	//m_ObjMap.emplace(MONSTER_KEY, new list<CObj*>[MONSTER_END]);
	//m_ObjMap.emplace(EFFECT_KEY, new list<CObj*>[EFFECT_END]);
}


CObjMgr::~CObjMgr()
{
	Release(OBJ_KEY);
	Release(UI_KEY);
		//for (auto& elem : m_ObjMap)
		//{
		//	Release(elem.first);
		//}
	m_ObjMap.clear();

}

void CObjMgr::Initialize(const TCHAR * _pkey)
{
	map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_pkey));

	if (elem == m_ObjMap.end())
	{
		if (!lstrcmp(_pkey, OBJ_KEY))
		{
			m_ObjMap.emplace(OBJ_KEY, new list<CObj*>[OBJ_END]);
		}
		else if (!lstrcmp(_pkey, MENU_KEY))
		{
			m_ObjMap.emplace(MENU_KEY, new list<CObj*>[MENU_END]);
		}
		else if (!lstrcmp(_pkey, SYS_KEY))
		{
			m_ObjMap.emplace(SYS_KEY, new list<CObj*>[SYS_END]);
		}
		else if (!lstrcmp(_pkey, SKILL_KEY))
		{
			m_ObjMap.emplace(SKILL_KEY, new list<CObj*>[SKILLID_END]);
		}
		else if (!lstrcmp(_pkey, MONSTER_KEY))
		{
			m_ObjMap.emplace(MONSTER_KEY, new list<CObj*>[MONSTER_END]);
		}
		else if (!lstrcmp(_pkey, UI_KEY))
		{
			m_ObjMap.emplace(UI_KEY, new list<CObj*>[UI_END]);
		}
		else if (!lstrcmp(_pkey, EFFECT_KEY))
		{
			m_ObjMap.emplace(EFFECT_KEY, new list<CObj*>[EFFECT_END]);
		}
	}
}
CObj* CObjMgr::Get_Target(const TCHAR * _pkey,int eID, CObj* pObj)
{
	CObj* pTarget = nullptr;

	float	fDistance = 0.f;

	map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_pkey));


	if (elem == m_ObjMap.end())
		return nullptr;
	else if ((*elem).second[eID].empty())
		return nullptr;
	else
		return (*elem).second[eID].back();
}
list<CObj*>* CObjMgr::Get_TargetList(const TCHAR* _pkey, int eID)
{
	map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_pkey));

	if (elem == m_ObjMap.end())
		return nullptr;

	return &(*elem).second[eID];
}
void CObjMgr::AddObject(const TCHAR* _pkey, int eID, CObj* pObj)
{
	if (nullptr == pObj)
		return;
			
	map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_pkey));

	if (elem == m_ObjMap.end())
		return;

	elem->second[eID].push_back(pObj);
}

int CObjMgr::Update(const TCHAR* _pkey)
{
	map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_pkey));

	if (elem == m_ObjMap.end())
		return OBJ_NOEVENT;
	
	if (!lstrcmp(elem->first, OBJ_KEY))
	{
		for (int i = 0; i < OBJ_END; ++i)
		{
			auto& iter = elem->second[i].begin();

			for (; iter != elem->second[i].end(); )
			{
				int iEvent = (*iter)->Update();

				if (OBJ_DEAD == iEvent)
				{
					Safe_Delete<CObj*>(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
		}
	}
	else if (!lstrcmp(elem->first, MENU_KEY))
	{
		for (int i = 0; i < MENU_END; ++i)
		{

			auto& iter = elem->second[i].begin();

			for (; iter != elem->second[i].end(); )
			{
				int iEvent = (*iter)->Update();

				if (OBJ_DEAD == iEvent)
				{
					Safe_Delete<CObj*>(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
		}
	}
	else if (!lstrcmp(elem->first, SYS_KEY))
	{
		for (int i = 0; i < SYS_END; ++i)
		{

			auto& iter = elem->second[i].begin();

			for (; iter != elem->second[i].end(); )
			{
				int iEvent = (*iter)->Update();

				if (OBJ_DEAD == iEvent)
				{
					Safe_Delete<CObj*>(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
		}
	}
	else if (!lstrcmp(elem->first, SKILL_KEY))
	{
		for (int i = 0; i < SKILLID_END; ++i)
		{

			auto& iter = elem->second[i].begin();

			for (; iter != elem->second[i].end(); )
			{
				int iEvent = (*iter)->Update();

				if (OBJ_DEAD == iEvent)
				{
					Safe_Delete<CObj*>(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
		}
	}
	else if (!lstrcmp(elem->first, MONSTER_KEY))
	{
		for (int i = 0; i < MONSTER_END; ++i)
		{

			auto& iter = elem->second[i].begin();

			for (; iter != elem->second[i].end(); )
			{
				int iEvent = (*iter)->Update();

				if (OBJ_DEAD == iEvent)
				{
					Safe_Delete<CObj*>(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
		}
	}
	else if (!lstrcmp(elem->first, UI_KEY))
	{
		for (int i = 0; i < UI_END; ++i)
		{

			auto& iter = elem->second[i].begin();

			for (; iter != elem->second[i].end(); )
			{
				int iEvent = (*iter)->Update();

				if (OBJ_DEAD == iEvent)
				{
					Safe_Delete<CObj*>(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
		}
	}
	else if (!lstrcmp(elem->first, EFFECT_KEY))
	{
		for (int i = 0; i < EFFECT_END; ++i)
		{

			auto& iter = elem->second[i].begin();

			for (; iter != elem->second[i].end(); )
			{
				int iEvent = (*iter)->Update();

				if (OBJ_DEAD == iEvent)
				{
					Safe_Delete<CObj*>(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
		}
	}
	
	return 0;
}

void CObjMgr::Late_Update(const TCHAR* _pkey)
{

	map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_pkey));

	if (elem == m_ObjMap.end())
	{
		return;
	}

	if (!lstrcmp(elem->first, OBJ_KEY))
	{
		//플레이어와 데코 간의 Y소팅
		for (int i = 0; i < OBJ_END; ++i)
		{
			for (auto& iter : elem->second[i])
			{

				iter->Late_Update();
				RENDERID eRenderiD = (iter)->Get_RenderID();
				m_RenderSort[eRenderiD].push_back(iter);
			}
		}

		//Y소팅을 위한 데코 리스트 추가
		for (auto& iter : *CTileMgr::Get_Instance()->Get_DecoVec())
		{
			RENDERID eRenderiD = (iter)->Get_RenderID();
			m_RenderSort[eRenderiD].push_back(iter);
		}
	}
	else if (!lstrcmp(elem->first, MENU_KEY))
	{
		
		for (int i = 0; i < MENU_END; ++i)
		{
			for (auto& iter : elem->second[i])
			{
				iter->Late_Update();
			}
		}
	}
	else if (!lstrcmp(elem->first, SYS_KEY))
	{

		for (int i = 0; i < SYS_END; ++i)
		{
			for (auto& iter : elem->second[i])
			{
				if (elem->second[i].empty())
					break;

				iter->Late_Update();
			}
		}
	}
	else if (!lstrcmp(elem->first, SKILL_KEY))
	{

		for (int i = 0; i < SKILLID_END; ++i)
		{
			for (auto& iter : elem->second[i])
			{
				iter->Late_Update();
				RENDERID eRenderiD = (iter)->Get_RenderID();
				m_RenderSort[eRenderiD].push_back(iter);
			}
		}
	}
	else if (!lstrcmp(elem->first, MONSTER_KEY))
	{

		for (int i = 0; i < MONSTER_END; ++i)
		{
			for (auto& iter : elem->second[i])
			{
				iter->Late_Update();
			}
		}
	}
	else if (!lstrcmp(elem->first, UI_KEY))
	{

		for (int i = 0; i < UI_END; ++i)
		{
			for (auto& iter : elem->second[i])
			{
				iter->Late_Update();
			}
		}
	}
	else if (!lstrcmp(elem->first, EFFECT_KEY))
	{
		for (int i = 0; i < EFFECT_END; ++i)
		{
			for (auto& iter : elem->second[i])
			{
				iter->Late_Update();
				RENDERID eRenderiD = (iter)->Get_RenderID();
				m_RenderSort[eRenderiD].push_back(iter);
			}
		}
	}
}

template<typename T>
bool		CompareYAscending(T Dest, T Sour)
{
	return Dest->Get_Info().fY > Sour->Get_Info().fY;
}

template<typename T>
bool		CompareYDescending(T Dest, T Sour)
{
	return Dest->Get_Info().fY < Sour->Get_Info().fY;
}


void CObjMgr::Render(HDC hDC, const TCHAR* _pkey)
{
	map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_pkey));

	if(elem==m_ObjMap.end())
		return;

	if (!lstrcmp(elem->first, OBJ_KEY))
	{

		//for (int i = 0; i < RENDER_END; ++i)
		//{
			m_RenderSort[GAMEOBJECT].sort(CompareYAscending<CObj*>);
			for (auto& iter : m_RenderSort[GAMEOBJECT])
			{
				//map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_pkey));

				//if (0 < elem->second->size())
					iter->Render(hDC);
			}

			m_RenderSort[GAMEOBJECT].clear();
		//}
	}
	else if (!lstrcmp(elem->first, MENU_KEY))
	{
		for (int i = 0; i < MENU_END; ++i)
		{
			for (auto& iter : elem->second[i])
				iter->Render(hDC);
		}
	}
	else if (!lstrcmp(elem->first, SYS_KEY))
	{
		for (int i = 0; i < SYS_END; ++i)
		{
			for (auto& iter : elem->second[i])
				iter->Render(hDC);
		}
	}
	else if (!lstrcmp(elem->first, SKILL_KEY))
	{
		//for (int i = 0; i < SKILLID_END; ++i)
		//{
		//	for (auto& iter : elem->second[i])
		//		iter->Render(hDC);
		//}
		m_RenderSort[RENDER_SKILL].sort(CompareYAscending<CObj*>);
		for (auto& iter : m_RenderSort[RENDER_SKILL])
		{
			//map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_pkey));

			//if (0 < elem->second->size())
				iter->Render(hDC);
		}
		m_RenderSort[RENDER_SKILL].clear();
	}
	else if (!lstrcmp(elem->first, MONSTER_KEY))
	{
		for (int i = 0; i < MONSTER_END; ++i)
		{
			for (auto& iter : elem->second[i])
				iter->Render(hDC);
		}
	}
	else if (!lstrcmp(elem->first, UI_KEY))
	{
		for (int i = 0; i < UI_END; ++i)
		{
			for (auto& iter : elem->second[i])
				iter->Render(hDC);
		}
	}
	else if (!lstrcmp(elem->first, EFFECT_KEY))
	{
		m_RenderSort[BACKGROUND].sort(CompareYDescending<CObj*>);
		for (auto& iter : m_RenderSort[BACKGROUND])
		{
			//map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_pkey));

			//if(0 < elem->second->size())
			iter->Render(hDC);
		}
		m_RenderSort[BACKGROUND].clear();

		m_RenderSort[EFFECT].sort(CompareYDescending<CObj*>);
		for (auto& iter : m_RenderSort[EFFECT])
		{
			//map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_pkey));

			//if(0 < elem->second->size())
				iter->Render(hDC);
		}
		m_RenderSort[EFFECT].clear();
	}
}

void CObjMgr::Release(const TCHAR* _pkey)
{
	map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_pkey));
	if (elem == m_ObjMap.end())
		return;

	if (!lstrcmp(elem->first, OBJ_KEY))
	{
		for (int i = 0; i < OBJ_END; ++i)
		{
			for (list<CObj*>::iterator iter = elem->second[i].begin(); iter != elem->second[i].end();)
			{
				if (*iter)
				{
					(*iter)->Release();
					Safe_Delete(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
			elem->second[i].clear();
		}
		Safe_Delete_Array(elem->second);
		m_ObjMap.erase(elem->first);
	}
	else if (!lstrcmp(elem->first, MENU_KEY))
	{
		for (int i = 0; i < MENU_END; ++i)
		{
			for (list<CObj*>::iterator iter = elem->second[i].begin(); iter != elem->second[i].end();)
			{
				if (*iter)
				{
					(*iter)->Release();
					Safe_Delete(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
			elem->second[i].clear();
		}
		Safe_Delete_Array(elem->second);
		m_ObjMap.erase(elem->first);
	}
	else if (!lstrcmp(elem->first, SYS_KEY))
	{
		for (int i = 0; i < SYS_END; ++i)
		{
			for (list<CObj*>::iterator iter = elem->second[i].begin(); iter != elem->second[i].end();)
			{
				if (*iter)
				{
					(*iter)->Release();
					Safe_Delete(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
			elem->second[i].clear();
		}
		Safe_Delete_Array(elem->second);
		m_ObjMap.erase(elem->first);
	}
	else if (!lstrcmp(elem->first, SKILL_KEY))
	{
		for (int i = 0; i < SKILLID_END; ++i)
		{
			for (list<CObj*>::iterator iter = elem->second[i].begin(); iter != elem->second[i].end();)
			{
				if (*iter)
				{
					(*iter)->Release();
					Safe_Delete(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
			elem->second[i].clear();
		}
		Safe_Delete_Array(elem->second);
		m_ObjMap.erase(elem->first);
	}
	else if (!lstrcmp(elem->first, MONSTER_KEY))
	{
		for (int i = 0; i < MONSTER_END; ++i)
		{
			for (list<CObj*>::iterator iter = elem->second[i].begin(); iter != elem->second[i].end();)
			{
				if (*iter)
				{
					(*iter)->Release();
					Safe_Delete(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
			elem->second[i].clear();
		}
		Safe_Delete_Array(elem->second);
		m_ObjMap.erase(elem->first);
	}
	else if (!lstrcmp(elem->first, UI_KEY))
	{
		for (int i = 0; i < UI_END; ++i)
		{
			for (list<CObj*>::iterator iter = elem->second[i].begin(); iter != elem->second[i].end();)
			{
				if (*iter)
				{
					(*iter)->Release();
					Safe_Delete(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
			elem->second[i].clear();
		}
		Safe_Delete_Array(elem->second);
		m_ObjMap.erase(elem->first);
	}
	else if (!lstrcmp(elem->first, EFFECT_KEY))
	{
		for (int i = 0; i < EFFECT_END; ++i)
		{
			for (list<CObj*>::iterator iter = elem->second[i].begin(); iter != elem->second[i].end();)
			{
				if (*iter)
				{
					(*iter)->Release();
					Safe_Delete(*iter);
					iter = elem->second[i].erase(iter);
				}
				else
					++iter;
			}
			elem->second[i].clear();
		}
		Safe_Delete_Array(elem->second);
		m_ObjMap.erase(elem->first);
	}
}

void CObjMgr::Delete_ID(const TCHAR* _key, int eID)
{
	map<const TCHAR*, list<CObj*>*>::iterator elem = find_if(m_ObjMap.begin(), m_ObjMap.end(), CTagFinder(_key));
	if (elem == m_ObjMap.end())
		return;

	list<CObj*>::iterator iter = elem->second[eID].begin();

	for (; iter != elem->second[eID].end();)
	{
		Safe_Delete(*iter);
		iter = elem->second[eID].erase(iter);
	}
	elem->second[eID].clear();
	list<CObj*>().swap(elem->second[eID]);
}

#include "stdafx.h"
#include "SceneMgr.h"

CSceneMgr* CSceneMgr::m_pInstance = nullptr;

CSceneMgr::CSceneMgr()
	: m_pScene(nullptr)
{
}


CSceneMgr::~CSceneMgr()
{
	Release();
}

void CSceneMgr::Scene_Change(SCENEID eScene)
{
	// state 패턴(상태 패턴) : FSM을 기반으로 하는 객체의 상태를 표현(Finite State Machine : 유한 상태 기계)
	// 자신이 취할 수 있는 유한한 개수의 상태를 의미, 그리고 그 중 반드시 단 하나만 상태를 취한다. 
	// 현재 상태에서 특정 조건이 완료되면 다른 상태로 변할 수 있다.

	if (m_pScene)
	{
		// 저장을 하던지 아니면 씬 전환 때마다 삭제하지 말고 특정 조건이 벌어지면 이전 씬을 삭제해라
		//Safe_Delete(m_pScene);
		Release();
	}

	switch (eScene)
	{
	case SCENE_LOGO:
		m_pScene = new CLogo;
		break;

	case SCENE_MENU:
		m_pScene = new CMyMenu;
		break;

	case SCENE_EDIT:
		m_pScene = new CMyEdit;
		break;

	case SCENE_STAGE:
		//m_pScene = new CStage;
		m_pScene = new CStage;
		break;

	case SCENE_STAGE2:
		m_pScene = new CStage2;
		break;

	case SCEN_ENDING:
		m_pScene = new CEnding;
		break;
	}

	m_pScene->Initialize();

}

void CSceneMgr::Update(void)
{
	if (m_ePreScenID != m_eScenID)
	{
		Scene_Change(m_eScenID);
		m_ePreScenID = m_eScenID;
	}
	m_pScene->Update();
}

void CSceneMgr::Late_Update(void)
{
	m_pScene->Late_Update();
}

void CSceneMgr::Render(HDC _DC)
{
	m_pScene->Render(_DC);
}

void CSceneMgr::Release(void)
{
	Safe_Delete(m_pScene);
}

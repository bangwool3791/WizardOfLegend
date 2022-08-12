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
	// state ����(���� ����) : FSM�� ������� �ϴ� ��ü�� ���¸� ǥ��(Finite State Machine : ���� ���� ���)
	// �ڽ��� ���� �� �ִ� ������ ������ ���¸� �ǹ�, �׸��� �� �� �ݵ�� �� �ϳ��� ���¸� ���Ѵ�. 
	// ���� ���¿��� Ư�� ������ �Ϸ�Ǹ� �ٸ� ���·� ���� �� �ִ�.

	if (m_pScene)
	{
		// ������ �ϴ��� �ƴϸ� �� ��ȯ ������ �������� ���� Ư�� ������ �������� ���� ���� �����ض�
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

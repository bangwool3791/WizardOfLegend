#pragma once

#include "Logo.h"
#include "MyMenu.h"
#include "Stage.h"
#include "MyEdit.h"
#include "Stage2.h"
#include "Ending.h"

class CSceneMgr
{
private :
	SCENEID m_eScenID = SCENEID::SCENE_END;
	SCENEID m_ePreScenID = SCENEID::SCENE_END;
public :
	void Set_ScenID(SCENEID eId) { m_eScenID = eId; }
private:
	CSceneMgr();
	~CSceneMgr();

public:
	void		Scene_Change(SCENEID eScene);
	void		Update(void);
	void		Late_Update(void);
	void		Render(HDC _DC);
	void		Release(void);

public:
	static CSceneMgr*		Get_Instance(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new CSceneMgr;
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
	static	CSceneMgr*		m_pInstance;
	CScene*					m_pScene;

};


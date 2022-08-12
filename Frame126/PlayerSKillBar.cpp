#include "stdafx.h"
#include "PlayerSKillBar.h"
#include "UserHeader.h"
#include "IconSKill.h"
CPlayerSKillBar::CPlayerSKillBar()
{

}


CPlayerSKillBar::~CPlayerSKillBar()
{
	Release();
}

void CPlayerSKillBar::Initialize()
{
	m_tInfo.fX = FX_UI_PLAYER_SKILL_BAR;
	m_tInfo.fY = FY_UI_PLAYER_SKILL_BAR;
	m_tInfo.fCX = FCX_UI_PLAYER_SKILL_BAR;
	m_tInfo.fCY = FCY_UI_PLAYER_SKILL_BAR;

	m_pFrameKey = L"UI_SKILLBAR";

	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	m_pVecInven = nullptr;
}

int CPlayerSKillBar::Update()
{
	Process_Skill_Timer();
	Update_Rect();
	return OBJ_NOEVENT;
}

void CPlayerSKillBar::Late_Update()
{

}

void CPlayerSKillBar::Render(HDC hDC)
{
	GdiTransparentBlt(hDC,
		(int)m_tRect.left,
		(int)m_tRect.top,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		m_hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));

	//for (int i = 0; i < INVEN_END; ++i)
	//{
	//	hMemDC = CBmpMgr::Get_Instance()->Find_Image((*m_pVecInven)[i].SKillImagekey);

	//	BitBlt(hDC,
	//		(int)(*m_pVecInven)[i].tSkillRect.left,
	//		(int)(*m_pVecInven)[i].tSkillRect.top,
	//		FCX_UI_SKILL_BAR_ICON,
	//		FCX_UI_SKILL_BAR_ICON,
	//		hMemDC,
	//		0,
	//		0,
	//		SRCCOPY);
	//}

	//for (int i = 0; i < INVEN_END; ++i)
	//{
	//	hMemDC = CBmpMgr::Get_Instance()->Find_Image((*m_pVecInven)[i].CoolTimeImgekey);
	//	BitBlt(hDC, 
	//		(int)(*m_pVecInven)[i].tSkillRect.left,
	//		(int)(*m_pVecInven)[i].tSkillRect.top,
	//		FCX_UI_SKILL_BAR_ICON,
	//		(int)(*m_pVecInven)[i].fCY,
	//		hMemDC, 
	//		0, 
	//		0, 
	//		SRCCOPY);
	//}
}

void CPlayerSKillBar::Release()
{
	m_pVecInven = nullptr;
}

void CPlayerSKillBar::Process_Skill_Timer()
{
	float fCount = 0, fMaxCount = 0;

	for (int i = 0; i < INVEN_END; ++i)
	{
		fCount = (float)(*m_pVecInven)[i].VecSkillElem[(*m_pVecInven)[i].eSkillState].iCount;
		fMaxCount = (float)(*m_pVecInven)[i].VecSkillElem[(*m_pVecInven)[i].eSkillState].iMaxCount;

		if (fCount > fMaxCount)
			continue;

		if ((*m_pVecInven)[i].VecSkillElem[(*m_pVecInven)[i].eSkillState].dTick +
			(*m_pVecInven)[i].VecSkillElem[(*m_pVecInven)[i].eSkillState].dMaxTick
			< GetTickCount())
		{

			if (fCount < fMaxCount)
			{
				++(*m_pVecInven)[i].VecSkillElem[(*m_pVecInven)[i].eSkillState].iCount;
				(*m_pVecInven)[i].fCY = (fMaxCount - fCount) / fMaxCount * FCY_UI_SKILL_BAR_ICON;
				(*m_pVecInven)[i].VecSkillElem[(*m_pVecInven)[i].eSkillState].dTick = GetTickCount();
			}
			else if(fCount == fMaxCount)
			{
				(*m_pVecInven)[i].fCY = (fMaxCount - fCount) / fMaxCount * FCY_UI_SKILL_BAR_ICON;
				(*m_pVecInven)[i].VecSkillElem[(*m_pVecInven)[i].eSkillState].dTick = 0xFFFFFFFE;
			}

		}
	}
}

void CPlayerSKillBar::Set_Inven(vector<INVEN>* _pVecInven) 
{ 
	m_pVecInven = _pVecInven; 

	m_pObjMgr->AddObject(UI_KEY, UI_SKILL_BAR_ICON, CAbstactFactory<CIconSKill>::Create(&(*m_pVecInven)[INVEN_VKL]));
	m_VecIconSkill.push_back(m_pObjMgr->Get_Target(UI_KEY, UI_SKILL_BAR_ICON, nullptr));
	m_pObjMgr->AddObject(UI_KEY, UI_SKILL_BAR_ICON, CAbstactFactory<CIconSKill>::Create(&(*m_pVecInven)[INVEN_SPACE]));
	m_VecIconSkill.push_back(m_pObjMgr->Get_Target(UI_KEY, UI_SKILL_BAR_ICON, nullptr));
	m_pObjMgr->AddObject(UI_KEY, UI_SKILL_BAR_ICON, CAbstactFactory<CIconSKill>::Create(&(*m_pVecInven)[INVEN_Q]));
	m_VecIconSkill.push_back(m_pObjMgr->Get_Target(UI_KEY, UI_SKILL_BAR_ICON, nullptr));
	m_pObjMgr->AddObject(UI_KEY, UI_SKILL_BAR_ICON, CAbstactFactory<CIconSKill>::Create(&(*m_pVecInven)[INVEN_E]));
	m_VecIconSkill.push_back(m_pObjMgr->Get_Target(UI_KEY, UI_SKILL_BAR_ICON, nullptr));
	m_pObjMgr->AddObject(UI_KEY, UI_SKILL_BAR_ICON, CAbstactFactory<CIconSKill>::Create(&(*m_pVecInven)[INVEN_VKR]));
	m_VecIconSkill.push_back(m_pObjMgr->Get_Target(UI_KEY, UI_SKILL_BAR_ICON, nullptr));
	m_pObjMgr->AddObject(UI_KEY, UI_SKILL_BAR_ICON, CAbstactFactory<CIconSKill>::Create(&(*m_pVecInven)[INVEN_R]));
	m_VecIconSkill.push_back(m_pObjMgr->Get_Target(UI_KEY, UI_SKILL_BAR_ICON, nullptr));
}

void CPlayerSKillBar::Set_SKillICon(const INVENTORY& _eInven, PRO_SKILL& _eProSkill, TCHAR* _pSkillImage)
{
	dynamic_cast<CIconSKill*>(m_VecIconSkill[_eInven])->Set_SKillIcon(_eProSkill, _pSkillImage);
}
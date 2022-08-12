#include "stdafx.h"
#include "IconSKill.h"
#include "UserHeader.h"

CIconSKill::CIconSKill()
{
}

CIconSKill::CIconSKill(INVEN* _Inven)
{
	m_pInvenIcon = _Inven;
	m_pInvenIcon->tSkillRect.top = _Inven->tSkillRect.top;
	m_pInvenIcon->tSkillRect.left = _Inven->tSkillRect.left;
	m_pInvenIcon->fCY = _Inven->fCY;
	m_pInvenIcon->SKillImagekey = _Inven->SKillImagekey;
	m_pInvenIcon->CoolTimeImgekey = L"COOLTIME_SHADOW";
}

CIconSKill::~CIconSKill()
{
}
void CIconSKill::Initialize(void)
{
	m_hDCIcon = CBmpMgr::Get_Instance()->Find_Image((*m_pInvenIcon).SKillImagekey);
	m_hDCCoolTime = CBmpMgr::Get_Instance()->Find_Image((*m_pInvenIcon).CoolTimeImgekey);
 }
int CIconSKill::Update(void)
{
	if (lstrcmp(m_pInvenIcon->pPreSKillImage, m_pInvenIcon->SKillImagekey))
	{
		m_hDCIcon = CBmpMgr::Get_Instance()->Find_Image((*m_pInvenIcon).SKillImagekey);
		m_pInvenIcon->pPreSKillImage = m_pInvenIcon->SKillImagekey;
	}
	return OBJ_NOEVENT;
 }
void CIconSKill::Late_Update(void)
{

}
void CIconSKill::Render(HDC hDC)
{

	BitBlt(hDC,
		(int)(*m_pInvenIcon).tSkillRect.left,
		(int)(*m_pInvenIcon).tSkillRect.top,
		FCX_UI_SKILL_BAR_ICON,
		FCX_UI_SKILL_BAR_ICON,
		m_hDCIcon,
		0,
		0,
		SRCCOPY);

	BitBlt(hDC, 
		(int)(*m_pInvenIcon).tSkillRect.left,
		(int)(*m_pInvenIcon).tSkillRect.top,
		FCX_UI_SKILL_BAR_ICON,
		(int)(*m_pInvenIcon).fCY,
		m_hDCCoolTime,
		0, 
		0, 
		SRCCOPY);
 }
void CIconSKill::Release(void)
{

}
//¿©±â
void CIconSKill::Set_SKillIcon(PRO_SKILL& _eProSKill, TCHAR* _pSkillImage)
{
	(*m_pInvenIcon).SKillImagekey =_pSkillImage;
	(*m_pInvenIcon).eSkillState = _eProSKill;
	//m_hDCIcon = CBmpMgr::Get_Instance()->Find_Image((*m_pInvenIcon).SKillImagekey);
	m_hDCCoolTime = CBmpMgr::Get_Instance()->Find_Image((*m_pInvenIcon).CoolTimeImgekey);
}

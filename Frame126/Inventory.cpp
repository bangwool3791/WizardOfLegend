#include "stdafx.h"
#include "Inventory.h"
#include "UserHeader.h"

CInventory::CInventory():m_pMouse(nullptr)
{
	ZeroMemory(&m_tIConInfo, sizeof(INFO));
}


CInventory::~CInventory()
{
}

void CInventory::Initialize()
{
	m_tInfo.fX = FX_UI_INVEN;
	m_tInfo.fY = FY_UI_INVEN;
	m_tInfo.fCX = FCX_UI_INVEN;
	m_tInfo.fCY = FCY_UI_INVEN;

	m_tIConInfo.fCX = FCX_SELECT_ICON;
	m_tIConInfo.fCY = FCY_SELECT_ICON;

	m_pFrameKey = L"INVENTORY";
	m_IconPrimaryKey = L"SELECT_ICON";
	Set_Frame(0, 0, 0, 0);

	m_eInven = HIDE;

	m_pMouse = m_pObjMgr->Get_Target(OBJ_KEY, OBJ_MOUSE, nullptr);

	m_dTick = GetTickCount();
	Update_Rect();
}

int CInventory::Update()
{
	switch (Get_InvenState())
	{
	case HIDE:
		break;
	case SHOW:
		Update_Rect();

		Update_InvenRect();

		if (!Click_Item())
		{
			if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
			{
				if (m_tRect.left <= m_pMouse->Get_Info().fX - m_pMouse->Get_Info().fCX * 0.5 &&
					m_tRect.right >= m_pMouse->Get_Info().fX + m_pMouse->Get_Info().fCX * 0.5 &&
					m_tRect.top <= m_pMouse->Get_Info().fY + m_pMouse->Get_Info().fCY * 0.5 &&
					m_tRect.bottom >= m_pMouse->Get_Info().fY - m_pMouse->Get_Info().fCY * 0.5)
				{
					m_tInfo.fX = m_pMouse->Get_Info().fX;
					m_tInfo.fY = m_pMouse->Get_Info().fY;
				}
			}
		}
		break;
	}
	return OBJ_NOEVENT;
}

void CInventory::Late_Update()
{
	RECT rc = {};
	static DWORD dTick = GetTickCount();

	if (dTick + 50 < GetTickCount())
	{
		for (int i = 0; i < INVEN_END; ++i)
		{
			if (IntersectRect(&rc, &(*m_pVecSkill)[i].tRect, &m_pMouse->Get_Rect()))
				m_iMouseCursor = i;
		}

		dTick = GetTickCount();
	}
}

void CInventory::Render(HDC hDC)
{
	switch (Get_InvenState())
	{
	case HIDE:
		break;
	case SHOW:
		m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

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


		for (int i = 0; i < INVEN_END; ++i)
		{
			m_hMemDC = CBmpMgr::Get_Instance()->Find_Image((*m_pVecSkill)[i].InvenImageKey);

			BitBlt(hDC,
				(int)(*m_pVecSkill)[i].tRect.left,
				(int)(*m_pVecSkill)[i].tRect.top,
				FCX_UI_SKILL_ICON,
				FCY_UI_SKILL_ICON,
				m_hMemDC,
				0,
				0,
				SRCCOPY);
		}


		m_hMemDC = CBmpMgr::Get_Instance()->Find_Image((*m_pVecSkill)[m_iMouseCursor].ImageEX);

		BitBlt(hDC,
			(int)FX_INVEN_EX,
			(int)FY_INVEN_EX,
			(int)FCX_INVEN_EX,
			(int)FCY_INVEN_EX,
			m_hMemDC,
			0,
			0,
			SRCCOPY);


		m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_IconPrimaryKey);

		GdiTransparentBlt(hDC,
			(int)(m_tIConInfo.fX - FCX_SELECT_ICON * 0.5),
			(int)(m_tIConInfo.fY - FCY_SELECT_ICON * 0.5),
			(int)FCX_SELECT_ICON,
			(int)FCY_SELECT_ICON,
			m_hMemDC,
			0,
			0,
			(int)FCX_SELECT_ICON,
			(int)FCY_SELECT_ICON,
			(UINT)RGB(255, 0, 255));
		break;
	}
}

void CInventory::Release()
{

}

void CInventory::Update_InvenRect()
{
	m_tIConInfo.fX = (float)(m_tInfo.fX + (*m_pVecSkill)[m_iMouseCursor].fX + FCX_UI_SKILL_ICON * 0.5);
	m_tIConInfo.fY = (float)(m_tInfo.fY + (*m_pVecSkill)[m_iMouseCursor].fY + FCY_UI_SKILL_ICON * 0.5);

	for (int i = 0; i < INVEN_END; ++i)
	{
		(*m_pVecSkill)[i].tRect.left = (LONG)(m_tInfo.fX + (*m_pVecSkill)[i].fX);
		(*m_pVecSkill)[i].tRect.right = (LONG)((*m_pVecSkill)[i].tRect.left + FCX_UI_SKILL_ICON);
		(*m_pVecSkill)[i].tRect.top = (LONG)(m_tInfo.fY + (*m_pVecSkill)[i].fY);
		(*m_pVecSkill)[i].tRect.bottom = (LONG)((*m_pVecSkill)[i].tRect.top + FCY_UI_SKILL_ICON);
	}
}

bool CInventory::Click_Item()
{
	RECT		rc{};
	static bool bChecked = false;
	if (m_dTick + 80 < GetTickCount())
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON))
		{
			for (int i = 0; i < INVEN_END; ++i)
			{
				if (IntersectRect(&rc, &(*m_pVecSkill)[i].tRect, &m_pMouse->Get_Rect()))
				{
					if (!bChecked)
					{
						m_iSelectedIndex = i;
						bChecked = true;
					}
					else
					{
						if (m_iSelectedIndex != i)
						{
							INVEN tInven{};
							//tInven.InvenImageKey = (TCHAR*)malloc(lstrlen((*m_pVecSkill)[m_iSelectedIndex].InvenImageKey) + 1);
							//memset(tInven.InvenImageKey, 0, sizeof(lstrlen((*m_pVecSkill)[m_iSelectedIndex].InvenImageKey) + 1));
							printf("¹Ù²î±â Àü %ls  %ls ", (*m_pVecSkill)[m_iSelectedIndex].InvenImageKey, (*m_pVecSkill)[i].InvenImageKey);
							SwapInven(tInven, (*m_pVecSkill)[m_iSelectedIndex]);
							SwapInven((*m_pVecSkill)[m_iSelectedIndex], (*m_pVecSkill)[i]);
							SwapInven((*m_pVecSkill)[i], tInven);
							/*			free(tInven.InvenImageKey);*/
							printf("¹Ù²ï ÈÄ %ls  %ls ", (*m_pVecSkill)[m_iSelectedIndex].InvenImageKey, (*m_pVecSkill)[i].InvenImageKey);
							bChecked = false;
							CSoundMgr::Get_Instance()->Set_Sound(L"CHANGE_SKILL.mp3", SOUND_UI, UI_VOLUME);
						}
					}
				}
			}
			m_dTick = GetTickCount();
		}
	}
	return true;
}

void CInventory::SwapInven(INVEN& _Rhs, INVEN& _Lhs)
{
	_Rhs.eSkillState = _Lhs.eSkillState;
	_Rhs.VecSkillElem.swap(_Lhs.VecSkillElem);
	_Rhs.InvenImageKey =_Lhs.InvenImageKey;
	_Rhs.SKillImagekey = _Lhs.SKillImagekey;
	_Rhs.CoolTimeImgekey = _Lhs.CoolTimeImgekey;
	_Rhs.eSKillMotion = _Lhs.eSKillMotion;
	_Rhs.ImageEX = _Lhs.ImageEX;
	_Rhs.fCY = _Lhs.fCY;
}
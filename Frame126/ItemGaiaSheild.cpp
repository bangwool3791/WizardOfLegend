#include "stdafx.h"
#include "ItemGaiaSheild.h"
#include "UserHeader.h"
#include "Player.h"

CItemGaiaSheild::CItemGaiaSheild()
{
}

CItemGaiaSheild::CItemGaiaSheild(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}
CItemGaiaSheild::~CItemGaiaSheild()
{
}


void CItemGaiaSheild::Initialize(void)
{
	m_tInfo.fCX = FCX_GAIA_ARMOR_CARD;
	m_tInfo.fCY = FCY_GAIA_ARMOR_CARD;

	m_pFrameKey = L"GAIA_ARMOR_CARD";
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	
	m_tItem.eInven = INVEN_R;
	m_tItem.eProSKill = PRO_SKILL_GAIA_ARMOR;
	m_tItem.pSkillImage = L"GAIA_ARMOR_SKILLBAR";
	m_tItem.pInvenImage = L"GAIA_ARMOR_ICON";
	m_tItem.pInvenEx = L"GAIA_ARMOR_EX";
	m_dAttackTick = GetTickCount();
	m_eState = STATE_CREATE;
	m_DownCastingPlayer = dynamic_cast<CPlayer*>(m_pObjMgr->Get_Player());
	Update_Rect();
}

int	CItemGaiaSheild::Update(void)
{
	if (m_bDead)
	{

		return OBJ_DEAD;
	}

	if (m_dAttackTick + 2000 < GetTickCount())
	{
		m_eState = STATE_END;
		m_dAttackTick = GetTickCount();
	}
	return OBJ_NOEVENT;
}
void CItemGaiaSheild::Late_Update(void)
{
	Collision_Rect();
}
void CItemGaiaSheild::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();


	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		FCX_GAIA_ARMOR_CARD,
		FCY_GAIA_ARMOR_CARD,
		m_hMemDC,
		0,
		0,
		FCX_GAIA_ARMOR_CARD,
		FCY_GAIA_ARMOR_CARD,
		RGB(255, 0, 255));
}
void CItemGaiaSheild::Release(void)
{

}

void CItemGaiaSheild::Collision_Rect()
{
	RECT		rc{};

	if (IntersectRect(&rc, &(this->Get_Rect()), &(m_DownCastingPlayer->Get_Rect())))
	{
		if (STATE_END == m_eState)
		{
			if (GAIA_ARMOR_GOLD <= m_DownCastingPlayer->Get_Gold())
			{
				m_DownCastingPlayer->Get_Item(m_tItem);
				m_DownCastingPlayer->Set_PlayerGold(-GAIA_ARMOR_GOLD);
				this->Set_Dead();
			}
		}
	}
}
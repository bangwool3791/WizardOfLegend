#include "stdafx.h"
#include "ItemFDragon.h"
#include "UserHeader.h"
#include "Player.h"

CItemFDragon::CItemFDragon()
{
}

CItemFDragon::CItemFDragon(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}
CItemFDragon::~CItemFDragon()
{
}


void CItemFDragon::Initialize(void)
{
	m_tInfo.fCX = FCX_SKILL_CARD;
	m_tInfo.fCY = FCY_SKILL_CARD;

	m_pFrameKey = L"FIRE_DRAGON_CARD";
	m_tItem.eInven = INVEN_VKR;
	m_tItem.eProSKill = PRO_SKILL_FDRAGON;
	m_tItem.pSkillImage = L"FIRE_DRAGON_SKILLBAR";
	m_tItem.pInvenImage = L"FIRE_DRAGON_ICON";
	m_tItem.pInvenEx = L"FIRE_DRAGON_EX";

	m_dAttackTick = GetTickCount();
	m_eState = STATE_CREATE;
	m_DownCastingPlayer = dynamic_cast<CPlayer*>(m_pObjMgr->Get_Player());
	Update_Rect();
}

int	CItemFDragon::Update(void)
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
void CItemFDragon::Late_Update(void)
{
	Collision_Rect();
}
void CItemFDragon::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		FCX_SKILL_CARD,
		FCY_SKILL_CARD,
		hMemDC,
		0,
		0,
		FCX_SKILL_CARD,
		FCY_SKILL_CARD,
		RGB(255, 0, 255));
}
void CItemFDragon::Release(void)
{

}

void CItemFDragon::Collision_Rect()
{
	RECT		rc{};

	if (IntersectRect(&rc, &(this->Get_Rect()), &(m_DownCastingPlayer->Get_Rect())))
	{
		if (STATE_END == m_eState)
		{
			m_DownCastingPlayer->Get_Item(m_tItem);
			this->Set_Dead();
		}
	}
}
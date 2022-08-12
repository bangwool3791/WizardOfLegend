#include "stdafx.h"
#include "ItemPotion.h"
#include "UserHeader.h"
#include "Player.h"

CItemPotion::CItemPotion()
{
}

CItemPotion::CItemPotion(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}
CItemPotion::~CItemPotion()
{
}


void CItemPotion::Initialize(void)
{
	m_tInfo.fCX = FCX_ITEM_POTION;
	m_tInfo.fCY = FCY_ITEM_POTION;

	m_pFrameKey = L"POTION";

	m_dAttackTick = GetTickCount();
	m_eState = STATE_CREATE;
	m_DownCastingPlayer = dynamic_cast<CPlayer*>(m_pObjMgr->Get_Player());
	Update_Rect();
}

int	CItemPotion::Update(void)
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
void CItemPotion::Late_Update(void)
{
	Collision_Rect();
}
void CItemPotion::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		FCX_ITEM_POTION,
		FCY_ITEM_POTION,
		hMemDC,
		0,
		0,
		FCX_ITEM_POTION,
		FCY_ITEM_POTION,
		RGB(255, 0, 255));
}
void CItemPotion::Release(void)
{

}

void CItemPotion::Collision_Rect()
{
	RECT		rc{};

	if (IntersectRect(&rc, &(this->Get_Rect()), &(m_DownCastingPlayer->Get_Rect())))
	{
		if (STATE_END == m_eState)
		{
			if (ITEM_POTIN_GOLD <= m_DownCastingPlayer->Get_Gold())
			{
				m_DownCastingPlayer->Set_PlayerGold(-ITEM_POTIN_GOLD);
				CSoundMgr::Get_Instance()->Set_Sound(L"GET_SKILL.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
				m_DownCastingPlayer->Process_Potion(PLAYER_HP);
				this->Set_Dead();
			}
		}
	}
}
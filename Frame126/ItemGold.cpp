#include "stdafx.h"
#include "ItemGold.h"
#include "UserHeader.h"
#include "Player.h"

CItemGold::CItemGold()
{
}

CItemGold::CItemGold(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}
CItemGold::~CItemGold()
{
}


void CItemGold::Initialize(void)
{
	m_tInfo.fCX = FCX_ITEM_GOLD;
	m_tInfo.fCY = FCY_ITEM_GOLD;

	m_pFrameKey = L"MONEY";
	m_dAttackTick = GetTickCount();
	m_eState = STATE_CREATE;
	m_pPlayer = m_pObjMgr->Get_Player();
	m_DownCastingPlayer = dynamic_cast<CPlayer*>(m_pPlayer);
	int iX = rand() % 2;
	int iY = rand() % 3;
	Set_Frame(iX, 0xFFFF, iY, iX);
	Set_Gold(rand() % 6 + 1);
	m_fSpeed = 5.f;
	Update_Rect();
}

int	CItemGold::Update(void)
{
	float m_fR = 0.f;
	float m_fAngle = 0.f;
	Get_PlayerInfo(&m_fR, &m_fAngle);
	if (m_bDead)
		return OBJ_DEAD;
	switch (Get_State())
	{
	case STATE_CREATE:
		if (m_dAttackTick + 1000 < GetTickCount())
		{
			m_eState = STATE_END;
			m_dAttackTick = GetTickCount();
		}

		break;
	case STATE_END:
		m_tInfo.fX += m_fSpeed * cosf(m_fAngle);
		m_tInfo.fY -= m_fSpeed * sinf(m_fAngle);
		break;
	}
	Update_Rect();
	return OBJ_NOEVENT;
}
void CItemGold::Late_Update(void)
{
	Collision_Rect();
}
void CItemGold::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		m_tRect.left + iScrollX,
		m_tRect.top + iScrollY,
		FCY_ITEM_GOLD,
		FCY_ITEM_GOLD,
		hMemDC,
		0,
		0,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		RGB(255, 0, 255));
}
void CItemGold::Release(void)
{

}

void CItemGold::Collision_Rect()
{
	RECT		rc{};

	if (m_pPlayer->Get_Rect().left <= m_tInfo.fX  &&
		m_pPlayer->Get_Rect().top <= m_pPlayer->Get_Info().fY &&
		m_pPlayer->Get_Rect().right >= m_tInfo.fX  &&
		m_pPlayer->Get_Rect().bottom >= m_tInfo.fY)
	{
		m_DownCastingPlayer->Set_PlayerGold(m_iGold);
		CSoundMgr::Get_Instance()->Set_Sound(L"MONEY_TAKE.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
		Set_Dead();
	}
}
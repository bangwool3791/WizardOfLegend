#include "stdafx.h"
#include "SwordMan.h"
#include "UserHeader.h"
#include "Slash.h"
#include "ItemIceCrystal.h"
#include "ItemGold.h"
#include "ItemFDragon.h"
#include "ItemIceBlast.h"

CSwordMan::CSwordMan()
{
}

CSwordMan::CSwordMan(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CSwordMan::~CSwordMan()
{
}

void CSwordMan::Initialize(void)
{

	m_tInfo.fCX = (float)FCX_SWORD_MONSTER_CARD;
	m_tInfo.fCY = (float)FCY_SWORD_MONSTER_CARD;
	m_pFrameKey = L"SWORDMAN_CARD";
	Set_State(STATE_CREATE);

	m_pPlayer = m_pObjMgr->Get_Player();

	Set_Frame(0, 150, 0, 32);
	m_fSpeed = 5.f;

	Set_Hp(SWORD_MAN_HP);
	Update_Rect();
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	CSoundMgr::Get_Instance()->Set_Sound(L"CARD_SUMMON.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);

	m_eGroup = GAMEOBJECT;

	m_dMoveTick = GetTickCount();
	//m_pPlayer = CObjMgr::Get_Instance()->
}

int	CSwordMan::Update(void)
{
	static int iSoundIndex = 0;
	if (InCrease_X_Frame(m_tFrame))
	{
		float fR = 0.f;
		float fAngle = 0.f;

		switch (Get_State())
		{
		case STATE_CREATE:
			m_eMoveState = IDLE;
			m_eIdleState = IDLE_DOWN;
			m_tInfo.fCX = (float)FCX_SWORD_MONSTER;
			m_tInfo.fCY = (float)FCY_SWORD_MONSTER;
			Set_State(STATE_MOVE);
			break;
		case STATE_ATTACK:
			CSoundMgr::Get_Instance()->Set_Sound(L"SWORDMAN_ATTACK.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME_BACK);
			break;
		case STATE_MOVE:
		case STATE_DETECT:
			if (0 == iSoundIndex)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"SWORDMAN_RUN_1.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME_BACK);
				iSoundIndex = 1;
			}
			else
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"SWORDMAN_RUN_2.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME_BACK);
				iSoundIndex = 0;
			}
			break;
		case STATE_STUN:
			if (0 >= Get_Hp())
				Process_Dead();
			else
			{
				if (m_dStunTick + MONSTER_STUN_TICK < GetTickCount())
					Clear_Stun();
			}
			break;
		case STATE_DEAD:
			switch (Get_DeadEvent())
			{
			case DEAD_EVENT_CRYSTAL:
				m_pObjMgr->AddObject(UI_KEY, UI_ITEM, CAbstactFactory<CItemIceCrystal>::Create(m_tInfo.fX, m_tInfo.fY));
				break;
			case DEAD_EVENT_FDRAGONE:
				m_pObjMgr->AddObject(UI_KEY, UI_ITEM, CAbstactFactory<CItemFDragon>::Create(m_tInfo.fX, m_tInfo.fY));
				break;
			case DEAD_EVENT_ICEBLAST:
				m_pObjMgr->AddObject(UI_KEY, UI_ITEM, CAbstactFactory<CItemIceBlast>::Create(m_tInfo.fX, m_tInfo.fY));
				break;
			case DEAD_EVENT_GOLD:
				m_pObjMgr->AddObject(UI_KEY, UI_ITEM, CAbstactFactory<CItemGold>::Create(m_tInfo.fX, m_tInfo.fY));
				break;
			}
			return OBJ_DEAD;
		}
	}
	switch (m_eState)
	{
	case STATE_MOVE:
	case STATE_DETECT:
		if (m_eMovePreState != m_eMoveState)
		{
			Update_Frame_Move();
			m_eMovePreState = m_eMoveState;
		}
		if (STATE_MOVE == m_eState)
			ProcessMove_Detect_Player();
		else if(STATE_DETECT == m_eState)
			ProcessAttck_Detect_Player();
		break;
	case STATE_ATTACK:
		if (m_ePreAttackState != m_eAttackState)
		{
			Update_AttackFrame();
			m_ePreAttackState = m_eAttackState;

		}
		if (m_dAttackTick + SWORD_MAN_ATTACK_TICK < GetTickCount())
		{
			ProcessAttack_SwordMan();
			m_dAttackTick = GetTickCount();
		}
		break;
	case STATE_STUN:
	case STATE_DEAD:
		if (m_ePreDamageState != m_eDamageState)
		{
			Update_Frame_Damage(m_eDamageState);
			m_ePreDamageState = m_eDamageState;
		}
		break;
	}
	Set_PreState(m_eState);

	Update_Rect();

	return OBJ_NOEVENT;
}

void CSwordMan::Late_Update(void)
{
	//Collision_RectEx(this, m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN));
}

void CSwordMan::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (lstrcmp(m_pPrePrimaryKey, m_pFrameKey))
	{
		m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
		m_pPrePrimaryKey = m_pFrameKey;
	}

	GdiTransparentBlt(hDC,
		(int)(m_tRect.left + iScrollX),
		(int)(m_tRect.top + iScrollY),
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		m_hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));
}

void CSwordMan::Release(void)
{

}

void CSwordMan::ProcessMove_Detect_Player()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);
	if (m_dMoveTick + SWORD_MAN_MOVE_TICK < GetTickCount())
	{
		if (SWORD_MAN_PLAYER_DETECT < fR)
		{
			m_eMoveState = IDLE;
		}
		else if (SWORD_MAN_PLAYER_DETECT >= fR)
		{
			Set_State(STATE_DETECT);
		}
	}

}

void CSwordMan::Update_Frame_Move()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	switch (m_eMoveState)
	{
	case IDLE:
		if (SWORD_MAN_PLAYER_DETECT < fR)
		{
			if (90 >= fAngle || 270 <= fAngle)
			{
				m_pFrameKey = L"SWORDMAN_RIGHT";
				Set_Frame(0, 100, 0, 0);
			}
			else
			{
				m_pFrameKey = L"SWORDMAN_LEFT";
				Set_Frame(0, 100, 0, 0);
			}
		}
		break;
	case MOVE_LEFT:
	case MOVE_RIGHT:
		if (SWORD_MAN_PLAYER_DETECT >= fR && SWORD_MAN_PLAYER_ATTCK < fR)
		{
			if (90 >= fAngle || 270 <= fAngle)
			{
				m_pFrameKey = L"SWORDMAN_RIGHT";
				Set_Frame(0, 100, 1, 5);
			}
			else
			{
				m_pFrameKey = L"SWORDMAN_LEFT";
				Set_Frame(0, 100, 1, 5);
			}
		}
		break;
	}


}

void CSwordMan::ProcessAttck_Detect_Player()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	if (90 >= fAngle || 270 <= fAngle)
		m_eMoveState = MOVE_RIGHT;
	else
		m_eMoveState = MOVE_LEFT;

	if (SWORD_MAN_PLAYER_DETECT < fR)
	{
		m_eState = STATE_MOVE;
		m_dMoveTick = GetTickCount();
	}
	if (SWORD_MAN_PLAYER_DETECT >= fR && SWORD_MAN_PLAYER_ATTCK < fR)
	{
		m_tInfo.fX += m_fSpeed * cosf(fAngle * PI / 180);
		m_tInfo.fY -= m_fSpeed * sinf(fAngle * PI / 180);
	}
	else if(SWORD_MAN_PLAYER_ATTCK >= fR)
	{
		m_dAttackTick = GetTickCount();
		m_eState = STATE_ATTACK;
		m_eAttackState = ACTIVE;
		m_ePreAttackState = UNACTIVE;
	}
	//else if (SWORD_MAN_PLAYER_DETECT < fR)
}

void CSwordMan::Update_AttackFrame()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	if (SWORD_MAN_PLAYER_ATTCK >= fR && 0 <= fR)
	{
		if (90 >= fAngle || 270 <= fAngle)
		{
			m_pFrameKey = L"SWORDMAN_RIGHT";
			Set_Frame(0, 1000, 2, 1);
		}
		else
		{
			m_pFrameKey = L"SWORDMAN_LEFT";
			Set_Frame(0, 1000, 2, 1);
		}
	}
}
void CSwordMan::Get_PlayerInfo(float * _fR, float* _fAngle)
{
	float fX = Displacement<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX);
	float fY = Displacement<float>(m_pPlayer->Get_Info().fY, m_tInfo.fY);
	*_fR = Distance<float>(fX, fY);
	*_fAngle = Angle<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX,  m_tInfo.fY, m_pPlayer->Get_Info().fY);
	*_fAngle = *_fAngle * 180 / PI;
	*_fAngle = (float)((int)*_fAngle % 360);
}

void CSwordMan::ProcessAttack_SwordMan()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	//if (SWORD_MAN_PLAYER_DETECT < fR)
	//{
	//	m_eState = STATE_MOVE;
	//	m_eMoveState = IDLE;
	//	m_eAttackState = UNACTIVE;
	//}
	//else if (SWORD_MAN_PLAYER_DETECT >= fR && SWORD_MAN_PLAYER_ATTCK < fR)
	//{
	//	m_eState = STATE_DETECT;
	//	//m_eMovePreState = MOVE_END;

	//	if (90 >= fAngle || 270 <= fAngle)
	//		m_eMoveState = MOVE_RIGHT;
	//	else
	//		m_eMoveState = MOVE_LEFT;

	//	m_eMovePreState = MOVE_END;
	//}
	//else
	//{
		Get_PlayerInfo(&fR, &fAngle);
		m_pObjMgr->AddObject(MONSTER_KEY, SOWRDMAN_ATTCK, CAbstactFactory<CSlash>::Create((float)(m_tInfo.fX + m_tInfo.fCX * 0.5 *cosf(fAngle / 180 * PI)), 
			float(m_tInfo.fY - m_tInfo.fCY * 0.5 * sinf(fAngle / 180 * PI))));
		if (315 < fAngle || 45 >= fAngle)
			m_pObjMgr->Get_Target(MONSTER_KEY, SOWRDMAN_ATTCK, nullptr)->Set_Frame(0, 100, 1, 3);
		else if (45 < fAngle && 90 >= fAngle)
			m_pObjMgr->Get_Target(MONSTER_KEY, SOWRDMAN_ATTCK, nullptr)->Set_Frame(0, 100, 2, 3);
		else if (90 < fAngle && 135 >= fAngle)
			m_pObjMgr->Get_Target(MONSTER_KEY, SOWRDMAN_ATTCK, nullptr)->Set_Frame(0, 100, 3, 3);
		else if (135 < fAngle && 225 >= fAngle)
			m_pObjMgr->Get_Target(MONSTER_KEY, SOWRDMAN_ATTCK, nullptr)->Set_Frame(0, 100, 0, 3);
		else if (225 < fAngle && 270 >= fAngle)
			m_pObjMgr->Get_Target(MONSTER_KEY, SOWRDMAN_ATTCK, nullptr)->Set_Frame(0, 100, 4, 3);
		else
			m_pObjMgr->Get_Target(MONSTER_KEY, SOWRDMAN_ATTCK, nullptr)->Set_Frame(0, 100, 5, 3);

		m_eState = STATE_MOVE;
		m_eMoveState = IDLE;
		m_eAttackState = UNACTIVE;
		m_eMovePreState = MOVE_END;
		m_dMoveTick = GetTickCount();
	//}
}

void CSwordMan::Update_Frame_Damage(const DAMAGE_STATE& _eState)
{
	switch (_eState)
	{
	case DAMAGE_STATE_STUNED:
		Set_Frame(0, 50, 3, 1);
		break;
	case DAMAGE_STATE_DEAD:
		Set_Frame(0, 100, 4, 5);
	default:
		break;
	}
}

void CSwordMan::Clear_Stun()
{
	Set_State(STATE_MOVE);
	Clear_Move_PreState();
	Clear_Damage_PreState();
}

void CSwordMan::Process_Dead()
{
	Set_State(STATE_DEAD);
	Set_DamageState(DAMAGE_STATE_DEAD);
}


void CSwordMan::Collision_RectEx(CObj* _Dest, list<CObj*>* _Sour)
{
	RECT		rc{};

	for (auto& Sour : *_Sour)
	{
		if ((int)_Dest == (int)Sour)
			continue;

		float	fX = 0.f, fY = 0.f;

		if (CCollisionMgr::Check_Rect(_Dest, Sour, &fX, &fY))
		{
			if (fX > fY)	// 상하 충돌의 경우
			{
				// 상 충돌
				if (Sour->Get_Info().fY < _Dest->Get_Info().fY)
				{
					Sour->Set_PosY(-fY);
				}
				else // 하 충돌
				{
					Sour->Set_PosY(fY);
				}
			}
			else			// 좌우 충돌의 경우
			{
				// 상 충돌
				if (Sour->Get_Info().fX < _Dest->Get_Info().fX)
				{
					Sour->Set_PosX(-fX);
				}
				else // 하 충돌
				{
					Sour->Set_PosX(fX);
				}
			}
		}
	}
}

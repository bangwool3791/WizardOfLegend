#include "stdafx.h"
#include "ArcherMan.h"
#include "UserHeader.h"
#include "Arrow.h"
#include "ItemIceCrystal.h"
#include "ItemGold.h"
#include "ItemFDragon.h"
#include "ItemIceBlast.h"

CArcherMan::CArcherMan()
{
}

CArcherMan::CArcherMan(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}
CArcherMan::~CArcherMan()
{
}

void CArcherMan::Initialize(void)
{

	m_tInfo.fCX = (float)FCX_ARCHER_MONSTER_CARD;
	m_tInfo.fCY = (float)FCY_ARCHER_MONSTER_CARD;
	m_pFrameKey = L"SUMMON_CARD_ARCHER";
	Set_State(STATE_CREATE);

	m_pPlayer = m_pObjMgr->Get_Player();

	Set_Frame(0, 150, 0, 32);
	m_fSpeed = 5.f;

	Set_Hp(ARCHER_MAN_HP);
	m_dAttackTick = GetTickCount();
	CSoundMgr::Get_Instance()->Set_Sound(L"CARD_SUMMON.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);

	m_eGroup = GAMEOBJECT;
	Update_Rect();

}

int	CArcherMan::Update(void)
{

	if (InCrease_X_Frame(m_tFrame))
	{

		switch (Get_State())
		{
		case STATE_CREATE:
			m_eMoveState = IDLE;
			m_eIdleState = IDLE_DOWN;
			m_tInfo.fCX = (float)FCX_ARCHER_MONSTER;
			m_tInfo.fCY = (float)FCY_ARCHER_MONSTER;
			Set_State(STATE_MOVE);
			break;
		case STATE_ATTACK:
	
			break;
		case STATE_MOVE:
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
	case STATE_READY:
		if (m_eMovePreState != m_eMoveState)
		{
			Update_Frame_Move();
			m_eMovePreState = m_eMoveState;
		}
		if (STATE_MOVE == m_eState)
			ProcessMove();
		else if (STATE_DETECT == m_eState)
			ProcessDetect();
		else if (STATE_READY == m_eState)
		{
			if (m_dArrowTick + 1000 < GetTickCount())
			{
				Clear_AttackState();
				m_eAttackState = ACTIVE;
			}
		}
		break;
	case STATE_ATTACK:
		if (m_ePreAttackState != m_eAttackState)
		{
			Update_AttackFrame();
			m_ePreAttackState = m_eAttackState;

		}
		if (m_dAttackTick + ARCHER_MAN_ATTACK_TICK < GetTickCount())
		{
			ProcessAttack();
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

	list<CObj*>::iterator iter = m_listArrow.begin();

	for (; iter != m_listArrow.end();)
	{
		if (OBJ_DEAD == (*iter)->Update())
		{
			Safe_Delete(*iter);
			iter = m_listArrow.erase(iter);

		}
		else
			++iter;
	}

	Update_Rect();

	return OBJ_NOEVENT;
}

void CArcherMan::Late_Update(void)
{
	for (auto& elem : m_listArrow)
		elem->Late_Update();
}

void CArcherMan::Render(HDC hDC)
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

	for (auto& elem : m_listArrow)
	{
		elem->Render(hDC);
	}
}

void CArcherMan::Release(void)
{
	for (auto& elem : m_listArrow)
	{
		if (m_listArrow.empty())
			break;
		Safe_Delete(elem);
	}
	m_listArrow.clear();
}

void CArcherMan::ProcessMove()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);
	//플레이어 위치 알수 있음
	if (ARCHER_MAN_PLAYER_DETECT >= fR)
	{
		Clear_DetectState();
	}
}

void CArcherMan::Update_Frame_Move()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	switch (m_eMoveState)
	{
	case IDLE:
		if (ARCHER_MAN_PLAYER_DETECT < fR || ARCHER_MAN_PLAYER_ATTCK >= fR)
		{
			if (90 >= fAngle || 270 <= fAngle)
			{
				m_pFrameKey = L"ARCHER_RIGHT";
				Set_Frame(0, 100, 0, 0);
			}
			else
			{
				m_pFrameKey = L"ARCHER_LEFT";
				Set_Frame(0, 100, 0, 0);
			}
		}
		break;
	case MOVE_LEFT:
	case MOVE_RIGHT:
		if (ARCHER_MAN_PLAYER_DETECT >= fR && ARCHER_MAN_PLAYER_ATTCK < fR)
		{
			if (90 >= fAngle || 270 <= fAngle)
			{
				m_pFrameKey = L"ARCHER_RIGHT";
				Set_Frame(0, 100, 1, 5);
			}
			else
			{
				m_pFrameKey = L"ARCHER_LEFT";
				Set_Frame(0, 100, 1, 5);
			}
		}
		break;
	}
}

void CArcherMan::ProcessDetect()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	if (90 >= fAngle || 270 <= fAngle)
		m_eMoveState = MOVE_RIGHT;
	else
		m_eMoveState = MOVE_LEFT;

	if (ARCHER_MAN_PLAYER_DETECT < fR)
	{
		m_eState = STATE_MOVE;
		Set_MoveState(IDLE);
	}
	else if (ARCHER_MAN_PLAYER_DETECT >= fR && ARCHER_MAN_PLAYER_ATTCK < fR)
	{
		m_tInfo.fX += m_fSpeed * cosf(fAngle * PI / 180);
		m_tInfo.fY -= m_fSpeed * sinf(fAngle * PI / 180);
	}
	else if (ARCHER_MAN_PLAYER_ATTCK >= fR)
	{
		m_dArrowTick = GetTickCount();
		m_eState = STATE_READY;
		Set_MoveState(IDLE);
	}
	//else if (ARCHER_MAN_PLAYER_DETECT < fR)
}

void CArcherMan::Update_AttackFrame()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	if (ARCHER_MAN_PLAYER_ATTCK >= fR && 0 <= fR)
	{
		if (90 >= fAngle || 270 <= fAngle)
		{
			m_pFrameKey = L"ARCHER_RIGHT";
			Set_Frame(0, 300, 2, 1);
		}
		else
		{
			m_pFrameKey = L"ARCHER_LEFT";
			Set_Frame(0, 300, 2, 1);
		}
	}
}
void CArcherMan::Get_PlayerInfo(float * _fR, float* _fAngle)
{
	float fX = Displacement<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX);
	float fY = Displacement<float>(m_pPlayer->Get_Info().fY, m_tInfo.fY);
	*_fR = Distance<float>(fX, fY);
	*_fAngle = Angle<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX, m_tInfo.fY, m_pPlayer->Get_Info().fY);
	*_fAngle = *_fAngle * 180 / PI;
	*_fAngle = (float)((int)*_fAngle % 360);
}

void CArcherMan::ProcessAttack()
{
		float fR = 0.f;
		float fAngle = 0.f;
		CArrow* pArrow = nullptr;

		Get_PlayerInfo(&fR, &fAngle);
		m_listArrow.push_back(CAbstactFactory<CArrow>::Create(m_tInfo.fX + cosf(fAngle / 180 * PI), m_tInfo.fY - sinf(fAngle / 180 * PI), fAngle / 180 * PI));
		pArrow = dynamic_cast<CArrow*>(m_listArrow.back());
		pArrow->Set_Monster(this);
		if (350 < fAngle || 10 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_RIGHT");
			pArrow->Set_Frame(0, 100, 4, 3, 0);
			pArrow->Set_YFrame(4);
		}
		else if (10 < fAngle && 30 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_RIGHT");
			pArrow->Set_Frame(0, 300, 3, 3, 0);
			pArrow->Set_YFrame(3);
		}
		else if (30 < fAngle && 50 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_RIGHT");
			pArrow->Set_Frame(0, 300, 2, 3, 0);
			pArrow->Set_YFrame(2);
		}
		else if (50 < fAngle && 70 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_RIGHT");
			pArrow->Set_Frame(0, 300, 1, 3, 0);
			pArrow->Set_YFrame(1);
		}
		else if (70 < fAngle && 90 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_RIGHT");
			pArrow->Set_Frame(0, 300, 0, 3, 0);
			pArrow->Set_YFrame(0);
		}
		else if (90 < fAngle && 110 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_LEFT");
			pArrow->Set_Frame(0, 300, 0, 3, 0);
			pArrow->Set_YFrame(0);
		}
		else if (110 < fAngle && 130 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_LEFT");
			pArrow->Set_Frame(0, 300, 1, 3, 0);
			pArrow->Set_YFrame(1);
		}
		else if (130 < fAngle && 150 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_LEFT");
			pArrow->Set_Frame(0, 300, 2, 3, 0);
			pArrow->Set_YFrame(2);
		}
		else if (150 < fAngle && 170 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_LEFT");
			pArrow->Set_Frame(0, 300, 3, 3, 0);
			pArrow->Set_YFrame(3);
		}
		else if (170 < fAngle && 190 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_LEFT");
			pArrow->Set_Frame(0, 300, 4, 3, 0);
			pArrow->Set_YFrame(4);
		}
		else if (190 < fAngle && 210 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_LEFT");
			pArrow->Set_Frame(0, 300, 5, 3, 0);
			pArrow->Set_YFrame(5);
		}
		else if (210 < fAngle && 230 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_LEFT");
			pArrow->Set_Frame(0, 300, 6, 3, 0);
			pArrow->Set_YFrame(6);
		}
		else if (230 < fAngle && 250 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_LEFT");
			pArrow->Set_Frame(0, 300, 7, 3, 0);
			pArrow->Set_YFrame(7);
		}
		else if (250 < fAngle && 270 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_LEFT");
			pArrow->Set_Frame(0, 300, 8, 3, 0);
			pArrow->Set_YFrame(8);
		}
		else if (270 < fAngle && 290 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_RIGHT");
			pArrow->Set_Frame(0, 300, 8, 3, 0);
			pArrow->Set_YFrame(8);
		}
		else if (290 < fAngle && 310 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_RIGHT");
			pArrow->Set_Frame(0, 300, 7, 3, 0);
			pArrow->Set_YFrame(7);
		}
		else if (310 < fAngle && 330 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_RIGHT");
			pArrow->Set_Frame(0, 300, 6, 3, 0);
			pArrow->Set_YFrame(6);
		}
		else if (330 < fAngle && 350 >= fAngle)
		{
			pArrow->Set_FrameKey(L"ARCHER_BOW_RIGHT");
			pArrow->Set_Frame(0, 300, 5, 3, 0);
			pArrow->Set_YFrame(5);
		}
		//화살을 객체를 넣기 까지 1초, 발사하기 까지 1초 뒤 무브상태로 전환

	if (Is_PlayerRange())
		Clear_MoveState();
}

void CArcherMan::Update_Frame_Damage(const DAMAGE_STATE& _eState)
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

void CArcherMan::Clear_Stun()
{
	Set_State(STATE_MOVE);
	Clear_Move_PreState();
	Clear_Damage_PreState();
}

void CArcherMan::Clear_MoveState()
{
	Set_State(STATE_MOVE);
	Set_MoveState(IDLE);
	Clear_Move_PreState();
}
void CArcherMan::Clear_DetectState() 
{
	Set_State(STATE_DETECT);
	Clear_Move_PreState();
}
void CArcherMan::Clear_AttackState()
{
	Set_State(STATE_ATTACK);
	Clear_PreAttackState();
}

void CArcherMan::Process_Dead()
{
	Set_State(STATE_DEAD);
	Set_DamageState(DAMAGE_STATE_DEAD);
}

bool CArcherMan::Is_PlayerRange()
{
	float fR = 0.f, fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	if (ARROW_DISTNACE < fR)
		return true;
	return false;
}
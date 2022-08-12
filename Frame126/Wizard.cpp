#include "stdafx.h"
#include "Wizard.h"
#include "UserHeader.h"
#include "WizardFire.h"
#include "ItemIceCrystal.h"
#include "ItemGold.h"
#include "ItemFDragon.h"
#include "ItemIceBlast.h"

CWizard::CWizard()
{
}

CWizard::CWizard(float _fX, float _fY) :m_iFireBallCount(7), m_fPlayerAngle(0.f), m_fPlayerRadius(0.f), m_fWizardFireAngle(0.f)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CWizard::~CWizard()
{
	Release();
}

void CWizard::Initialize(void)
{

	m_tInfo.fCX = (float)FCX_WIZARD_MONSTER_CARD;
	m_tInfo.fCY = (float)FCY_WIZARD_MONSTER_CARD;
	m_pFrameKey = L"SUMMON_CARD_WIZARD";
	Set_State(STATE_CREATE);

	m_pPlayer = m_pObjMgr->Get_Player();

	Set_Frame(0, 150, 0, 32);
	m_fSpeed = 5.f;

	Set_Hp(10);
	m_dAttackTick = GetTickCount();
	CSoundMgr::Get_Instance()->Set_Sound(L"CARD_SUMMON.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);

	m_eGroup = GAMEOBJECT;
	Update_Rect();
}

int	CWizard::Update(void)
{

	if (InCrease_X_Frame(m_tFrame))
	{

		switch (Get_State())
		{
		case STATE_CREATE:
			m_eMoveState = IDLE;
			m_eIdleState = IDLE_DOWN;
			m_tInfo.fCX = (float)FCX_WIZARD_MONSTER;
			m_tInfo.fCY = (float)FCY_WIZARD_MONSTER;
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
			if (m_dArrowTick + WIZARD_ATTKC_TICK < GetTickCount())
			{
				ProcessReady();
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
		if (m_dAttackTick + 200 < GetTickCount())
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

	Update_Rect();

	return OBJ_NOEVENT;
}

void CWizard::Late_Update(void)
{
}

void CWizard::Render(HDC hDC)
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

void CWizard::Release(void)
{

}

void CWizard::ProcessMove()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);
	//플레이어 위치 알수 있음
	if (WIZARD_MAN_PLAYER_DETECT >= fR)
	{
		Clear_DetectState();
	}
}

void CWizard::Update_Frame_Move()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	switch (m_eMoveState)
	{
	case IDLE:
		if (WIZARD_MAN_PLAYER_DETECT < fR || WIZARD_MAN_PLAYER_ATTCK >= fR)
		{
			if (90 >= fAngle || 270 <= fAngle)
			{
				m_pFrameKey = L"WIZARD_RIGHT";
				Set_Frame(0, 100, 0, 0);
			}
			else
			{
				m_pFrameKey = L"WIZARD_LEFT";
				Set_Frame(0, 100, 0, 0);
			}
		}
		break;
	case MOVE_LEFT:
	case MOVE_RIGHT:
		if (WIZARD_MAN_PLAYER_DETECT >= fR && WIZARD_MAN_PLAYER_ATTCK < fR)
		{
			if (90 >= fAngle || 270 <= fAngle)
			{
				m_pFrameKey = L"WIZARD_RIGHT";
				Set_Frame(0, 100, 1, 5);
			}
			else
			{
				m_pFrameKey = L"WIZARD_LEFT";
				Set_Frame(0, 100, 1, 5);
			}
		}
		break;
	}
}

void CWizard::ProcessDetect()
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
	else if (WIZARD_MAN_PLAYER_DETECT >= fR && WIZARD_MAN_PLAYER_ATTCK < fR)
	{
		m_tInfo.fX += m_fSpeed * cosf(fAngle * PI / 180);
		m_tInfo.fY -= m_fSpeed * sinf(fAngle * PI / 180);
	}
	else if (WIZARD_MAN_PLAYER_ATTCK >= fR)
	{
		m_dArrowTick = GetTickCount();
		m_eState = STATE_READY;
		Set_MoveState(IDLE);


	}
	//else if (ARCHER_MAN_PLAYER_DETECT < fR)
}

void CWizard::ProcessReady()
{
	Get_PlayerInfo(&m_fPlayerRadius, &m_fPlayerAngle);
	m_fWizardFireAngle = m_fPlayerAngle;
}
void CWizard::Update_AttackFrame()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	if (WIZARD_MAN_PLAYER_ATTCK >= fR && 0 <= fR)
	{
		if (90 >= fAngle || 270 <= fAngle)
		{
			m_pFrameKey = L"WIZARD_RIGHT";
			Set_Frame(0, 300, 2, 1);
		}
		else
		{
			m_pFrameKey = L"WIZARD_LEFT";
			Set_Frame(0, 300, 2, 1);
		}
	}
}
void CWizard::Get_PlayerInfo(float * _fR, float* _fAngle)
{
	float fX = Displacement<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX);
	float fY = Displacement<float>(m_pPlayer->Get_Info().fY, m_tInfo.fY);
	*_fR = Distance<float>(fX, fY);
	*_fAngle = Angle<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX, m_tInfo.fY, m_pPlayer->Get_Info().fY);
	*_fAngle = *_fAngle * 180 / PI;
	*_fAngle = (float)((int)*_fAngle % 360);
}

void CWizard::ProcessAttack()
{
	CWizardFire* pWizardBall = nullptr;

	m_pObjMgr->AddObject(SKILL_KEY, WIZARD_FIRE, CAbstactFactory<CWizardFire>::Create(m_tInfo.fX + 100 * cosf(m_fWizardFireAngle / 180 * PI), m_tInfo.fY - 100 * sinf(m_fWizardFireAngle / 180 * PI), m_fWizardFireAngle / 180 * PI));
	pWizardBall = dynamic_cast<CWizardFire*>(m_pObjMgr->Get_Target(SKILL_KEY, WIZARD_FIRE, nullptr));
	pWizardBall->Set_Monster(this);

	int iYFrame = 0;
	if  (0 == m_fPlayerAngle)
		iYFrame = 0;
	else if (22.5 < m_fPlayerAngle && 45 >= m_fPlayerAngle)
		iYFrame = 15;
	else if (45 < m_fPlayerAngle && 67.5 >= m_fPlayerAngle)
		iYFrame = 14;
	else if (67.5 < m_fPlayerAngle && 90 >= m_fPlayerAngle)
		iYFrame = 13;
	else if (112.5 < m_fPlayerAngle && 135 >= m_fPlayerAngle)
		iYFrame = 12;
	else if (135 < m_fPlayerAngle && 157.5 >= m_fPlayerAngle)
		iYFrame = 11;
	else if (157.5 < m_fPlayerAngle && 180 >= m_fPlayerAngle)
		iYFrame = 10;
	else if (180 < m_fPlayerAngle && 202.5 >= m_fPlayerAngle)
		iYFrame = 9;
	else if (202.5 < m_fPlayerAngle && 225 >= m_fPlayerAngle)
		iYFrame = 8;
	else if (225 < m_fPlayerAngle && 247.5 >= m_fPlayerAngle)
		iYFrame = 7;
	else if (247.5 < m_fPlayerAngle && 270 >= m_fPlayerAngle)
		iYFrame = 6;
	else if (270 < m_fPlayerAngle && 292.5 >= m_fPlayerAngle)
		iYFrame = 5;
	else if (292.5 < m_fPlayerAngle && 315 >= m_fPlayerAngle)
		iYFrame = 4;
	else if (315 < m_fPlayerAngle && 337.5 >= m_fPlayerAngle)
		iYFrame = 3;
	else if (337.5 < m_fPlayerAngle && 360 >= m_fPlayerAngle)
		iYFrame = 2;
	else if (0 < m_fPlayerAngle && 22.5 >= m_fPlayerAngle)
		iYFrame = 1;

	pWizardBall->Set_Frame(0, 100, iYFrame, 5);

	if ((0 == Get_FireBallCnt()))
	{
		for (auto & elem : *m_pObjMgr->Get_TargetList(SKILL_KEY, WIZARD_FIRE))
		{
			if(m_pObjMgr->Get_TargetList(SKILL_KEY, WIZARD_FIRE)->empty())
				break;

			elem->Set_State(STATE_ATTACK);
		}
		
		Clear_MoveState();
		Set_FireBallCnt(7);
	}
	else
	{
		if (3 <= Get_FireBallCnt())
		{
			m_fWizardFireAngle += 30;

			if (3 == Get_FireBallCnt())
				m_fWizardFireAngle = m_fPlayerAngle - 30.f;
		}else
			m_fWizardFireAngle -= 30;
	}

	Plus_FireBallCnt(-1);
}

void CWizard::Update_Frame_Damage(const DAMAGE_STATE& _eState)
{
	float fR = 0.f, fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	switch (_eState)
	{
	case DAMAGE_STATE_STUNED:
		if (90 >= fAngle || 270 <= fAngle)
			m_pFrameKey = L"WIZARD_RIGHT";
		else
			m_pFrameKey = L"WIZARD_LEFT";
		Set_Frame(0, 100, 3, 1);
		break;
	case DAMAGE_STATE_DEAD:
		if (90 >= fAngle || 270 <= fAngle)
			m_pFrameKey = L"WIZARD_RIGHT";
		else
			m_pFrameKey = L"WIZARD_LEFT";
		Set_Frame(0, 100, 4, 3);
		break;
	default:
		break;
	}
}

void CWizard::Clear_Stun()
{
	Set_State(STATE_MOVE);
	Clear_Move_PreState();
	Clear_Damage_PreState();
}

void CWizard::Clear_MoveState()
{
	Set_State(STATE_MOVE);
	Set_MoveState(IDLE);
	Clear_Move_PreState();
}
void CWizard::Clear_DetectState()
{
	Set_State(STATE_DETECT);
	Clear_Move_PreState();
}
void CWizard::Clear_AttackState()
{
	Set_State(STATE_ATTACK);
	Clear_PreAttackState();
}

void CWizard::Process_Dead()
{
	Set_State(STATE_DEAD);
	Set_DamageState(DAMAGE_STATE_DEAD);
}

bool CWizard::Is_PlayerRange()
{
	float fR = 0.f, fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	if (WIZARD_FIRE_DISTNACE < fR)
		return true;
	return false;
}
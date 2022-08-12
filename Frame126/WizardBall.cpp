#include "stdafx.h"
#include "WizardBall.h"
#include "UserHeader.h"
#include "ItemIceCrystal.h"
#include "ItemGold.h"
#include "ItemFDragon.h"
#include "ItemIceBlast.h"

CWizardBall::CWizardBall()
{
}

CWizardBall::CWizardBall(float _fX, float _fY):m_iYFrame(0),m_fPlayerAngle(0.f), m_fPlayerRaidus(0.f)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
	m_hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
}
CWizardBall::~CWizardBall()
{
	Release();
}

void CWizardBall::Initialize(void)
{

	m_tInfo.fCX = (float)FCX_WIZARD_BALL_CARD;
	m_tInfo.fCY = (float)FCY_WIZARD_BALL_CARD;
	m_pFrameKey = L"SUMMON_CARD_WIZARDBALL";
	Set_State(STATE_CREATE);

	m_pPlayer = m_pObjMgr->Get_Player();

	Set_Frame(0, 150, 0, 32);
	m_fSpeed = 8.f;

	Set_Hp(15);
	m_dAttackTick = GetTickCount();
	Set_State(STATE_CREATE);
	CSoundMgr::Get_Instance()->Set_Sound(L"CARD_SUMMON.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
	m_eGroup = GAMEOBJECT;
	Update_Rect();
}

int	CWizardBall::Update(void)
{
	if (InCrease_X_Frame(m_tFrame))
	{
		switch (Get_State())
		{
		case STATE_CREATE:
			m_eMoveState = IDLE;
			m_eIdleState = IDLE_DOWN;
			m_pFrameKey = L"WIZARD_BALL";
			m_tInfo.fCX = (float)FCX_WIZARD_BALL;
			m_tInfo.fCY = (float)FCY_WIZARD_BALL;
			Set_Frame(0, 300, 0, 0);
			Set_State(STATE_MOVE);
			break;
		case STATE_STUN:
			if (0 >= Get_Hp())
			{
				Process_Dead();
			}
			else
			{
				if (m_dStunTick + 500 < GetTickCount())
					Clear_Stun();
			}
			break;
		case STATE_ATTACK:
			Set_State(STATE_MOVE);
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

	//위자드 볼은 충돌 시 데드
	if (m_bDead)
		return OBJ_DEAD;

	if (m_ePreState != m_eState)
	{
		Update_Frame_Object();
		Set_PreState(m_eState);
	}

	switch (m_eState)
	{
	case STATE_MOVE:
		ProcessMove();
		break;
	case STATE_DETECT:
		ProcessDetect();
		break;
	case STATE_READY:

		if (UNACITVE == m_eAttackState)
		{
			ProcessReady();
			CSoundMgr::Get_Instance()->Set_Sound(L"BALL_ATTACKMODE.mp3", SOUND_BALL, MONSTER_VOLUME);
		}
		else
		{
			if (m_dReadyTick + WIZARD_BALL_ATTACK_TICK < GetTickCount())
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"BALL_ATTACK.mp3", SOUND_BALL, MONSTER_VOLUME);
				Set_State(STATE_ATTACK);
				/*m_dReadyTick = GetTickCount();*/
			}
		}
		break;
	case STATE_ATTACK:
		ProcessAttack();
		break;
	case STATE_STUN:
	case STATE_DEAD:
		if (m_eMovePreState != m_eMoveState)
		{
			Update_Frame_Object();
			m_eMovePreState = m_eMoveState;
		}
		break;
	}


	Update_Rect();

	return OBJ_NOEVENT;
}

void CWizardBall::Late_Update(void)
{
	Collision_Sphere(this, m_pPlayer);
	Collision_Sphere(this, m_pObjMgr->Get_TargetList(SKILL_KEY, SKILL_GAIA_ARMOR));
}

void CWizardBall::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (lstrcmp(m_pPrePrimaryKey, m_pFrameKey))
	{
		m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
		m_pPrePrimaryKey = m_pFrameKey;
	}
	m_hDC = hDC;

	switch (Get_State())
	{
	case STATE_READY:
		m_hOldPen = (HPEN)::SelectObject(hDC, m_hPen);
		MoveToEx(hDC, (int)m_tInfo.fX + iScrollX, (int)m_tInfo.fY + iScrollY, nullptr);
		LineTo(hDC, (int)(m_tInfo.fX + m_fPlayerRaidus * cosf(m_fPlayerAngle * PI / 180) + iScrollX), (int)(m_tInfo.fY - m_fPlayerRaidus * sinf(m_fPlayerAngle * PI / 180) + iScrollY));
		break;
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

void CWizardBall::Release(void)
{
	m_hPen = (HPEN)::SelectObject(m_hDC, m_hOldPen);
	DeleteObject(m_hPen);
	ReleaseDC(g_hWnd, m_hDC);
}

void CWizardBall::Update_Frame_Object()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	switch (m_eState)
	{
	case STATE_MOVE:
	case STATE_DETECT:
			Set_Frame(0, 300, 0, 0); 
		break;
	case STATE_READY:
		if (ACTIVE == m_eAttackState)
		{
			if (0 == fAngle)
				m_iYFrame = 1;
			else if (22.5 < fAngle && 45 >= fAngle)
				m_iYFrame = 16;
			else if (45 < fAngle && 67.5 >= fAngle)
				m_iYFrame = 15;
			else if (67.5 < fAngle && 90 >= fAngle)
				m_iYFrame = 14;
			else if (112.5 < fAngle && 135 >= fAngle)
				m_iYFrame = 13;
			else if (135 < fAngle && 157.5 >= fAngle)
				m_iYFrame = 12;
			else if (157.5 < fAngle && 180 >= fAngle)
				m_iYFrame = 11;
			else if (180 < fAngle && 202.5 >= fAngle)
				m_iYFrame = 10;
			else if (202.5 < fAngle && 225 >= fAngle)
				m_iYFrame = 9;
			else if (225 < fAngle && 247.5 >= fAngle)
				m_iYFrame = 8;
			else if (247.5 < fAngle && 270 >= fAngle)
				m_iYFrame = 7;
			else if (270 < fAngle && 292.5 >= fAngle)
				m_iYFrame = 6;
			else if (292.5 < fAngle && 315 >= fAngle)
				m_iYFrame = 5;
			else if (315 < fAngle && 337.5 >= fAngle)
				m_iYFrame = 4;
			else if (337.5 < fAngle && 360 >= fAngle)
				m_iYFrame = 3;
			else if (0 < fAngle && 22.5 >= fAngle)
				m_iYFrame = 1;

			Set_Frame(0, 600, m_iYFrame, 2);
		}
		else
			Set_Frame(0, 300, 0, 0);
		break;
	case STATE_ATTACK:

		if (0 == fAngle)
			m_iYFrame = 1;
		else if (22.5 < fAngle && 45 >= fAngle)
			m_iYFrame = 16;
		else if (45 < fAngle && 67.5 >= fAngle)
			m_iYFrame = 15;
		else if (67.5 < fAngle && 90 >= fAngle)
			m_iYFrame = 14;
		else if (112.5 < fAngle && 135 >= fAngle)
			m_iYFrame = 13;
		else if (135 < fAngle && 157.5 >= fAngle)
			m_iYFrame = 12;
		else if (157.5 < fAngle && 180 >= fAngle)
			m_iYFrame = 11;
		else if (180 < fAngle && 202.5 >= fAngle)
			m_iYFrame = 10;
		else if (202.5 < fAngle && 225 >= fAngle)
			m_iYFrame = 9;
		else if (225 < fAngle && 247.5 >= fAngle)
			m_iYFrame = 8;
		else if (247.5 < fAngle && 270 >= fAngle)
			m_iYFrame = 7;
		else if (270 < fAngle && 292.5 >= fAngle)
			m_iYFrame = 6;
		else if (292.5 < fAngle && 315 >= fAngle)
			m_iYFrame = 5;
		else if (315 < fAngle && 337.5 >= fAngle)
			m_iYFrame = 4;
		else if (337.5 < fAngle && 360 >= fAngle)
			m_iYFrame = 3;
		else if (0 < fAngle && 22.5 >= fAngle)
			m_iYFrame = 2;

		Set_Frame(0, 300, m_iYFrame, 3);
		break;

	case STATE_STUN:
		Set_Frame(0, 300, 0, 2);
		break;
	}
}

void CWizardBall::ProcessMove()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);
	//플레이어 위치 알수 있음
	if (WIZARD_BALL_PLAYER_DETECT >= fR)
	{
		Set_State(STATE_DETECT);
		m_eAttackState = UNACITVE;
	}
}

void CWizardBall::ProcessDetect()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	if (WIZARD_BALL_PLAYER_DETECT < fR )
	{
		Set_State(STATE_MOVE);
	}
	else if (WIZARD_BALL_PLAYER_DETECT >= fR)
	{
		Set_State(STATE_READY);
		m_eAttackState = UNACITVE;
	}
}

void CWizardBall::Get_PlayerInfo(float * _fR, float* _fAngle)
{
	float fX = Displacement<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX);
	float fY = Displacement<float>(m_pPlayer->Get_Info().fY, m_tInfo.fY);
	*_fR = Distance<float>(fX, fY);
	*_fAngle = Angle<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX, m_tInfo.fY, m_pPlayer->Get_Info().fY);
	*_fAngle = *_fAngle * 180 / PI;
	*_fAngle = (float)((int)*_fAngle % 360);
}

void CWizardBall::ProcessReady()
{
	float fR = 0.f;
	float fAngle = 0.f;

	Get_PlayerInfo(&fR, &fAngle);

	m_dReadyTick = GetTickCount();
	m_eAttackState = ACTIVE;
	Get_PlayerInfo(&m_fPlayerRaidus, &m_fPlayerAngle);
}

void CWizardBall::ProcessAttack()
{
	float fR = 0.f;
	float fAngle = 0.f;

	Get_PlayerInfo(&fR, &fAngle);

	m_tInfo.fX += 10.f * cosf(m_fPlayerAngle * PI / 180);
	m_tInfo.fY -= 10.f * sinf(m_fPlayerAngle * PI / 180);

	if (0 == m_fPlayerAngle)
		m_iYFrame = 1;
	else if (22.5 < m_fPlayerAngle && 45 >= m_fPlayerAngle)
		m_iYFrame = 16;
	else if (45 < m_fPlayerAngle && 67.5 >= fAngle)
		m_iYFrame = 15;
	else if (67.5 < m_fPlayerAngle && 90 >= m_fPlayerAngle)
		m_iYFrame = 14;
	else if (112.5 < m_fPlayerAngle && 135 >= m_fPlayerAngle)
		m_iYFrame = 13;
	else if (135 < m_fPlayerAngle && 157.5 >= m_fPlayerAngle)
		m_iYFrame = 12;
	else if (157.5 < m_fPlayerAngle && 180 >= m_fPlayerAngle)
		m_iYFrame = 11;
	else if (180 < m_fPlayerAngle && 202.5 >= m_fPlayerAngle)
		m_iYFrame = 10;
	else if (202.5 < m_fPlayerAngle && 225 >= m_fPlayerAngle)
		m_iYFrame = 9;
	else if (225 < m_fPlayerAngle && 247.5 >= m_fPlayerAngle)
		m_iYFrame = 8;
	else if (247.5 < m_fPlayerAngle && 270 >= m_fPlayerAngle)
		m_iYFrame = 7;
	else if (270 < m_fPlayerAngle && 292.5 >= m_fPlayerAngle)
		m_iYFrame = 6;
	else if (292.5 < m_fPlayerAngle && 315 >= m_fPlayerAngle)
		m_iYFrame = 5;
	else if (315 < m_fPlayerAngle && 337.5 >= m_fPlayerAngle)
		m_iYFrame = 4;
	else if (337.5 < m_fPlayerAngle && 360 >= m_fPlayerAngle)
		m_iYFrame = 3;
	else if (0 < m_fPlayerAngle && 22.5 >= m_fPlayerAngle)
		m_iYFrame = 2;
}

void CWizardBall::ProcessStun()
{
	m_dStunTick = GetTickCount();
}

void CWizardBall::Clear_Stun()
{
	Set_State(STATE_MOVE);
}

void CWizardBall::Process_Dead()
{
	Set_State(STATE_DEAD);
	Set_DamageState(DAMAGE_STATE_DEAD);
}
void CWizardBall::Collision_Sphere(CObj* _Dest, CObj* _Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;

	if (CCollisionMgr::Check_Sphere(_Sour, _Dest, &fR, &fAngle))
	{
		if (
			CObj::STATE_CREATE != _Sour->Get_State() &&
			CObj::STATE_STUN != _Sour->Get_State() &&
			CObj::STATE_DEAD != _Sour->Get_State() &&
			CObj::STATE_ATTACK == _Dest->Get_State() && 
			CObj::STATE_SKILL != _Sour->Get_State() &&
			CObj::STATE_CREATE != _Dest->Get_State())
		{
			_Sour->Set_State(CObj::STATE_STUN);
			_Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
			_Sour->Plus_Hp(-_Dest->Get_Attack());

			fAngle = fAngle * 180 / PI;
			fAngle = (float)((int)fAngle % 360);
			CSoundMgr::Get_Instance()->Set_Sound(L"PLAYER_HITED_1.mp3", SOUND_PLAYER_BACK, PLAYER_VOLUME_BACK);
		}
	}
}
void CWizardBall::Collision_Sphere(CObj* _Dest, list<CObj*>* Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;
	if (0 >= Sour->size())
		return;
	for (auto& _Sour : *Sour)
	{
		float fX = Displacement<float>(m_tInfo.fX, _Sour->Get_Info().fX);
		float fY = Displacement<float>(_Sour->Get_Info().fY, m_tInfo.fY);
		float fR = Distance<float>(fX, fY);

		if (100 <= fR)
			continue;
		if (CCollisionMgr::Check_Sphere(_Sour, _Dest, &fR, &fAngle))
		{
			this->Set_Dead();
		}
	}
}
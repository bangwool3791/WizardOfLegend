#include "stdafx.h"
#include "MiddleBoss.h"
#include "UserHeader.h"
#include "WizardFire.h"
#include "FireBlastBullet.h"
#include "MiddleBossShield.h"
#include "MiddleBossSton.h"
#include "MiddleBossHpBar.h"
#include "WizardBall.h"
#include "SwordMan.h"
#include "ArcherMan.h"
#include "Wizard.h"
#include "Shadow.h"

CMiddleBoss::CMiddleBoss()
{
}

CMiddleBoss::CMiddleBoss(float _fX, float _fY) : m_fPlayerAngle(0.f), m_fPlayerRadius(0.f), m_eBossSkill(BOSS_SKILL_STATUE)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CMiddleBoss::~CMiddleBoss()
{
	Release();
}

void CMiddleBoss::Initialize(void)
{

	m_tInfo.fCX = (float)FCX_MIDDLE_BOSS;
	m_tInfo.fCY = (float)FCY_MIDDLE_BOSS;
	m_pFrameKey = L"MIDDLE_BOSS_LEFT";
	Set_State(STATE_MOVE);

	m_pPlayer = m_pObjMgr->Get_Player();

	Set_Frame(0, 1000, 0, 0);
	m_fSpeed = 5.f;

	Set_Hp(MIDDLE_BOSS_HP);

	m_vecSKillElem.reserve(BOSS_SKILL_END);
	for (auto& elem : m_vecSKillElem)
		ZeroMemory(&elem, sizeof(SkillElem));

	m_vecSKillElem.push_back(SkillElem{ 1, 1, 0, 5000 , SKILLID_END, FCX_MIDDLE_BOSS * 0.5 , 0 });
	m_vecSKillElem.push_back(SkillElem{ 7, 7, 0, 1000 , SKILLID_END, FCY_MIDDLE_BOSS * 0.5 , 0});
	m_vecSKillElem.push_back(SkillElem{ 5, 5, 0, 500 , SKILLID_END, 150 , 0 });

	Set_GodMode(GOD_MODE_ACTIVE);

	m_pObjMgr->AddObject(UI_KEY, UI_MIDDLE_BOSS_BAR, CAbstactFactory<CMiddleBossHpBar>::Create());
	m_pBossHpBar = dynamic_cast<CMiddleBossHpBar*>(m_pObjMgr->Get_Target(UI_KEY, UI_MIDDLE_BOSS_BAR, nullptr));
	m_eGroup = GAMEOBJECT;


	m_pShadow = dynamic_cast<CShadow*>(CAbstactFactory<CShadow>::Create((float)(m_tInfo.fX), (float)(m_tInfo.fY + m_tInfo.fCY * 0.5)));
	m_pShadow->Initialize();
	m_pShadow->Set_Target(this);
	m_pShadow->Set_Shawdow(L"BIG_SHADOW", FCX_UI_BIG_SHADOW, FCY_UI_BIG_SHADOW);

	Update_Rect();
}

int	CMiddleBoss::Update(void)
{

	if (InCrease_X_Frame(m_tFrame))
	{

		switch (Get_State())
		{
		case STATE_STUN:
			if (0 >= Get_Hp())
				Process_Dead();
			else
			{
				if (m_dStunTick + MONSTER_STUN_TICK < GetTickCount())
				{
					m_pBossHpBar->Process_HpBar(m_iHp);
					Clear_Stun();
				}
			}
			break;
		case STATE_DEAD:
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
			ProcessReady();
			Clear_AttackState();
			m_eAttackState = ATTACK_ACTIVE;
		}
		break;
	case STATE_ATTACK:
		if (m_ePreAttackState != m_eAttackState)
		{
			Update_AttackFrame();
			m_ePreAttackState = m_eAttackState;

		}
		if (m_vecSKillElem[m_eBossSkill].dTick + m_vecSKillElem[m_eBossSkill].dMaxTick < GetTickCount())
		{
			ProcessAttack();
			//테스트 없이 수정 함
			m_vecSKillElem[m_eBossSkill].dTick = GetTickCount();
		}
		break;
	case STATE_GOD_MODE:
		ProcessGodMode();
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


	list<CObj*>::iterator iter = m_listBlast.begin();

	for (; iter != m_listBlast.end();)
	{
		if (OBJ_DEAD == (*iter)->Update())
		{
			Safe_Delete(*iter);
			iter = m_listBlast.erase(iter);
		}
		else
		++iter;
	}

	Update_Rect();
	m_pShadow->Update();
	return OBJ_NOEVENT;
}

void CMiddleBoss::Late_Update(void)
{
	for (auto& elem : m_listBlast)
		elem->Late_Update();
}

void CMiddleBoss::Render(HDC hDC)
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

	for (auto& elem : m_listBlast)
	{
		elem->Render(hDC);
	}

	m_pShadow->Render(hDC);
}

void CMiddleBoss::Release(void)
{
	m_pBossHpBar->Set_Dead();
	for (auto& elem : m_listBlast)
	{
		if (m_listBlast.empty())
			break;
		elem->Release();
	}
	Safe_Delete(m_pShadow);
}

void CMiddleBoss::ProcessMove()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);
	//플레이어 위치 알수 있음
	if (MIDDLE_BOSS_PLAYER_DETECT >= fR)
	{
		Clear_DetectState();
	}
}

void CMiddleBoss::Update_Frame_Move()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	switch (m_eMoveState)
	{
	case IDLE:
		if (90 >= fAngle || 270 <= fAngle)
		{
			m_pFrameKey = L"MIDDLE_BOSS_RIGHT";
			Set_Frame(0, 1000, 0, 0);
		}
		else
		{
			m_pFrameKey = L"MIDDLE_BOSS_LEFT";
			Set_Frame(0, 1000, 0, 0);
		}
		break;
	case MOVE_LEFT:
	case MOVE_RIGHT:
		if (90 >= fAngle || 270 <= fAngle)
		{
			m_pFrameKey = L"MIDDLE_BOSS_RIGHT";
			Set_Frame(0, 100, 1, 3);
		}
		else
		{
			m_pFrameKey = L"MIDDLE_BOSS_LEFT";
			Set_Frame(0, 100, 1, 3);
		}
	case MOVE_READY:
		if (90 >= fAngle || 270 <= fAngle)
		{
			m_pFrameKey = L"MIDDLE_BOSS_RIGHT";
			Set_Frame(0, 100, 2, 3);
		}
		else
		{
			m_pFrameKey = L"MIDDLE_BOSS_LEFT";
			Set_Frame(0, 100, 2, 3);
		}
		break;
	}
}

void CMiddleBoss::ProcessDetect()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	if (90 >= fAngle || 270 <= fAngle)
		m_eMoveState = MOVE_RIGHT;
	else
		m_eMoveState = MOVE_LEFT;

	if (MIDDLE_BOSS_PLAYER_DETECT < fR)
	{
		m_eState = STATE_MOVE;
		Set_MoveState(IDLE);
	}
	else if (MIDDLE_BOSS_PLAYER_DETECT >= fR && MIDDLE_BOSS_PLAYER_ATTCK < fR)
	{
		m_tInfo.fX += m_fSpeed * cosf(fAngle * PI / 180);
		m_tInfo.fY -= m_fSpeed * sinf(fAngle * PI / 180);
	}
	else if (MIDDLE_BOSS_PLAYER_ATTCK >= fR)
		Set_State(STATE_GOD_MODE);
}

void CMiddleBoss::ProcessReady()
{
	Set_MoveState(MOVE_DOWN);
	Get_PlayerInfo(&m_fPlayerRadius, &m_fPlayerAngle);
	m_vecSKillElem[m_eBossSkill].fSkillAngle = m_fPlayerAngle;
	m_vecSKillElem[m_eBossSkill].dTick = GetTickCount();
}
void CMiddleBoss::Update_AttackFrame()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);
	//스킬에 따른 모션효과 다르게
	if (90 >= fAngle || 270 <= fAngle)
	{
		m_pFrameKey = L"MIDDLE_BOSS_RIGHT";
		Set_Frame(0, 300, 3, 3);
	}
	else
	{
		m_pFrameKey = L"MIDDLE_BOSS_LEFT";
		Set_Frame(0, 300, 3, 3);
	}
}
void CMiddleBoss::Get_PlayerInfo(float * _fR, float* _fAngle)
{
	float fX = Displacement<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX);
	float fY = Displacement<float>(m_pPlayer->Get_Info().fY, m_tInfo.fY);
	*_fR = Distance<float>(fX, fY);
	*_fAngle = Angle<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX, m_tInfo.fY, m_pPlayer->Get_Info().fY);
	*_fAngle = *_fAngle * 180 / PI;
	*_fAngle = (float)((int)*_fAngle % 360);
}

void CMiddleBoss::ProcessAttack()
{
	float fAngle = 0.f;
	if (0 < m_vecSKillElem[m_eBossSkill].iCount)
		--m_vecSKillElem[m_eBossSkill].iCount;

	switch (m_eBossSkill)
	{
	case BOSS_SKILL_CREAT_WIZARD_BALL:
		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create
		(m_tInfo.fX - m_tInfo.fCX * 0.5f,
			m_tInfo.fY - m_tInfo.fCY * 0.5f));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create
		(m_tInfo.fX - m_tInfo.fCX * 0.5f,
			m_tInfo.fY + m_tInfo.fCY * 0.5f));
		break;
	case BOSS_SKILL_ICEBLAST:
		CSoundMgr::Get_Instance()->Set_Sound(L"BALL_ATTACKMODE.mp3", SOUND_BALL, BOSS_VOLUME);
		m_listBlast.push_back(CAbstactFactory<CFireBlastBullet>::Create((float)(m_tInfo.fX + m_tInfo.fCX * 0.5 * cosf(m_fPlayerAngle)), (float)(m_tInfo.fY - m_tInfo.fCY * 0.5 * sinf(m_fPlayerAngle))));
		break;
	case BOSS_SKILL_STATUE:

		if (3 <= m_vecSKillElem[m_eBossSkill].iCount)
		{
			CSoundMgr::Get_Instance()->Set_Sound(L"ROCK_THROW_START.mp3", SOUND_BOSS, BOSS_VOLUME);
			m_vecSKillElem[m_eBossSkill].fSkillAngle += 60;
		}
		else
		{
			CSoundMgr::Get_Instance()->Set_Sound(L"ROCK_THROW_START.mp3", SOUND_BOSS, BOSS_VOLUME);
			m_vecSKillElem[m_eBossSkill].fSkillAngle -= 60;
		}

		if (3 == m_vecSKillElem[m_eBossSkill].iCount)
			m_vecSKillElem[m_eBossSkill].fSkillAngle = m_fPlayerAngle;

		fAngle = m_vecSKillElem[m_eBossSkill].fSkillAngle * PI / 180;

		m_listBlast.push_back(CAbstactFactory<CMiddleBossSton>::Create
		((float)(m_tInfo.fX + m_vecSKillElem[m_eBossSkill].fSkillRadius * cosf(fAngle)),
			(float)(m_tInfo.fY - m_vecSKillElem[m_eBossSkill].fSkillRadius * sinf(fAngle)),
			fAngle));

		//동상 던지기는 스킬 카운트 만큼 다 만든 후 던짐
		if (0 == m_vecSKillElem[m_eBossSkill].iCount)
		{
			for (auto& elem : m_listBlast)
				elem->Set_State(STATE_ATTACK);
		}
		break;
	}

	if (0 >= m_vecSKillElem[m_eBossSkill].iCount)
	{
		switch (m_eBossSkill)
		{
		case BOSS_SKILL_CREAT_WIZARD_BALL:
			m_eBossSkill = BOSS_SKILL_ICEBLAST;
			break;
		case BOSS_SKILL_ICEBLAST:
			m_eBossSkill = BOSS_SKILL_STATUE;
			break;
		case BOSS_SKILL_STATUE:
			m_eBossSkill = BOSS_SKILL_CREAT_WIZARD_BALL;
			break;
		}
		m_vecSKillElem[m_eBossSkill].iCount = m_vecSKillElem[m_eBossSkill].iMaxCount;
		Set_State(STATE_MOVE);
		Set_MoveState(IDLE);
	}
	else
	{
		Set_State(STATE_ATTACK);
		m_vecSKillElem[m_eBossSkill].dTick = GetTickCount();
	}
}

void CMiddleBoss::ProcessGodMode()
{
	switch (Get_GodMode())
	{
	case GOD_MODE_ACTIVE:
		Set_GodMode(GOD_MODE_UNACTIVE);
		m_dGodModeTick = GetTickCount();
		break;
	case GOD_MODE_UNACTIVE:
		if (m_dGodModeTick + MIDDLE_BOSS_GODMODE_TICK < GetTickCount())
		{
			Set_GodMode(GOD_MODE_ACTIVE);
			Set_State(STATE_READY);
			Set_MoveState(MOVE_READY);
		}
		break;
	}
}

void CMiddleBoss::Update_Frame_Damage(const DAMAGE_STATE& _eState)
{
	float fAngle = 0.f, fR = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	switch (_eState)
	{
	case DAMAGE_STATE_STUNED:
		if (90 >= fAngle || 270 <= fAngle)
			m_pFrameKey = L"MIDDLE_BOSS_RIGHT";
		else
			m_pFrameKey = L"MIDDLE_BOSS_LEFT";
		Set_Frame(0, 50, 4, 1);
		break;
	case DAMAGE_STATE_DEAD:
		if (90 >= fAngle || 270 <= fAngle)
			m_pFrameKey = L"MIDDLE_BOSS_RIGHT";
		else
			m_pFrameKey = L"MIDDLE_BOSS_LEFT";
		Set_Frame(0, 100, 5, 3);
	default:
		break;
	}
}

void CMiddleBoss::Clear_Stun()
{
	Set_State(STATE_MOVE);
	Clear_Move_PreState();
	Clear_Damage_PreState();
}

void CMiddleBoss::Clear_MoveState()
{
	Set_State(STATE_MOVE);
	Set_MoveState(IDLE);
	Clear_Move_PreState();
}
void CMiddleBoss::Clear_DetectState()
{
	Set_State(STATE_DETECT);
	Clear_Move_PreState();
}
void CMiddleBoss::Clear_AttackState()
{
	Set_State(STATE_ATTACK);
	Clear_PreAttackState();
}

void CMiddleBoss::Process_Dead()
{
	Set_State(STATE_DEAD);
	Set_DamageState(DAMAGE_STATE_DEAD);
}
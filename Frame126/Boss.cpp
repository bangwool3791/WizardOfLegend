#include "stdafx.h"
#include "Boss.h"
#include "BossHpBar.h"
#include "ColumnSkill.h"
#include "BossSton.h"
#include "BottomHole.h"
#include "UserHeader.h"
#include "SoilEffect.h"
#include "TileMgr.h"
#include "Shadow.h"

CBoss::CBoss()
{
}

CBoss::CBoss(float _fX, float _fY) : m_fPlayerAngle(0.f), m_fPlayerRadius(0.f), m_eBossSkill(BOSS_SKILL_COLUMN), m_eHpState(HP_STATE_FULL)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CBoss::~CBoss()
{
	Release();
}

void CBoss::Initialize(void)
{

	m_tInfo.fCX = (float)FCX_BOSS;
	m_tInfo.fCY = (float)FCY_BOSS;
	m_pFrameKey = L"BOSS_LEFT";
	Set_State(STATE_MOVE);

	m_pPlayer = m_pObjMgr->Get_Player();

	Set_Frame(0, 1000, 0, 0);
	m_fSpeed = 5.f;

	Set_Hp(BOSS_HP);

	m_vecSKillElem.reserve(BOSS_SKILL_END);
	for (auto& elem : m_vecSKillElem)
		ZeroMemory(&elem, sizeof(SkillElem));

	m_vecSKillElem.push_back(SkillElem{ 6, 6, 0, 1500 , SKILLID_END, FCX_BOSS * 0.5 , 0.f });
	m_vecSKillElem.push_back(SkillElem{ 3, 3, 0, 1000 , SKILLID_END, FCX_BOSS , 0.f });
	m_vecSKillElem.push_back(SkillElem{ 0xFF, 0xFF, 0, 1 , SKILLID_END, FCX_BOSS , 0.f });
	m_vecSKillElem.push_back(SkillElem{ 7, 7,  0, 1000 , SKILLID_END, FCX_BOSS , 0.f });
	m_vecSKillElem.push_back(SkillElem{ 0xFF, 0xFF, 0, 1 , SKILLID_END, FCX_BOSS , 0.f });

	Set_GodMode(GOD_MODE_ACTIVE);
	Set_AttackState(ATTACK_SKILL_COLUMN);
	m_pObjMgr->AddObject(UI_KEY, UI_BOSS_BAR, CAbstactFactory<CBossHpBar>::Create());
	m_pBossHpBar = dynamic_cast<CBossHpBar*>(m_pObjMgr->Get_Target(UI_KEY, UI_BOSS_BAR, nullptr));

	CSoundMgr::Get_Instance()->Set_Bgm(L"BOSS_BGM.mp3", BGM_VOLUME);
	CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_POSE.mp3", SOUND_BOSS, BOSS_VOLUME);
	m_eGroup = GAMEOBJECT;

	m_pShadow = dynamic_cast<CShadow*>(CAbstactFactory<CShadow>::Create((float)(m_tInfo.fX), (float)(m_tInfo.fY + m_tInfo.fCY * 0.5)));
	m_pShadow->Initialize();
	m_pShadow->Set_Target(this);
	m_pShadow->Set_Shawdow(L"BIG_SHADOW", FCX_UI_BIG_SHADOW, FCY_UI_BIG_SHADOW);

	Update_Rect();
}

int	CBoss::Update(void)
{
	static bool bDead = false;
	if (InCrease_X_Frame(m_tFrame))
	{

		switch (Get_State())
		{
		case STATE_ATTACK:
			switch (m_eAttackState)
			{
			case ATTACK_SKILL_JUMP_READY:
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_READYJUMP.mp3", SOUND_BOSS, BOSS_VOLUME);
				Set_AttackState(ATTACK_SKILL_JUMP);
				break;
			}
			break;
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
			if (!bDead)
			{
				Set_Frame(0, 0xFFFF, 5, 0);
				bDead = true;
			}
			break;
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

	m_pShadow->Update();
	Update_Rect();

	return OBJ_NOEVENT;
}

void CBoss::Late_Update(void)
{
	for (auto& elem : m_listBlast)
		elem->Late_Update();

	Collision_RectEx(CTileMgr::Get_Instance()->Get_ColVec(), this);
}

void CBoss::Render(HDC hDC)
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

void CBoss::Release(void)
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

void CBoss::ProcessMove()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);
	//플레이어 위치 알수 있음
	if (BOSS_PLAYER_DETECT >= fR)
	{
		Clear_DetectState();
	}
}

void CBoss::Update_Frame_Move()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	switch (m_eMoveState)
	{
	case IDLE:
		if (90 >= fAngle || 270 <= fAngle)
		{
			m_pFrameKey = L"BOSS_RIGHT";
			Set_Frame(0, 1000, 0, 0);
		}
		else
		{
			m_pFrameKey = L"BOSS_LEFT";
			Set_Frame(0, 1000, 0, 0);
		}
		break;
	case MOVE_LEFT:
	case MOVE_RIGHT:
		if (90 >= fAngle || 270 <= fAngle)
		{
			m_pFrameKey = L"BOSS_RIGHT";
			Set_Frame(0, 200, 1, 8);
		}
		else
		{
			m_pFrameKey = L"BOSS_LEFT";
			Set_Frame(0, 200, 1, 8);
		}
		break;
	}
}

void CBoss::ProcessDetect()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	if (90 >= fAngle || 270 <= fAngle)
		m_eMoveState = MOVE_RIGHT;
	else
		m_eMoveState = MOVE_LEFT;

	if (BOSS_PLAYER_DETECT < fR)
	{
		m_eState = STATE_MOVE;
		Set_MoveState(IDLE);
	}
	else if (BOSS_PLAYER_DETECT >= fR && BOSS_PLAYER_ATTCK < fR)
	{
		m_tInfo.fX += m_fSpeed * cosf(fAngle * PI / 180);
		m_tInfo.fY -= m_fSpeed * sinf(fAngle * PI / 180);
	}
	else if (BOSS_PLAYER_ATTCK >= fR)
		Set_State(STATE_GOD_MODE);
}

void CBoss::ProcessReady()
{
	Get_PlayerInfo(&m_fPlayerRadius, &m_fPlayerAngle);
	m_vecSKillElem[m_eBossSkill].fSkillAngle = m_fPlayerAngle;
	m_vecSKillElem[m_eBossSkill].dTick = GetTickCount();
	
	switch (m_eHpState)
	{
	case HP_STATE_FULL:
		if (BOSS_HALF_HP > Get_Hp())
		{
			m_eHpState = HP_STATE_HALF;
			m_eBossSkill = BOSS_SKILL_STONE_COLUMN;
			Set_AttackState(ATTCK_CREATE_STONE);
		}
		break;
	}
}
void CBoss::Update_AttackFrame()
{
	float fR = 0.f;
	float fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	switch (m_eAttackState)
	{
	case ATTACK_SKILL_COLUMN:
		if (90 >= fAngle || 270 <= fAngle)
			m_pFrameKey = L"BOSS_RIGHT";
		else
			m_pFrameKey = L"BOSS_LEFT";
		Set_Frame(0, 820, 2, 1);
		break;
	case ATTCK_CREATE_STONE:
		if (90 >= fAngle || 270 <= fAngle)
			m_pFrameKey = L"BOSS_RIGHT";
		else
			m_pFrameKey = L"BOSS_LEFT";
			Set_Frame(0, 500, 2, 1);
		break;
	case ATTCK_SHOOT_STONE:
		m_pFrameKey = L"BOSS_ATTACK";
		if (315 < fAngle || 45 >= fAngle)
			Set_Frame(0, 320, 2, 5);
		else if (45 < fAngle && 135 >= fAngle)
			Set_Frame(0, 320, 1, 5);
		else if (135 < fAngle && 215 >= fAngle)
			Set_Frame(0, 320, 0, 5);
		else
			Set_Frame(0, 320, 3, 5);
		break;
	case ATTACK_SKILL_JUMP_READY:
		if (90 >= fAngle || 270 <= fAngle)
			m_pFrameKey = L"BOSS_RIGHT";
		else
			m_pFrameKey = L"BOSS_LEFT";

		Set_Frame(0, 500, 3, 0);
		break;
	case ATTACK_SKILL_JUMP:
		if (90 >= fAngle || 270 <= fAngle)
			m_pFrameKey = L"BOSS_RIGHT";
		else
			m_pFrameKey = L"BOSS_LEFT";
		Set_Frame(1, 0xFFFF, 3, 1);
		break;
	case ATTACK_SKILL_FALL:
		CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_DROP.mp3",SOUND_BOSS, BOSS_VOLUME);

		if (90 >= fAngle || 270 <= fAngle)
			m_pFrameKey = L"BOSS_RIGHT";
		else
			m_pFrameKey = L"BOSS_LEFT";
		Set_Frame(2, 200, 3, 7);
		break;
	case ATTACK_SKILL_JUMP_COLUMN:
		if (90 >= fAngle || 270 <= fAngle)
			m_pFrameKey = L"BOSS_RIGHT";
		else
			m_pFrameKey = L"BOSS_LEFT";
		Set_Frame(2, 200, 2, 7);
		break;
	case ATTACK_SKILL_STONE_COLUMN:
		m_pFrameKey = L"BOSS_ATTACK";
		if (315 < fAngle || 45 >= fAngle)
			Set_Frame(0, 1000, 2, 5);
		else if (45 < fAngle && 135 >= fAngle)
			Set_Frame(0, 1000, 1, 5);
		else if (135 < fAngle && 215 >= fAngle)
			Set_Frame(0, 1000, 0, 5);
		else
			Set_Frame(0, 1000, 3, 5);
		break;
	}
	//스킬에 따른 모션효과 다르게
}
void CBoss::Get_PlayerInfo(float * _fR, float* _fAngle)
{
	float fX = Displacement<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX);
	float fY = Displacement<float>(m_pPlayer->Get_Info().fY, m_tInfo.fY);
	*_fR = Distance<float>(fX, fY);
	*_fAngle = Angle<float>(m_tInfo.fX, m_pPlayer->Get_Info().fX, m_tInfo.fY, m_pPlayer->Get_Info().fY);
	*_fAngle = *_fAngle * 180 / PI;
	*_fAngle = (float)((int)*_fAngle % 360);
}

void CBoss::ProcessAttack()
{
	static int iSoundIndex = 0;
	float fAngle = 0.f, fR = 0.f;
	static INFO m_PlayerInfo = {};
	list<CObj*>::iterator iter;
	int iIndex = 0;
	if (0 < m_vecSKillElem[m_eBossSkill].iCount)
		--m_vecSKillElem[m_eBossSkill].iCount;

	switch (m_eBossSkill)
	{
	case BOSS_SKILL_COLUMN:
		Get_PlayerInfo(&fR, &fAngle);
		m_pObjMgr->AddObject(SKILL_KEY, SOIL_SKILL, CAbstactFactory<CColumnSkill>::Create(
			(float)(m_tInfo.fX + m_tInfo.fCX  * cosf(fAngle * PI / 180)),
			(float)(m_tInfo.fY - m_tInfo.fCY  * sinf(fAngle * PI / 180)),
			fAngle
		));
		switch(iSoundIndex)
		{
		case 0:
			iSoundIndex = 1;
			CSoundMgr::Get_Instance()->Set_Sound(L"COLUMN_1.mp3", SOUND_BOSS_BACK, BOSS_VOLUME);
			break;
		case 1:
			iSoundIndex = 2;
			CSoundMgr::Get_Instance()->Set_Sound(L"COLUMN_2.mp3", SOUND_BOSS_BACK, BOSS_VOLUME);
			break;
		case 2:
			iSoundIndex = 0;
			CSoundMgr::Get_Instance()->Set_Sound(L"COLUMN_3.mp3", SOUND_BOSS_BACK, BOSS_VOLUME);
			break;
		}
		break;
	case BOSS_SKILL_STONE:
		switch (m_eAttackState)
		{
		case ATTCK_CREATE_STONE:
			m_pObjMgr->AddObject(SKILL_KEY, SOIL_STON, CAbstactFactory<CBossSton>::Create(
				(float)(m_tInfo.fX + m_vecSKillElem[m_eBossSkill].fSkillRadius * cosf(m_vecSKillElem[m_eBossSkill].fSkillAngle * PI / 180)),
				(float)(m_tInfo.fY - m_vecSKillElem[m_eBossSkill].fSkillRadius * sinf(m_vecSKillElem[m_eBossSkill].fSkillAngle* PI / 180))));

			if (2 == m_vecSKillElem[m_eBossSkill].iCount)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"ROCK_THROW_START.mp3", SOUND_BOSS, BOSS_VOLUME);
				m_vecSKillElem[m_eBossSkill].fSkillAngle += 60;
			}
			else if (1 == m_vecSKillElem[m_eBossSkill].iCount)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"ROCK_THROW_1.mp3", SOUND_BOSS, BOSS_VOLUME);
				m_vecSKillElem[m_eBossSkill].fSkillAngle = m_fPlayerAngle;
				m_vecSKillElem[m_eBossSkill].fSkillAngle -= 60;
			}
			else if (0 == m_vecSKillElem[m_eBossSkill].iCount)
			{
				m_vecSKillElem[m_eBossSkill].fSkillAngle += 60;
				CSoundMgr::Get_Instance()->Set_Sound(L"ROCK_THROW_2.mp3", SOUND_BOSS, BOSS_VOLUME);
			}
			break;
		case ATTCK_SHOOT_STONE:
			iter = m_pObjMgr->Get_TargetList(SKILL_KEY, SOIL_STON)->begin();
			for (; iIndex < m_vecSKillElem[m_eBossSkill].iCount; ++iIndex)
				++iter;
			Get_PlayerInfo(&fR, &fAngle);

			if (iter != m_pObjMgr->Get_TargetList(SKILL_KEY, SOIL_STON)->end())
			{
				(*iter)->Set_Angle(fAngle * PI / 180);
				(*iter)->Set_State(STATE_ATTACK);
			
				if (2 == m_vecSKillElem[m_eBossSkill].iCount)
					CSoundMgr::Get_Instance()->Set_Sound(L"ROCK_THROW_START.mp3", SOUND_BOSS, BOSS_VOLUME);
				else if (1 == m_vecSKillElem[m_eBossSkill].iCount)
					CSoundMgr::Get_Instance()->Set_Sound(L"ROCK_THROW_1.mp3", SOUND_BOSS, BOSS_VOLUME);
				else if (0 == m_vecSKillElem[m_eBossSkill].iCount)
					CSoundMgr::Get_Instance()->Set_Sound(L"ROCK_THROW_2.mp3", SOUND_BOSS, BOSS_VOLUME);
			}
			break;
			}

	case BOSS_SKILL_JUMP:
		switch (m_eAttackState)
		{
		case ATTACK_SKILL_JUMP:
			Get_PlayerInfo(&fR, &fAngle);
			if (0 < (m_tInfo.fY + CScrollMgr::Get_Instance()->Get_ScrollY() + m_tInfo.fCY * 0.5))
				m_tInfo.fY -= 20;
			else
			{
				m_vecSKillElem[m_eBossSkill].dMaxTick = 2000;
				Set_AttackState(ATTACK_SKILL_JUMP_STOP);
			}
			break;
		case ATTACK_SKILL_JUMP_STOP:
			m_vecSKillElem[m_eBossSkill].dMaxTick = 1;
			Set_AttackState(ATTACK_SKILL_FALL);
			m_PlayerInfo.fX = m_pPlayer->Get_Info().fX;
			break;
		case ATTACK_SKILL_FALL:
			if (m_tInfo.fY < m_pPlayer->Get_Info().fY - m_pPlayer->Get_Info().fCY * 0.5)
			{
				Get_PlayerInfo(&fR, &fAngle);
				m_tInfo.fX = m_PlayerInfo.fX;
				m_tInfo.fY += 20;
			}
			else
			{
				m_pObjMgr->AddObject(UI_KEY, EFFECT_BOTTOM_HOLE, CAbstactFactory<CBottomHole>::Create((float)m_tInfo.fX, (float)(m_tInfo.fY + m_tInfo.fCY * 0.5)));
				for (float i = 0; i < 360; i+=2.5f)
				{
					m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(
						(float)(m_tInfo.fX + DISTANCE_JUMP_EFFECT * cosf(PI * i / 180)),
						(float)(m_tInfo.fY - DISTANCE_JUMP_EFFECT * sinf(PI * i / 180))
							)
						);
				}
				
				m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_tInfo.fX, m_tInfo.fY - DISTANCE_SOIL_COLUM_EFFECT));
				m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_tInfo.fX, m_tInfo.fY - 0));
				m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_tInfo.fX, m_tInfo.fY + DISTANCE_SOIL_COLUM_EFFECT));
				m_vecSKillElem[m_eBossSkill].iCount = 0;
				Collision_Sphere();
			}
			break;
		}
		break;
	case BOSS_SKILL_STONE_COLUMN:
		switch (m_eAttackState)
		{
		case ATTCK_CREATE_STONE:
			m_vecSKillElem[m_eBossSkill].fSkillAngle = 0.f;
			for (int i = 0; i < 6; ++i)
			{
				m_pObjMgr->AddObject(SKILL_KEY, SOIL_STON, CAbstactFactory<CBossSton>::Create(
					(float)(m_tInfo.fX + m_vecSKillElem[m_eBossSkill].fSkillRadius * cosf(m_vecSKillElem[m_eBossSkill].fSkillAngle * PI / 180)),
					(float)(m_tInfo.fY - m_vecSKillElem[m_eBossSkill].fSkillRadius * sinf(m_vecSKillElem[m_eBossSkill].fSkillAngle* PI / 180))));
				m_vecSKillElem[m_eBossSkill].fSkillAngle += 60.f;
			}
			CSoundMgr::Get_Instance()->Set_Sound(L"ROCK_THROW_1.mp3", SOUND_BOSS, BOSS_VOLUME);
			Set_AttackState(ATTCK_SHOOT_STONE);
			break;
		case ATTCK_SHOOT_STONE:
			iter = m_pObjMgr->Get_TargetList(SKILL_KEY, SOIL_STON)->begin();
			for (; iIndex < m_vecSKillElem[m_eBossSkill].iCount; ++iIndex)
				++iter;
			Get_PlayerInfo(&fR, &fAngle);

			if (iter != m_pObjMgr->Get_TargetList(SKILL_KEY, SOIL_STON)->end())
			{
				(*iter)->Set_Angle(fAngle * PI / 180);
				(*iter)->Set_State(STATE_ATTACK);
				CSoundMgr::Get_Instance()->Set_Sound(L"ROCK_THROW_1.mp3", SOUND_BOSS, BOSS_VOLUME);
			}
			
			Get_PlayerInfo(&fR, &fAngle);
			m_pObjMgr->AddObject(SKILL_KEY, SOIL_SKILL, CAbstactFactory<CColumnSkill>::Create(
				(float)(m_tInfo.fX + m_tInfo.fCX  * cosf(fAngle * PI / 180)),
				(float)(m_tInfo.fY - m_tInfo.fCY  * sinf(fAngle * PI / 180)),
				fAngle
			));
			break;
		}
		break;
	case BOSS_SKILL_JUMP_COLUMN:
		switch (m_eAttackState)
		{
		case ATTACK_SKILL_JUMP:
			Get_PlayerInfo(&fR, &fAngle);
			if (0 < (m_tInfo.fY + CScrollMgr::Get_Instance()->Get_ScrollY() + m_tInfo.fCY * 0.5))
				m_tInfo.fY -= 20;
			else
			{
				m_vecSKillElem[m_eBossSkill].dMaxTick = 2000;
				Set_AttackState(ATTACK_SKILL_JUMP_STOP);
			}
			break;
		case ATTACK_SKILL_JUMP_STOP:
			m_vecSKillElem[m_eBossSkill].dMaxTick = 1;
			Set_AttackState(ATTACK_SKILL_FALL);
			m_PlayerInfo.fX = m_pPlayer->Get_Info().fX;
			break;
		case ATTACK_SKILL_FALL:
			if (m_tInfo.fY < m_pPlayer->Get_Info().fY - m_pPlayer->Get_Info().fCY * 0.5)
			{
				m_tInfo.fX = m_PlayerInfo.fX;
				m_tInfo.fY += 20;
			}
			else
			{
				m_vecSKillElem[m_eBossSkill].fSkillAngle = 0.f;
				fAngle = -45.f;
				for (int i = 0; i < 4; ++i)
				{
					m_pObjMgr->AddObject(SKILL_KEY, SOIL_SKILL, CAbstactFactory<CColumnSkill>::Create(
						(float)(m_tInfo.fX + m_tInfo.fCX  * cosf(m_vecSKillElem[m_eBossSkill].fSkillAngle * PI / 180)),
						(float)(m_tInfo.fY - m_tInfo.fCY  * sinf(m_vecSKillElem[m_eBossSkill].fSkillAngle * PI / 180)),
						fAngle
					));
					fAngle += 90.f;
					m_vecSKillElem[m_eBossSkill].fSkillAngle += 90.f;
				}
				for (float i = 0; i < 360; i += 2.5f)
				{
					m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(
						(float)(m_tInfo.fX + DISTANCE_JUMP_EFFECT * cosf(PI * i / 180)),
						(float)(m_tInfo.fY - DISTANCE_JUMP_EFFECT * sinf(PI * i / 180))
					)
					);
				}
				CSoundMgr::Get_Instance()->Set_Sound(L"ROCK_THROW_START.mp3", SOUND_BOSS_BACK, BOSS_VOLUME);
				m_pObjMgr->AddObject(UI_KEY, EFFECT_BOTTOM_HOLE, CAbstactFactory<CBottomHole>::Create(m_tInfo.fX, (float)(m_tInfo.fY + m_tInfo.fCY * 0.5)));
				m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_tInfo.fX, m_tInfo.fY - DISTANCE_SOIL_COLUM_EFFECT));
				m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_tInfo.fX, m_tInfo.fY - 0));
				m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_tInfo.fX, m_tInfo.fY + DISTANCE_SOIL_COLUM_EFFECT));
				m_vecSKillElem[m_eBossSkill].iCount = 0;
				Collision_Sphere();
			}
			break;
		}

		break;
	}

	if (0 >= m_vecSKillElem[m_eBossSkill].iCount)
	{
		m_vecSKillElem[m_eBossSkill].iCount = m_vecSKillElem[m_eBossSkill].iMaxCount;

		switch (m_eBossSkill)
		{
		case BOSS_SKILL_COLUMN:
			m_eBossSkill = BOSS_SKILL_STONE;
			Set_AttackState(ATTCK_CREATE_STONE);
			break;
		case BOSS_SKILL_STONE:
			switch (m_eAttackState)
			{
			case ATTCK_CREATE_STONE:
				Set_AttackState(ATTCK_SHOOT_STONE);
				break;
			case ATTCK_SHOOT_STONE:
				m_eBossSkill = BOSS_SKILL_JUMP;
				Set_AttackState(ATTACK_SKILL_JUMP_READY);
				break;
			}
			break;
		case BOSS_SKILL_JUMP:
			switch (m_eAttackState)
			{
			case ATTACK_SKILL_FALL:
				m_eBossSkill = BOSS_SKILL_COLUMN;
				Set_AttackState(ATTACK_SKILL_COLUMN);
				break;
			}
			break;
		case BOSS_SKILL_STONE_COLUMN:
			switch (m_eAttackState)
			{
			case ATTCK_CREATE_STONE:
				Set_AttackState(ATTCK_SHOOT_STONE);
				break;
			case ATTCK_SHOOT_STONE:
				m_eBossSkill = BOSS_SKILL_JUMP_COLUMN;
				Set_AttackState(ATTACK_SKILL_JUMP_READY);
				break;
			}
			break;
		case BOSS_SKILL_JUMP_COLUMN:
			switch (m_eAttackState)
			{
			case ATTACK_SKILL_FALL:
				m_eBossSkill = BOSS_SKILL_STONE_COLUMN;
				Set_AttackState(ATTCK_CREATE_STONE);
				break;
			}
			break;
		}
		Set_State(STATE_MOVE);
	}
	else
		Set_State(STATE_ATTACK);
}

void CBoss::ProcessGodMode()
{
	switch (Get_GodMode())
	{
	case GOD_MODE_ACTIVE:
		Set_GodMode(GOD_MODE_UNACTIVE);
		m_dGodModeTick = GetTickCount();
		break;
	case GOD_MODE_UNACTIVE:
		if (m_dGodModeTick + BOSS_GODMODE_TICK < GetTickCount())
		{
			Set_GodMode(GOD_MODE_ACTIVE);
			Set_State(STATE_READY);
		}
		break;
	}
}

void CBoss::Update_Frame_Damage(const DAMAGE_STATE& _eState)
{
	float fR = 0.f, fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);
	switch (_eState)
	{
	case DAMAGE_STATE_STUNED:
		if (90 >= fAngle || 270 <= fAngle)
			m_pFrameKey = L"BOSS_RIGHT";
		else
			m_pFrameKey = L"BOSS_LEFT";
		Set_Frame(0, 50, 4, 1);
		break;
	case DAMAGE_STATE_DEAD:
		CSoundMgr::Get_Instance()->Set_Bgm(L"Success.mp3", SOUND_BGM);
		if (90 >= fAngle || 270 <= fAngle)
			m_pFrameKey = L"BOSS_RIGHT";
		else
			m_pFrameKey = L"BOSS_LEFT";
		Set_Frame(0, 100, 5, 1);
	default:
		break;
	}
}

void CBoss::Clear_Stun()
{
	Set_State(STATE_MOVE);
	Clear_Move_PreState();
	Clear_Damage_PreState();
}

void CBoss::Clear_MoveState()
{
	Set_State(STATE_MOVE);
	Set_MoveState(IDLE);
	Clear_Move_PreState();
}
void CBoss::Clear_DetectState()
{
	Set_State(STATE_DETECT);
	Clear_Move_PreState();
}
void CBoss::Clear_AttackState()
{
	Set_State(STATE_ATTACK);
	Clear_PreAttackState();
}

void CBoss::Process_Dead()
{
	Set_State(STATE_DEAD);
	Set_DamageState(DAMAGE_STATE_DEAD);
}

bool CBoss::Is_PlayerRange()
{
	float fR = 0.f, fAngle = 0.f;
	Get_PlayerInfo(&fR, &fAngle);

	if (BOSS_PLAYER_DETECT < fR)
		return true;
	return false;
}

void CBoss::Collision_Sphere()
{
	float fR = 0.f;
	float fAngle = 0.f;

	if (CCollisionMgr::Check_Sphere(this, m_pPlayer, &fR, &fAngle))
	{
		if (
			CObj::STATE_ATTACK == Get_State() &&
			ATTACK_SKILL_FALL == m_eAttackState)
		{
			m_pPlayer->Set_PosX((float)(fR * cosf(fAngle)));
			m_pPlayer->Set_PosY(float(fR * sinf(fAngle)));
			m_pPlayer->Set_State(CObj::STATE_STUN);
			m_pPlayer->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
			m_pPlayer->Plus_Hp(-this->Get_Attack());
		}
	}
}

void CBoss::Collision_RectEx(vector<CObj*>* _Sour, CObj* _Dest)
{
	RECT		rc{};

	for (auto& Sour : *_Sour)
	{
		float	fX = 0.f, fY = 0.f;

		if (BOSS_SKILL_JUMP == m_eBossSkill || BOSS_SKILL_JUMP_COLUMN == m_eBossSkill)
			break;

		if (CCollisionMgr::Check_Rect(_Dest, Sour, &fX, &fY))
		{
			if (fX > fY)	// 상하 충돌의 경우
			{
				// 상 충돌
				if (_Dest->Get_Info().fY < Sour->Get_Info().fY)
				{
					_Dest->Set_PosY(-fY);
				}
				else // 하 충돌
				{
					_Dest->Set_PosY(fY);
				}
			}
			else			// 좌우 충돌의 경우
			{
				// 상 충돌
				if (_Dest->Get_Info().fX < Sour->Get_Info().fX)
				{
					_Dest->Set_PosX(-fX);
				}
				else // 하 충돌
				{
					_Dest->Set_PosX(fX);
				}
			}
		}
	}
}
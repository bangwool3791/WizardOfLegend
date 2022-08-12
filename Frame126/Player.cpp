#include "stdafx.h"
#include "Player.h"
#include "IShield.h"
#include "IceCrystal.h"
#include "FireDragon.h"
#include "Summon.h"
#include "Mouse.h"
#include "UserHeader.h"
#include "PlayerBar.h"
#include "PlayerSKillBar.h"
#include "IceBlast.h"
#include "Dash.h"
#include "Inventory.h"
#include "Enum.h"
#include "Stat.h"
#include "TileMgr.h"
#include "GoldBar.h"
#include "GaiaArmor.h"
#include "Combo.h"
//Test
#include "WizardBall.h"
#include "Shadow.h"

CPlayer::CPlayer()
	: m_pBmpMgr(CBmpMgr::Get_Instance()), m_eProSKill(PRO_SKILL_END), m_fDragonAngle(0.f), m_dSkillMaxTick(0)
{
	ZeroMemory(&m_tRecPoint, sizeof(POINT));
	m_VecInven.reserve(INVEN_END);//인벤 이넘 정의 순서에 따라 Pushback


	m_VecInven.push_back(INVEN{ PRO_SKILL_DASH, vector<SkillElem>(), FX_INVEN_LEFT_POS, FY_INVEN_LEFT_POS, L"NORMAL_ATTACK_ICON", RECT{ 0,0,0,0 },
		L"NORMAL_ATTACK_SKILLBAR", L"COOLTIME_SHADOW",L"NORMAL_ATTACK_EX", RECT{ 45, 530, 0, 0 }, 0, SKILL::SKILL_LDASH, L"" });
	m_VecInven.push_back(INVEN{ PRO_SKILL_RUN, vector<SkillElem>(),FX_INVEN_SPACE_POS, FY_INVEN_SPACE_POS, L"DASH_ICON" , RECT{ 0,0,0,0 },
		L"DASH_SKILLBAR", L"COOLTIME_SHADOW",L"DASH_EX",RECT{ 83,530,0,0 }, 0, SKILL::SKILL_RUN, L"" });
	m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(),FX_INVEN_Q_POS, FY_INVEN_Q_POS, L"NOT_USED" , RECT{ 0,0,0,0 },
		L"NOT_USED", L"COOLTIME_SHADOW",L"NOT_USED",RECT{ 160,530,0,0 }, 0, SKILL::SKILL_CREATE_PUNCH, L"" });
	m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(),FX_INVEN_E_POS, FY_INVEN_E_POS , L"NOT_USED" , RECT{ 0,0,0,0 },
		L"NOT_USED", L"COOLTIME_SHADOW", L"NOT_USED",RECT{ 200,530,0,0 }, 0, SKILL::SKILL_CREATE, L"" });
	m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(), FX_INVEN_RIGHT_POS, FY_INVEN_RIGHT_POS , L"NOT_USED" , RECT{ 0,0,0,0 },
		L"NOT_USED", L"COOLTIME_SHADOW", L"NOT_USED",RECT{ 120,530,0,0 }, 0, SKILL::SKILL_CREATE, L"" });
	m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(), FX_INVEN_R_POS, FY_INVEN_R_POS , L"NOT_USED" , RECT{ 0,0,0,0 },
		L"NOT_USED", L"COOLTIME_SHADOW", L"NOT_USED",RECT{ 235,530,0,0 }, 0, SKILL::SKILL_CREATE , L"" });

	//m_VecInven.push_back(INVEN{ PRO_SKILL_DASH, vector<SkillElem>(), FX_INVEN_LEFT_POS, FY_INVEN_LEFT_POS, L"NORMAL_ATTACK_ICON", RECT{0,0,0,0},
	//	L"NORMAL_ATTACK_SKILLBAR", L"COOLTIME_SHADOW",L"NORMAL_ATTACK_EX", RECT{ 45, 530, 0, 0 }, 0, SKILL::SKILL_LDASH });
	//m_VecInven.push_back(INVEN{ PRO_SKILL_RUN, vector<SkillElem>(),FX_INVEN_SPACE_POS, FY_INVEN_SPACE_POS, L"DASH_ICON" , RECT{ 0,0,0,0 }, 
	//	L"DASH_SKILLBAR", L"COOLTIME_SHADOW",L"DASH_EX",RECT{ 83,530,0,0 }, 0, SKILL::SKILL_RUN });
	//m_VecInven.push_back(INVEN{ PRO_SKILL_ICE_BLAST, vector<SkillElem>(),FX_INVEN_Q_POS, FY_INVEN_Q_POS, L"NOT_USED" , RECT{ 0,0,0,0 },
	//	L"NOT_USED", L"COOLTIME_SHADOW",L"NOT_USED",RECT{ 160,530,0,0 }, 0, SKILL::SKILL_CREATE_PUNCH });
	//m_VecInven.push_back(INVEN{ PRO_SKILL_ISHIELD, vector<SkillElem>(),FX_INVEN_E_POS, FY_INVEN_E_POS , L"NOT_USED" , RECT{ 0,0,0,0 },
	//	L"NOT_USED", L"COOLTIME_SHADOW", L"NOT_USED",RECT{ 200,530,0,0 }, 0, SKILL::SKILL_CREATE });
	//m_VecInven.push_back(INVEN{ PRO_SKILL_FDRAGON, vector<SkillElem>(), FX_INVEN_RIGHT_POS, FY_INVEN_RIGHT_POS , L"NOT_USED" , RECT{ 0,0,0,0 },
	//	L"NOT_USED", L"COOLTIME_SHADOW", L"NOT_USED",RECT{ 120,530,0,0 }, 0, SKILL::SKILL_CREATE });
	//m_VecInven.push_back(INVEN{ PRO_SKILL_GAIA_ARMOR, vector<SkillElem>(), FX_INVEN_R_POS, FY_INVEN_R_POS , L"NOT_USED" , RECT{ 0,0,0,0 },
	//	L"NOT_USED", L"COOLTIME_SHADOW", L"NOT_USED",RECT{ 235,530,0,0 }, 0, SKILL::SKILL_CREATE });

	/*m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(),FX_INVEN_Q_POS, FY_INVEN_Q_POS, L"NOT_USED" , RECT{ 0,0,0,0 }, L"NOT_USED", L"COOLTIME_SHADOW",RECT{ 160,530,0,0 }, 0, SKILL::SKILL_CREATE_PUNCH });
	m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(),FX_INVEN_E_POS, FY_INVEN_E_POS , L"NOT_USED" , RECT{ 0,0,0,0 }, L"NOT_USED", L"COOLTIME_SHADOW", RECT{ 200,530,0,0 }, 0, SKILL::SKILL_CREATE});
	m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(), FX_INVEN_RIGHT_POS, FY_INVEN_RIGHT_POS , L"NOT_USED" , RECT{ 0,0,0,0 },L"NOT_USED", L"COOLTIME_SHADOW", RECT{ 120,530,0,0 }, 0, SKILL::SKILL_CREATE });*/

	for (int i = 0; i < INVEN_END; ++i)
	{
		//스킬 실행 시간 SkillElem에 추가하기
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ DASH_CNT, DASH_CNT, 0, DASH_TIME, SKILLID::SKILL_DASH, 1.f, 0.f});//PRO_SKILL 정의 순서에 따라 값 Pushback
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ RUN_CNT, RUN_CNT, 0, RUN_TIME, SKILLID::SKILLID_END, 0.f, 0.f });
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ ISHEILD_CNT, ISHEILD_CNT, 0, ISHEILD_TIME, SKILLID::SKILL_ICE_SHIELD, 0.f, 0.f });
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ FDRAGONE_CNT, FDRAGONE_CNT, 0, FDRAGONE_TIME, SKILLID::SKILL_FDRAGON, 0.f, 0.f });
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ ICE_BLAST_CNT, ICE_BLAST_CNT, 0, ICEBLAST_TIME, SKILLID::SKILL_ICE_BLAST, 1.f, 0.f });
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ GAIA_ARMOR_CNT, GAIA_ARMOR_CNT, 0, GAIA_ARMOR_TIME, SKILLID::SKILL_GAIA_ARMOR, 80.f, 0.f });
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ 0, 0, 0, NOT_USED_TIME, SKILLID::SKILLID_END, 0.f, 0.f });
	}

	m_eGroup = GAMEOBJECT;

	m_pScroll = CScrollMgr::Get_Instance();
}


CPlayer::CPlayer(float _fX, float _fY)
	: m_pBmpMgr(CBmpMgr::Get_Instance()), m_eProSKill(PRO_SKILL_END), m_fDragonAngle(0.f), m_dSkillMaxTick(0)
{
	ZeroMemory(&m_tRecPoint, sizeof(POINT));
	m_VecInven.reserve(INVEN_END);//인벤 이넘 정의 순서에 따라 Pushback

	m_VecInven.push_back(INVEN{ PRO_SKILL_DASH, vector<SkillElem>(), FX_INVEN_LEFT_POS, FY_INVEN_LEFT_POS, L"NORMAL_ATTACK_ICON", RECT{ 0,0,0,0 },
		L"NORMAL_ATTACK_SKILLBAR", L"COOLTIME_SHADOW",L"NORMAL_ATTACK_EX", RECT{ 45, 530, 0, 0 }, 0, SKILL::SKILL_LDASH, L"" });
	m_VecInven.push_back(INVEN{ PRO_SKILL_RUN, vector<SkillElem>(),FX_INVEN_SPACE_POS, FY_INVEN_SPACE_POS, L"DASH_ICON" , RECT{ 0,0,0,0 },
		L"DASH_SKILLBAR", L"COOLTIME_SHADOW",L"DASH_EX",RECT{ 83,530,0,0 }, 0, SKILL::SKILL_RUN, L"" });
	m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(),FX_INVEN_Q_POS, FY_INVEN_Q_POS, L"NOT_USED" , RECT{ 0,0,0,0 },
		L"NOT_USED", L"COOLTIME_SHADOW",L"NOT_USED",RECT{ 160,530,0,0 }, 0, SKILL::SKILL_CREATE_PUNCH, L"" });
	m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(),FX_INVEN_E_POS, FY_INVEN_E_POS , L"NOT_USED" , RECT{ 0,0,0,0 },
		L"NOT_USED", L"COOLTIME_SHADOW", L"NOT_USED",RECT{ 200,530,0,0 }, 0, SKILL::SKILL_CREATE, L"" });
	m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(), FX_INVEN_RIGHT_POS, FY_INVEN_RIGHT_POS , L"NOT_USED" , RECT{ 0,0,0,0 },
		L"NOT_USED", L"COOLTIME_SHADOW", L"NOT_USED",RECT{ 120,530,0,0 }, 0, SKILL::SKILL_CREATE, L"" });
	m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(), FX_INVEN_R_POS, FY_INVEN_R_POS , L"NOT_USED" , RECT{ 0,0,0,0 },
		L"NOT_USED", L"COOLTIME_SHADOW", L"NOT_USED",RECT{ 235,530,0,0 }, 0, SKILL::SKILL_CREATE , L"" });

	//m_VecInven.push_back(INVEN{ PRO_SKILL_DASH, vector<SkillElem>(), FX_INVEN_LEFT_POS, FY_INVEN_LEFT_POS, L"NORMAL_ATTACK_ICON", RECT{0,0,0,0},
	//	L"NORMAL_ATTACK_SKILLBAR", L"COOLTIME_SHADOW",L"NORMAL_ATTACK_EX", RECT{ 45, 530, 0, 0 }, 0, SKILL::SKILL_LDASH });
	//m_VecInven.push_back(INVEN{ PRO_SKILL_RUN, vector<SkillElem>(),FX_INVEN_SPACE_POS, FY_INVEN_SPACE_POS, L"DASH_ICON" , RECT{ 0,0,0,0 }, 
	//	L"DASH_SKILLBAR", L"COOLTIME_SHADOW",L"DASH_EX",RECT{ 83,530,0,0 }, 0, SKILL::SKILL_RUN });
	//m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(),FX_INVEN_Q_POS, FY_INVEN_Q_POS, L"NOT_USED" , RECT{ 0,0,0,0 },
	//	L"NOT_USED", L"COOLTIME_SHADOW",L"NOT_USED",RECT{ 160,530,0,0 }, 0, SKILL::SKILL_CREATE_PUNCH });
	//m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(),FX_INVEN_E_POS, FY_INVEN_E_POS , L"NOT_USED" , RECT{ 0,0,0,0 },
	//	L"NOT_USED", L"COOLTIME_SHADOW", L"NOT_USED",RECT{ 200,530,0,0 }, 0, SKILL::SKILL_CREATE });
	//m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(), FX_INVEN_RIGHT_POS, FY_INVEN_RIGHT_POS , L"NOT_USED" , RECT{ 0,0,0,0 },
	//	L"NOT_USED", L"COOLTIME_SHADOW", L"NOT_USED",RECT{ 120,530,0,0 }, 0, SKILL::SKILL_CREATE });
	//m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(), FX_INVEN_R_POS, FY_INVEN_R_POS , L"NOT_USED" , RECT{ 0,0,0,0 },
	//	L"NOT_USED", L"COOLTIME_SHADOW", L"NOT_USED",RECT{ 235,530,0,0 }, 0, SKILL::SKILL_CREATE });

	/*m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(),FX_INVEN_Q_POS, FY_INVEN_Q_POS, L"NOT_USED" , RECT{ 0,0,0,0 }, L"NOT_USED", L"COOLTIME_SHADOW",RECT{ 160,530,0,0 }, 0, SKILL::SKILL_CREATE_PUNCH });
	m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(),FX_INVEN_E_POS, FY_INVEN_E_POS , L"NOT_USED" , RECT{ 0,0,0,0 }, L"NOT_USED", L"COOLTIME_SHADOW", RECT{ 200,530,0,0 }, 0, SKILL::SKILL_CREATE});
	m_VecInven.push_back(INVEN{ PRO_SKILL_END, vector<SkillElem>(), FX_INVEN_RIGHT_POS, FY_INVEN_RIGHT_POS , L"NOT_USED" , RECT{ 0,0,0,0 },L"NOT_USED", L"COOLTIME_SHADOW", RECT{ 120,530,0,0 }, 0, SKILL::SKILL_CREATE });*/

	for (int i = 0; i < INVEN_END; ++i)
	{
		//스킬 실행 시간 SkillElem에 추가하기
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ DASH_CNT, DASH_CNT, 0, DASH_TIME, SKILLID::SKILL_DASH, 1.f, 0.f });//PRO_SKILL 정의 순서에 따라 값 Pushback
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ RUN_CNT, RUN_CNT, 0, RUN_TIME, SKILLID::SKILLID_END, 0.f, 0.f });
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ ISHEILD_CNT, ISHEILD_CNT, 0, ISHEILD_TIME, SKILLID::SKILL_ICE_SHIELD, 0.f, 0.f });
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ FDRAGONE_CNT, FDRAGONE_CNT, 0, FDRAGONE_TIME, SKILLID::SKILL_FDRAGON, 0.f, 0.f });
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ ICE_BLAST_CNT, ICE_BLAST_CNT, 0, ICEBLAST_TIME, SKILLID::SKILL_ICE_BLAST, 1.f, 0.f });
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ GAIA_ARMOR_CNT, GAIA_ARMOR_CNT, 0, GAIA_ARMOR_TIME, SKILLID::SKILL_GAIA_ARMOR, 80.f, 0.f });
		m_VecInven[i].VecSkillElem.push_back(SkillElem{ 0, 0, 0, NOT_USED_TIME, SKILLID::SKILLID_END, 0.f, 0.f });
	}

	m_eGroup = GAMEOBJECT;

	m_pScroll = CScrollMgr::Get_Instance();

	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize(void)
{
	m_tInfo.fCX = (float)FCX_PLAYER;	// 플레이어 가로 사이즈
	m_tInfo.fCY = (float)FCY_PLAYER;	// 플레이어 세로 사이즈
	m_fSpeed = 6.f;
	Set_Attack(5);
	Insert_Texture();
	First_State();

	m_iHp = PLAYER_HP;

	m_pObjMgr->AddObject(EFFECT_KEY, SUMMON, CAbstactFactory<CSummon>::Create(m_tInfo.fX, m_tInfo.fY));

	m_pObjMgr->AddObject(UI_KEY, UI_PLAYER_BAR, CAbstactFactory<CPlayerBar>::Create());
	m_pObjMgr->AddObject(UI_KEY, UI_PLAYER_SKILL_BAR, CAbstactFactory<CPlayerSKillBar>::Create());
	m_pObjMgr->AddObject(UI_KEY, UI_PLAYER_INVEN, CAbstactFactory<CInventory>::Create());
	m_pPlayerSkillBar = dynamic_cast<CPlayerSKillBar*>(m_pObjMgr->Get_Target(UI_KEY, UI_PLAYER_SKILL_BAR, nullptr));
	m_pPlayerSkillBar->Set_Inven(&m_VecInven);
	m_pPlayerHpBar = dynamic_cast<CPlayerBar*>(m_pObjMgr->Get_Target(UI_KEY, UI_PLAYER_BAR, nullptr));
	m_pInven = dynamic_cast<CInventory*>(m_pObjMgr->Get_Target(UI_KEY, UI_PLAYER_INVEN, nullptr));
	m_pInven->Set_SkillBar(&m_VecInven);
	CScrollMgr::Get_Instance()->Set_ShakeTick();

	m_pObjMgr->AddObject(UI_KEY, UI_GOLD, CAbstactFactory<CGoldBar>::Create(730.f, 426.f));
	m_pGoldBar = dynamic_cast<CGoldBar*>(m_pObjMgr->Get_Target(UI_KEY, UI_GOLD, nullptr));
	m_iGold = PLAYER_INIT_GOLD;
	m_pGoldBar->Set_Gold(m_iGold);

	m_pShadow = dynamic_cast<CShadow*>(CAbstactFactory<CShadow>::Create((float)(m_tInfo.fX), (float)(m_tInfo.fY + m_tInfo.fCY * 0.5)));
	m_pShadow->Initialize();
	m_pShadow->Set_Target(this);
	m_eGroup = GAMEOBJECT;
}

int CPlayer::Update(void)
{
	if (true == m_bDead)
		return OBJ_DEAD;

	//static DWORD dTick = GetTickCount();
	//if (dTick + 500 < GetTickCount())
	//{
	//	Set_RecoverPoint(m_tInfo.fX, m_tInfo.fY);
	//	dTick = GetTickCount();
	//}
	OffSet();
	Key_Input();
	Motion_Change();
	Process_PlayerState();
	ProcessCombo();
	//Process_Skill_Timer();
	m_pShadow->Update();
	Update_Rect();
	return OBJ_NOEVENT;
}
void CPlayer::Create_Skill(SKILL _eState, int eSkillID)
{
	float fAngle = 0.f;
	switch (_eState)
	{
	case SKILL_RUN:
		fAngle = Get_Skill_Angle(m_eInven);
		fAngle = fAngle / PI * 180;
		fAngle = (float)((int)fAngle % 360);
		if (315 <= fAngle || 45 >= fAngle)
			CSoundMgr::Get_Instance()->Set_Sound(L"DASH_1.mp3", SOUND_PLAYER, PLAYER_VOLUME);
		else if (45 <= fAngle && 135 >= fAngle)
			CSoundMgr::Get_Instance()->Set_Sound(L"DASH_2.mp3", SOUND_PLAYER, PLAYER_VOLUME);
		else if (135 <= fAngle && 225 >= fAngle)
			CSoundMgr::Get_Instance()->Set_Sound(L"DASH_3.mp3", SOUND_PLAYER, PLAYER_VOLUME);
		else if (225 <= fAngle && 315 >= fAngle)
			CSoundMgr::Get_Instance()->Set_Sound(L"DASH_4.mp3", SOUND_PLAYER, PLAYER_VOLUME);

		Set_RecoverPoint(m_tInfo.fX, m_tInfo.fY);
		m_eProSKill = PRO_SKILL_RUN;
		m_dSkillTick = GetTickCount();
		m_dSkillMaxTick = 1;
		break;
	}

	m_eProSKill = m_VecInven[Get_InvenState()].eSkillState;

	switch (eSkillID)
	{
	case SKILL_FDRAGON:
		m_dSkillTick = GetTickCount();
		m_dSkillMaxTick = 220;
		break;
	case SKILL_ICE_BLAST:
		fAngle = Get_Skill_Angle(m_eInven);
		fAngle = fAngle * 180 / PI;
		fAngle = (float)((int)fAngle % 360);
		if (315 <= fAngle || 45 >= fAngle)
			CSoundMgr::Get_Instance()->Set_Sound(L"ICE_BLAST_1.mp3", SOUND_ICEBLAST, PLAYER_VOLUME);
		else if (45 <= fAngle && 135 >= fAngle)
			CSoundMgr::Get_Instance()->Set_Sound(L"ICE_BLAST_2.mp3", SOUND_ICEBLAST, PLAYER_VOLUME);
		else if (135 <= fAngle && 225 >= fAngle)
			CSoundMgr::Get_Instance()->Set_Sound(L"ICE_BLAST_3.mp3", SOUND_ICEBLAST, PLAYER_VOLUME);
		else if (225 <= fAngle && 315 >= fAngle)
			CSoundMgr::Get_Instance()->Set_Sound(L"ICE_BLAST_4.mp3", SOUND_ICEBLAST, PLAYER_VOLUME);
		//m_eProSKill = PRO_SKILL_ICE_BLAST;
		m_dSkillTick = GetTickCount();
		m_dSkillMaxTick = 20;
		break;
	case SKILL_DASH:
		//m_eProSKill = PRO_SKILL_DASH;
		m_dSkillTick = GetTickCount();
		m_dSkillMaxTick = 220;
		break;
	case SKILL_ICE_SHIELD:
	//	m_eProSKill = PRO_SKILL_ISHIELD;
		//CSoundMgr::Get_Instance()->Set_Sound(L"ICE_KRYSTAL_START.mp3", SOUND_PLAYER, PLAYER_VOLUME);
		m_dSkillTick = GetTickCount();
		m_dSkillMaxTick = 500;
		break;
	case SKILL_GAIA_ARMOR:
	//	m_eProSKill = PRO_SKILL_GAIA_ARMOR;
		CSoundMgr::Get_Instance()->Set_Sound(L"ICE_KRYSTAL_START.mp3", SOUND_PLAYER, PLAYER_VOLUME);
		m_dSkillTick = GetTickCount();
		m_dSkillMaxTick = 1000;
		break;
	default:
		break;
	}
}
void CPlayer::Process_PlayerState(void)
{
	switch (Get_State())
	{
	case STATE::STATE_MOVE:
		if (InCrease_X_Frame(m_tFrame))
			Clear_Move();
		break;
	case STATE::STATE_SKILL:
		switch (m_eSkillState)
		{
		case SKILL::SKILL_LDASH:
		case SKILL::SKILL_RDASH:
			m_tInfo.fX += m_fSpeed * m_SkillDashSpeed * cosf(Get_Skill_Angle(Get_InvenState()));
			m_tInfo.fY -= m_fSpeed * m_SkillDashSpeed * sinf(Get_Skill_Angle(Get_InvenState()));
			break;
		default:
			break;
		}

		switch (m_eProSKill)
		{
		case PRO_SKILL_FDRAGON:
		case PRO_SKILL_ICE_BLAST:
		case PRO_SKILL_DASH:
		case PRO_SKILL_ISHIELD:
		case PRO_SKILL_RUN:
		case PRO_SKILL_GAIA_ARMOR:
			Process_Skill(Get_InvenState(), m_eProSKill);
			break;
		}

		if (InCrease_X_Frame(m_tFrame))
			Clear_Skill();
		break;
	case STATE::STATE_STUN:
		if (InCrease_X_Frame(m_tFrame))
		{
			if (m_dStunTick + PLAYER_STUN_TICK < GetTickCount())
			{
				m_pPlayerHpBar->Process_HpBar(m_iHp);
				Clear_Stun();
			}
		}
		break;
	}
}

void CPlayer::Late_Update(void)
{
	if (SKILL_RUN == m_eSkillState)
	{
		for (int i = 0; i < BOSS; ++i)
			Collision_Sphere(this, m_pObjMgr->Get_TargetList(MONSTER_KEY, i));
		if (!m_pObjMgr->Get_TargetList(MONSTER_KEY, BOSS)->empty())
			Collision_Sphere(this, m_pObjMgr->Get_TargetList(MONSTER_KEY, BOSS)->front());
	}
}

void CPlayer::Render(HDC hDC)
{
	int		iScrollX = (int)m_pScroll->Get_ScrollX();
	int		iScrollY = (int)m_pScroll->Get_ScrollY();

	m_hMemDC = m_pBmpMgr->Find_Image(m_pFrameKey);

	//BitBlt(hDC, m_tRect.left + iScrollX, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, m_hMemDC, 0, 0, SRCCOPY);
	// 1인자 : 복사 받을 최종적으로 그림을 그릴 DC
	// 2, 3인자 : 복사 받을 위치의 좌표를 전달(L, T 좌표)
	// 4, 5인자 : 복사 받을 텍스처의 X, Y 사이즈
	// 6 인자 : 비트맵을 가지고 있는 DC
	// 7, 8인자 : 비트맵을 출력할 시작 좌표(L, T좌표)
	// 9인자 : 출력 효과를 설정 SRCCOPY : 그대로 출력
	GdiTransparentBlt(hDC,							// 1인자 : 복사 받을 최종적으로 그림을 그릴 DC
		(int)(m_tRect.left + iScrollX),	// 2,3인자 : 2, 3인자 : 복사 받을 위치의 좌표를 전달(L, T 좌표)
		(int)(m_tRect.top + iScrollY),
		(int)m_tInfo.fCX,				// 4,5 인자 : 복사 받을 텍스처의 X, Y 사이즈
		(int)m_tInfo.fCY,
		m_hMemDC,							// 6인자 : 비트맵을 가지고 있는 DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,			// 7, 8인자 : 비트맵을 출력할 시작 좌표
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,		// 9, 10인자 : 복사할 비트맵의 X,Y 사이즈	
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));			// 11인자 : 제거하고자 하는 색상

	m_pShadow->Render(hDC);

}

void CPlayer::Release(void)
{
	m_VecInven.clear();
	Safe_Delete(m_pShadow);
}

void CPlayer::Key_Input(void)
{

	if (STATE::STATE_SKILL == Get_State() || STATE::STATE_STUN == Get_State())
		return;

	if (CKeyMgr::Get_Instance()->Key_Up('W') ||
		CKeyMgr::Get_Instance()->Key_Up('A') ||
		CKeyMgr::Get_Instance()->Key_Up('D') ||
		CKeyMgr::Get_Instance()->Key_Up('S'))
		Clear_Move();

	if (CKeyMgr::Get_Instance()->Key_Down('W'))
		Set_IdleState(IDLE_UP);
	else if (CKeyMgr::Get_Instance()->Key_Down('A'))
		Set_IdleState(IDLE_LEFT);
	else if (CKeyMgr::Get_Instance()->Key_Down('D'))
		Set_IdleState(IDLE_RIGHT);
	else if (CKeyMgr::Get_Instance()->Key_Down('S'))
		Set_IdleState(IDLE_DOWN);
	else if (CKeyMgr::Get_Instance()->Key_Down('I'))
	{
		switch (m_pInven->Get_InvenState())
		{
		case CInventory::SHOW:
			CSoundMgr::Get_Instance()->Set_Sound(L"CLOSE_INVENTORY.mp3", SOUND_UI, UI_VOLUME);
			m_pInven->Set_InvenState(CInventory::HIDE);
			break;
		case CInventory::HIDE:
			CSoundMgr::Get_Instance()->Set_Sound(L"OPEN_INVENTORY.mp3", SOUND_UI, UI_VOLUME);
			m_pInven->Set_InvenState(CInventory::SHOW);
			break;
		}
	}

	if (CKeyMgr::Get_Instance()->Key_Down('E'))
	{
		if (0 >= Get_SkillCnt(INVEN_E))
			return;

		Set_InvenState(INVEN_E);
		Set_Skill_Radius(INVEN_E, 1);
		Set_Skill_Angle(INVEN_E, Get_MouseAngle(m_tInfo));
		Set_SkillTick(INVEN_E);
		Set_SkillState(Get_Skill_Motion(INVEN_E), Get_Skill_ID(INVEN_E));
		m_ComboQue.push(m_szE);
	}
	else if (CKeyMgr::Get_Instance()->Key_Down('Q'))
	{
		if (0 >= Get_SkillCnt(INVEN_Q))
			return;

		Set_InvenState(INVEN_Q);
		Set_Skill_Radius(INVEN_Q, 1);
		Set_Skill_Angle(INVEN_Q, Get_MouseAngle(m_tInfo));
		Set_SkillTick(INVEN_Q);
		Set_SkillState(Get_Skill_Motion(INVEN_Q), Get_Skill_ID(INVEN_Q));
		m_ComboQue.push(m_szQ);
	}
	else if (CKeyMgr::Get_Instance()->Key_Down('R'))
	{
		if (0 >= Get_SkillCnt(INVEN_R))
			return;

		Set_InvenState(INVEN_R);
		Set_Skill_Radius(INVEN_R, 1);
		Set_Skill_Angle(INVEN_R, Get_MouseAngle(m_tInfo));
		Set_SkillTick(INVEN_R);
		Set_SkillState(Get_Skill_Motion(INVEN_R), Get_Skill_ID(INVEN_R));
		m_ComboQue.push(m_szR);
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
	{
		if (0 >= Get_SkillCnt(INVEN_VKR))
			return;
		Set_InvenState(INVEN_VKR);
		Set_Skill_Radius(INVEN_VKR, 1);
		Set_Skill_Angle(INVEN_VKR, Get_MouseAngle(m_tInfo));
		Set_SkillTick(INVEN_VKR);
		Set_SkillState(Get_Skill_Motion(INVEN_VKR), Get_Skill_ID(INVEN_VKR));
		m_ComboQue.push(m_szVKR);
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
	{
		if (0 >= Get_SkillCnt(INVEN_VKL) || CInventory::SHOW == m_pInven->Get_InvenState())
			return;

		Set_InvenState(INVEN_VKL);
		Set_Skill_Radius(INVEN_VKL, 1);
		Set_Skill_Angle(INVEN_VKL, Get_MouseAngle(m_tInfo));
		Set_SkillTick(INVEN_VKL);
		Set_SkillState(Get_Skill_Motion(INVEN_VKL), Get_Skill_ID(INVEN_VKL));
		m_ComboQue.push(m_szVKL);
	}


	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		if (0 >= Get_SkillCnt(INVEN_SPACE))
			return;

		Set_InvenState(INVEN_SPACE);
		Set_Skill_Radius(INVEN_SPACE, 1);
		Set_Skill_Angle(INVEN_SPACE, Get_MouseAngle(m_tInfo));
		Set_SkillTick(INVEN_SPACE);
		Set_SkillState(Get_Skill_Motion(INVEN_SPACE), Get_Skill_ID(INVEN_SPACE));
		m_ComboQue.push(m_szSPACE);
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
			Set_Move(-m_fSpeed, m_fSpeed, PI / 4, MOVE_LEFT_DOWN);
		else if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
			Set_Move(-m_fSpeed, -m_fSpeed, PI / 4, MOVE_LEFT_UP);
		else
			Set_Move(-m_fSpeed, 0, 0, MOVE_LEFT);
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
			Set_Move(m_fSpeed, m_fSpeed, PI / 4, MOVE_RIGHT_DOWN);
		else if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
			Set_Move(m_fSpeed, -m_fSpeed, PI / 4, MOVE_RIGHT_UP);
		else
			Set_Move(+m_fSpeed, 0, 0, MOVE_RIGHT);
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
			Set_Move(-m_fSpeed, +m_fSpeed, PI / 4, MOVE_DOWN_LEFT);
		else if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
			Set_Move(+m_fSpeed, +m_fSpeed, PI / 4, MOVE_DOWN_RIGHT);
		else
			Set_Move(0, m_fSpeed, PI / 2, MOVE_DOWN);
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
			Set_Move(-m_fSpeed, -m_fSpeed, PI / 4, MOVE_UP_LEFT);
		else if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
			Set_Move(+m_fSpeed, -m_fSpeed, PI / 4, MOVE_UP_RIGHT);
		else
			Set_Move(0, -m_fSpeed, PI / 2, MOVE_UP);
	}
}

void CPlayer::OffSet(void)
{
	int		iOffsetX = WINCX >> 1;
	int		iOffsetY = WINCY >> 1;

	int		iOffsetLeftX = 100;
	int		iOffsetRightX = 700;

	int		iScrollX = (int)m_pScroll->Get_ScrollX();
	int		iScrollY = (int)m_pScroll->Get_ScrollY();

	// 중간 기준으로 플레이어가 왼쪽을 향하는 경우
	float fX = m_tInfo.fX + iScrollX;
	fX = (float((int)fX % 100));

	float fY = m_tInfo.fY + iScrollY;
	fY = (float)((int)fY % 100);

	if (iOffsetX > (INT)(m_tInfo.fX + iScrollX - fX))
		m_pScroll->Set_ScrollX(m_fSpeed);
	else if (iOffsetX < (INT)(m_tInfo.fX + iScrollX - fX))
		m_pScroll->Set_ScrollX(-m_fSpeed);

	if (iOffsetY >(INT)(m_tInfo.fY + iScrollY - fY))
		m_pScroll->Set_ScrollY(+m_fSpeed);
	else if (iOffsetY <(INT)(m_tInfo.fY + iScrollY - fY))
		m_pScroll->Set_ScrollY(-m_fSpeed);
}

void CPlayer::Motion_Change(void)
{
	switch (Get_State())
	{
	case STATE_MOVE:
		if (m_eMovePreState != m_eMoveState)
		{
			Update_Frame_Move(m_eMoveState);
			m_eMovePreState = m_eMoveState;
		}
		break;
	case STATE_SKILL:
		if (m_eSKillPreState != m_eSkillState)
		{
			Update_Frame_Skill(m_eSkillState);
			m_eSKillPreState = m_eSkillState;
		}
		break;
	case STATE_STUN:
		if (m_ePreDamageState != m_eDamageState)
		{
			Update_FrameStun();
			
			m_ePreDamageState = m_eDamageState;
		}
		break;
	}
	Set_PreState(Get_State());
}
void CPlayer::Update_Frame_Skill(SKILL _eState)
{

	switch (_eState)
	{
	case SKILL::SKILL_RUN:
		if (SKILL_RUN == _eState)
			Set_Frame(0, 70, 2, 7);


		switch (m_eMoveState)
		{
		case MOVE::IDLE:
			switch (m_eIdleState)
			{
			case IDLE_STATE::IDLE_LEFT:
				m_eRunState = IDLE_LEFT;
				m_pFrameKey = L"PLAYER_LEFT";
				m_pShadow->Set_Shawdow(m_pBigShadow, FCX_UI_BIG_SHADOW, FCY_UI_BIG_SHADOW);
				break;
			case IDLE_STATE::IDLE_UP:
				m_eRunState = IDLE_UP;
				m_pFrameKey = L"PLAYER_FRONT";
				m_pShadow->Set_Shawdow(m_pBigVShadow, FCX_UI_BIG_V_SHADOW, FCY_UI_BIG_V_SHADOW);
				break;
			case IDLE_STATE::IDLE_RIGHT:
				m_eRunState = IDLE_RIGHT;
				m_pFrameKey = L"PLAYER_RIGHT";
				m_pShadow->Set_Shawdow(m_pBigShadow, FCX_UI_BIG_SHADOW, FCY_UI_BIG_SHADOW);
				break;
			case IDLE_STATE::IDLE_DOWN:
				m_eRunState = IDLE_DOWN;
				m_pFrameKey = L"PLAYER_BACK";
				m_pShadow->Set_Shawdow(m_pBigVShadow, FCX_UI_BIG_V_SHADOW, FCY_UI_BIG_V_SHADOW);
				break;
			}
			break;
		case MOVE::MOVE_LEFT:
		case MOVE::MOVE_LEFT_DOWN:
		case MOVE::MOVE_LEFT_UP:
		case MOVE::MOVE_UP_LEFT:
		case MOVE::MOVE_DOWN_LEFT:
			m_eRunState = IDLE_LEFT;
			m_pFrameKey = L"PLAYER_LEFT";
			m_pShadow->Set_Shawdow(m_pBigShadow, FCX_UI_BIG_SHADOW, FCY_UI_BIG_SHADOW);
			break;
		case MOVE::MOVE_DOWN:
			m_eRunState = IDLE_DOWN;
			m_pFrameKey = L"PLAYER_BACK";
			m_pShadow->Set_Shawdow(m_pBigVShadow, FCX_UI_BIG_V_SHADOW, FCY_UI_BIG_V_SHADOW);
			break;
		case MOVE::MOVE_RIGHT:
		case MOVE::MOVE_RIGHT_DOWN:
		case MOVE::MOVE_RIGHT_UP:
		case MOVE::MOVE_UP_RIGHT:
		case MOVE::MOVE_DOWN_RIGHT:
			m_eRunState = IDLE_RIGHT;
			m_pFrameKey = L"PLAYER_RIGHT";
			m_pShadow->Set_Shawdow(m_pBigShadow, FCX_UI_BIG_SHADOW, FCY_UI_BIG_SHADOW);
			break;
		case MOVE::MOVE_UP:
			m_eRunState = IDLE_UP;
			m_pFrameKey = L"PLAYER_FRONT";
			m_pShadow->Set_Shawdow(m_pBigVShadow, FCX_UI_BIG_V_SHADOW, FCY_UI_BIG_V_SHADOW);
			break;
		}
		break;
	case SKILL::SKILL_CREATE:
	case SKILL::SKILL_CREATE_PUNCH:
	case SKILL::SKILL_LDASH:
	case SKILL::SKILL_RDASH:
		m_pShadow->Set_Shawdow(m_pNormalShadow, FCX_UI_SHADOW, FCY_UI_SHADOW);
		switch (m_eProSKill)
		{
		case PRO_SKILL_ISHIELD:
			Set_Frame(0, 90, 4, 7);
			break;
		case PRO_SKILL_ICE_BLAST:
			Set_Frame(0, 80, 4, 7);
			break;
		default:
			if (SKILL_CREATE == _eState)
				Set_Frame(0, 50, 4, 7);
			else if (SKILL_CREATE_PUNCH == _eState)
				Set_Frame(0, 50, 3, 7);
			else if (SKILL_LDASH == _eState)
				Set_Frame(0, 50, 3, 7);
			else if (SKILL_RDASH == _eState)
				Set_Frame(0, 50, 4, 7);
			break;
		}

		Set_PlayerDirection();
		break;
	}
	Update_Rect();
}
void CPlayer::Update_Frame_Move(MOVE _eState)
{
	switch (_eState)
	{
	case IDLE:
		m_pShadow->Set_Shawdow(m_pNormalShadow, FCX_UI_SHADOW, FCY_UI_SHADOW);
		switch (m_eIdleState)
		{
		case IDLE_LEFT:
			Set_Frame(0, 100, 0, 0);
			m_pFrameKey = L"PLAYER_LEFT";
			break;
		case IDLE_UP:
			Set_Frame(0, 100, 0, 0);
			m_pFrameKey = L"PLAYER_FRONT";
			break;
		case IDLE_RIGHT:
			Set_Frame(0, 100, 0, 0);
			m_pFrameKey = L"PLAYER_RIGHT";
			break;
		case IDLE_DOWN:
			Set_Frame(0, 100, 0, 0);
			m_pFrameKey = L"PLAYER_BACK";
			break;
		}
		break;
	case MOVE_LEFT:
	case MOVE_LEFT_UP:
	case MOVE_LEFT_DOWN:
		Set_Frame(0, 100, 1, 9);
		m_pFrameKey = L"PLAYER_LEFT";
		CSoundMgr::Get_Instance()->Set_Sound(L"RUN_1.mp3", SOUND_PLAYER, PLAYER_VOLUME);
		m_pShadow->Set_Shawdow(m_pBigShadow, FCX_UI_BIG_SHADOW, FCY_UI_BIG_SHADOW);
		break;
	case MOVE_UP:
	case MOVE_UP_LEFT:
	case MOVE_UP_RIGHT:
		Set_Frame(0, 100, 1, 9);
		m_pFrameKey = L"PLAYER_FRONT";
		CSoundMgr::Get_Instance()->Set_Sound(L"RUN_2.mp3", SOUND_PLAYER, PLAYER_VOLUME);
		m_pShadow->Set_Shawdow(m_pBigVShadow, FCX_UI_BIG_V_SHADOW, FCY_UI_BIG_V_SHADOW);
		break;
	case MOVE_RIGHT:
	case MOVE_RIGHT_UP:
	case MOVE_RIGHT_DOWN:
		Set_Frame(0, 100, 1, 9);
		m_pFrameKey = L"PLAYER_RIGHT";
		CSoundMgr::Get_Instance()->Set_Sound(L"RUN_3.mp3", SOUND_PLAYER, PLAYER_VOLUME);
		m_pShadow->Set_Shawdow(m_pBigShadow, FCX_UI_BIG_SHADOW, FCY_UI_BIG_SHADOW);
		break;
	case MOVE_DOWN:
	case MOVE_DOWN_LEFT:
	case MOVE_DOWN_RIGHT:
		Set_Frame(0, 100, 1, 9);
		m_pFrameKey = L"PLAYER_BACK";
		CSoundMgr::Get_Instance()->Set_Sound(L"RUN_4.mp3", SOUND_PLAYER, PLAYER_VOLUME);
		m_pShadow->Set_Shawdow(m_pBigVShadow, FCX_UI_BIG_V_SHADOW, FCY_UI_BIG_V_SHADOW);
		break;
	}
}

void CPlayer::Insert_Texture(void)
{
}

void CPlayer::First_State(void)
{
	m_pFrameKey = L"Player_DOWN";
	Set_State(STATE_MOVE);
	m_eMoveState = IDLE;
	m_eIdleState = IDLE_DOWN;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iFrameMotion = 0;
	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount();
}

void CPlayer::Set_IdleState(IDLE_STATE _State) {
	m_eIdleState = _State;
}
void CPlayer::Set_SkillState(SKILL _State, SKILLID _eSkillID)
{
	Set_State(STATE_SKILL);
	m_eSkillState = _State;
	Create_Skill(_State, _eSkillID);
}

void CPlayer::Set_Move(float _xSpeed, float _ySpeed, float _fAngle, MOVE eMove)
{
	m_tInfo.fX += _xSpeed * cosf(_fAngle);
	m_tInfo.fY += _ySpeed * sinf(_fAngle);
	m_eMoveState = eMove;
}

void CPlayer::Clear_Skill()
{
	if (Check_Process_Skil())
		return;

	Set_State(STATE_MOVE);
	m_eMoveState = IDLE;
	m_eIdlePreState = IDLE_END;
	if (STATE::STATE_SKILL == m_ePreState)
	{
		if (SKILL_RUN == m_eSKillPreState)
		{
			m_eIdleState = m_eRunState;
		}
	}
	Clear_Skill_State();
	Clear_Move_PreState();
}

void CPlayer::Clear_Move()
{
	Set_State(STATE_MOVE);
	switch (m_eMovePreState)
	{
		break;
	case MOVE::MOVE_LEFT:
	case MOVE::MOVE_LEFT_DOWN:
	case MOVE::MOVE_LEFT_UP:
		m_eIdleState = IDLE_LEFT;
		break;
	case MOVE::MOVE_UP:
	case MOVE::MOVE_UP_LEFT:
	case MOVE::MOVE_UP_RIGHT:
		m_eIdleState = IDLE_UP;
		break;
	case MOVE::MOVE_RIGHT:
	case MOVE::MOVE_RIGHT_DOWN:
	case MOVE::MOVE_RIGHT_UP:
		m_eIdleState = IDLE_RIGHT;
		break;
	case MOVE::MOVE_DOWN:
	case MOVE::MOVE_DOWN_LEFT:
	case MOVE::MOVE_DOWN_RIGHT:
		m_eIdleState = IDLE_DOWN;
	}
	m_eMoveState = IDLE;
	m_eIdlePreState = IDLE_END;
	Clear_Move_PreState();
}
void CPlayer::Clear_Stun()
{
	Set_State(STATE_MOVE);
	m_eMoveState = IDLE;
	Clear_Damage_PreState();
	Clear_Skill_State();
}


void CPlayer::Process_Skill(INVENTORY _eInven, PRO_SKILL _eSKill)
{
	if(m_dSkillTick + m_dSkillMaxTick < GetTickCount())
	{
		float fX = 0.f;
		float fY = 0.f;
		float fAngle = Get_Skill_Angle(_eInven);
		float fRadius = Get_Skill_Radius(_eInven);
		SKILLID eSKillID = Get_Skill_ID(_eInven);

		switch (_eSKill)
		{
			case PRO_SKILL_RUN:
				switch (m_eMoveState)
				{
				case MOVE::IDLE:
					switch (m_eRunState)
					{
					case IDLE_LEFT:
						m_tInfo.fX -= m_fSpeed * m_SkillMoveSpeed;
						break;
					case IDLE_UP:
						m_tInfo.fY -= m_fSpeed * m_SkillMoveSpeed;
						break;
					case IDLE_RIGHT:
						m_tInfo.fX += m_fSpeed * m_SkillMoveSpeed;
						break;
					case IDLE_DOWN:
						m_tInfo.fY += m_fSpeed * m_SkillMoveSpeed;
						break;
					}
					break;
				case MOVE::MOVE_DOWN:
					m_tInfo.fY += m_fSpeed * m_SkillMoveSpeed;
					break;
				case MOVE::MOVE_RIGHT:
					m_tInfo.fX += m_fSpeed * m_SkillMoveSpeed;
					break;
				case MOVE::MOVE_LEFT:
					m_tInfo.fX -= m_fSpeed * m_SkillMoveSpeed;
					break;
				case MOVE::MOVE_UP:
					m_tInfo.fY -= m_fSpeed * m_SkillMoveSpeed;
					break;
				case MOVE::MOVE_LEFT_DOWN:
				case MOVE::MOVE_DOWN_LEFT:
					m_tInfo.fX -= m_fSpeed * m_SkillMoveSpeed * cosf(PI / 4);
					m_tInfo.fY += m_fSpeed * m_SkillMoveSpeed * sinf(PI / 4);
					break;
				case MOVE::MOVE_LEFT_UP:
				case MOVE::MOVE_UP_LEFT:
					m_tInfo.fX -= m_fSpeed * m_SkillMoveSpeed * cosf(PI / 4);
					m_tInfo.fY -= m_fSpeed * m_SkillMoveSpeed * sinf(PI / 4);
					break;
				case MOVE::MOVE_RIGHT_DOWN:
				case MOVE::MOVE_DOWN_RIGHT:
					m_tInfo.fX += m_fSpeed * m_SkillMoveSpeed * cosf(PI / 4);
					m_tInfo.fY += m_fSpeed * m_SkillMoveSpeed * sinf(PI / 4);
					break;
				case MOVE::MOVE_RIGHT_UP:
				case MOVE::MOVE_UP_RIGHT:
					m_tInfo.fX += m_fSpeed * m_SkillMoveSpeed * cosf(PI / 4);
					m_tInfo.fY -= m_fSpeed * m_SkillMoveSpeed * sinf(PI / 4);
					break;
				}
				break;
			case PRO_SKILL_ISHIELD:
				if (Get_SkillMaxCnt(_eInven) != Get_SkillCnt(_eInven))
					return;

				CSoundMgr::Get_Instance()->Set_Sound(L"ICE_KRYSTAL_START.mp3", SOUND_ISHIELD, PLAYER_VOLUME);
					m_pObjMgr->AddObject(SKILL_KEY, eSKillID,
						CAbstactFactory<CIShield>::Create(fX, fY, 0));
					m_pObjMgr->AddObject(SKILL_KEY, eSKillID,
						CAbstactFactory<CIShield>::Create(fX, fY, PI * 90 / 180));
					m_pObjMgr->AddObject(SKILL_KEY, eSKillID,
						CAbstactFactory<CIShield>::Create(fX, fY, PI * 180 / 180));
					m_pObjMgr->AddObject(SKILL_KEY, eSKillID,
						CAbstactFactory<CIShield>::Create(fX, fY, PI * 270 / 180));
					Init_SkillCnt(_eInven, 0);
				break;
			case PRO_SKILL_GAIA_ARMOR:
				if (Get_SkillMaxCnt(_eInven) != Get_SkillCnt(_eInven))
					return;
	
				m_pObjMgr->AddObject(SKILL_KEY, eSKillID,
						CAbstactFactory<CGaiaArmor>::Create(fX, fY, 0));
					m_pObjMgr->AddObject(SKILL_KEY, eSKillID,
						CAbstactFactory<CGaiaArmor>::Create(fX, fY, PI * 45 / 180));
					m_pObjMgr->AddObject(SKILL_KEY, eSKillID,
						CAbstactFactory<CGaiaArmor>::Create(fX, fY, PI * 90 / 180));
					m_pObjMgr->AddObject(SKILL_KEY, eSKillID,
						CAbstactFactory<CGaiaArmor>::Create(fX, fY, PI * 135 / 180));
					m_pObjMgr->AddObject(SKILL_KEY, eSKillID,
						CAbstactFactory<CGaiaArmor>::Create(fX, fY, PI * 180 / 180));
					m_pObjMgr->AddObject(SKILL_KEY, eSKillID,
						CAbstactFactory<CGaiaArmor>::Create(fX, fY, PI * 225 / 180));
					m_pObjMgr->AddObject(SKILL_KEY, eSKillID,
						CAbstactFactory<CGaiaArmor>::Create(fX, fY, PI * 270 / 180));
					m_pObjMgr->AddObject(SKILL_KEY, eSKillID,
						CAbstactFactory<CGaiaArmor>::Create(fX, fY, PI * 315 / 180));

					CSoundMgr::Get_Instance()->Set_Sound(L"GAIA_ARMOR_START.mp3", SOUND_GAIA_ARMOR, PLAYER_VOLUME);
					Init_SkillCnt(_eInven, 0);
				break;
			case PRO_SKILL_FDRAGON:
				if(Get_SkillCnt(m_eInven) % 3 == 0)
					CSoundMgr::Get_Instance()->Set_Sound(L"FIRE_DRAGON_1.mp3", SOUND_FDRAGONE, PLAYER_VOLUME);
				else if (Get_SkillCnt(m_eInven) % 3 == 1)
					CSoundMgr::Get_Instance()->Set_Sound(L"FIRE_DRAGON_2.mp3", SOUND_FDRAGONE, PLAYER_VOLUME);
				else if (Get_SkillCnt(m_eInven) % 3 == 2)
					CSoundMgr::Get_Instance()->Set_Sound(L"FIRE_DRAGON_3.mp3", SOUND_FDRAGONE, PLAYER_VOLUME);

				m_pObjMgr->AddObject(SKILL_KEY, eSKillID, CAbstactFactory<CFireDragon>::Create(
					(float)(m_tInfo.fX + FCX_PLAYER * 0.5 * cosf(fAngle)),
					(float)(m_tInfo.fY - FCY_PLAYER * 0.5 * sinf(fAngle)), fAngle));
				break;
			case PRO_SKILL_ICE_BLAST:			

				fX = (float)(m_tInfo.fX + FCX_PLAYER * 0.5 * fRadius * cosf(fAngle));
				fY = (float)(m_tInfo.fY - FCY_PLAYER * 0.5 * fRadius * sinf(fAngle));
				m_pObjMgr->AddObject(SKILL_KEY, eSKillID,CAbstactFactory<CIceBlast>::Create(fX,fY,Get_Skill_Angle(_eInven)));

				//아이스 블러스트 복귀 
				if (ICE_BLAST_CNT * 0.5 <= Get_SkillCnt(_eInven))
					Plus_Skill_Radius(_eInven, 1.5);
				else
					Plus_Skill_Radius(_eInven, -1.5);
				break;
			case PRO_SKILL_DASH:
				fX = (float)(m_tInfo.fX + FCX_PLAYER * 0.5 * cosf(fAngle));
				fY = (float)(m_tInfo.fY - FCY_PLAYER * 0.5 * sinf(fAngle));
				fAngle = fAngle / PI * 180;
				fAngle = (float)((int)fAngle % 360);
				m_pObjMgr->AddObject(SKILL_KEY, eSKillID, CAbstactFactory<CDash>::Create(fX, fY));
				if (315 <= fAngle || 45 >= fAngle)
					m_pObjMgr->Get_Target(SKILL_KEY, eSKillID, nullptr)->Set_Frame(0, 80, 1, 4);
				else if (45 <= fAngle && 135 >= fAngle)
				{
					switch (m_eSkillState)
					{
					case SKILL::SKILL_LDASH:
						m_pObjMgr->Get_Target(SKILL_KEY, eSKillID, nullptr)->Set_Frame(0, 80, 2, 4);
						break;
					case SKILL::SKILL_RDASH:
						m_pObjMgr->Get_Target(SKILL_KEY, eSKillID, nullptr)->Set_Frame(0, 80, 3, 4);
						break;
					}

				}
				else if (135 <= fAngle && 225 >= fAngle)
					m_pObjMgr->Get_Target(SKILL_KEY, eSKillID, nullptr)->Set_Frame(0, 80, 0, 4);
				else if (225 <= fAngle && 315 >= fAngle)
				{
					switch (m_eSkillState)
					{
					case SKILL::SKILL_LDASH:
						m_pObjMgr->Get_Target(SKILL_KEY, eSKillID, nullptr)->Set_Frame(0, 80, 4, 4);
						break;
					case SKILL::SKILL_RDASH:
						m_pObjMgr->Get_Target(SKILL_KEY, eSKillID, nullptr)->Set_Frame(0, 80, 5, 4);
						break;
					}
				}
				static int iDashSound = 0;
				switch (iDashSound)
				{
				case 0:
					CSoundMgr::Get_Instance()->Set_Sound(L"NORMAL_ATTACK_1.mp3", SOUND_DASH, PLAYER_VOLUME);
					iDashSound = 1;
					break;
				case 1:
					CSoundMgr::Get_Instance()->Set_Sound(L"NORMAL_ATTACK_2.mp3", SOUND_DASH, PLAYER_VOLUME);
					iDashSound = 2;
					break;
				case 2:
					CSoundMgr::Get_Instance()->Set_Sound(L"NORMAL_ATTACK_3.mp3", SOUND_DASH, PLAYER_VOLUME);
					iDashSound = 0;
					break;
				}
		}
		//스킬카운트 감소 스킬 카운트는 증가한 상태에서 감소하는 프로세스, 쿨타임은 따로 구현해야 할듯

		if(0 < Get_SkillCnt(_eInven))
			Plus_SkillCnt(_eInven, -1);

		if (0 >= Get_SkillCnt(_eInven))
		{
			m_eProSKill = PRO_SKILL_END;
			m_dSkillTick = 0xFFFFFFFE;
		}
		else
		{
			switch (m_eProSKill)
			{
			case PRO_SKILL::PRO_SKILL_DASH:
				if (SKILL::SKILL_LDASH == m_eSkillState)
					m_eSkillState = SKILL_RDASH;
				else
					m_eSkillState = SKILL_LDASH;
				break;
			}
			m_dSkillTick = GetTickCount();
		}
	}
}

void CPlayer::Plus_SkillCnt(INVENTORY _eInven, int _iCount)
{
	m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].iCount += _iCount;
}

void CPlayer::Set_SkillCnt(INVENTORY _eInven, int _iCount)
{
	m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].iCount += _iCount;
}

void CPlayer::Init_SkillCnt(INVENTORY _eInven, int _iCount)
{
	m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].iCount = _iCount;
}


const int CPlayer::Get_SkillCnt(INVENTORY _eInven)
{
	return m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].iCount;
}

const int CPlayer::Get_SkillMaxCnt(INVENTORY _eInven)
{
	return m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].iMaxCount;
}

const SKILLID CPlayer::Get_Skill_ID(INVENTORY _eInven)
{
	return m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].eSkillID;
}

void CPlayer::Set_Skill_ID(INVENTORY _eInven, SKILLID _eSKill_ID)
{
	m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].eSkillID = _eSKill_ID;
}

void CPlayer::Set_SkillTick(INVENTORY _eInven)
{
	m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].dTick = GetTickCount();
}

const DWORD CPlayer::Get_SkillTick(INVENTORY _eInven)
{
	return m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].dTick;
}

const DWORD CPlayer::Get_Skill_MaxTick(INVENTORY _eInven)
{
	return m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].dMaxTick;
}

void CPlayer::Set_Skill_Radius(INVENTORY _eInven, float _fRaidus)
{
	m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].fSkillRadius = _fRaidus;
}

void CPlayer::Plus_Skill_Radius(INVENTORY _eInven, float _fRaidus)
{
	m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].fSkillRadius += _fRaidus;
}

const float CPlayer::Get_Skill_Radius(INVENTORY _eInven)
{
	return m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].fSkillRadius;
}

void CPlayer::Set_Skill_Angle(INVENTORY _eInven, float _fAngle)
{
	m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].fSkillAngle = _fAngle;
}

const float CPlayer::Get_Skill_Angle(INVENTORY _eInven)
{
	return m_VecInven[_eInven].VecSkillElem[m_VecInven[_eInven].eSkillState].fSkillAngle;
}

void CPlayer::Set_Skill_Motion(INVENTORY _eInven, SKILL _eSkillMotion)
{
	m_VecInven[_eInven].eSKillMotion = _eSkillMotion;
}

const SKILL& CPlayer::Get_Skill_Motion(INVENTORY _eInven)
{
	return m_VecInven[_eInven].eSKillMotion;
}

bool CPlayer::Check_Process_Skil()
{
	if (PRO_SKILL_END != m_eProSKill)
		return true;
	else
		return false;
}

void CPlayer::Process_Detect_Mouse()
{
	CObj* pMouse = m_pObjMgr->Get_Target(OBJ_KEY, OBJ_MOUSE, nullptr);
	float fAngle = Angle<float>(m_tInfo.fX, pMouse->Get_Info().fX, pMouse->Get_Info().fY, m_tInfo.fY);
}

void CPlayer::Process_Recover_Point()
{
	m_tInfo.fX = (float)m_tRecPoint.x;
	m_tInfo.fY = (float)m_tRecPoint.y;
	Update_Rect();
	//ZeroMemory(&m_tRecPoint, sizeof(POINT));
}
void CPlayer::Set_RecoverPoint(float _x, float _y)
{
	if (SKILL::SKILL_RUN == m_eSkillState)
		return;

	m_tRecPoint.x = (LONG)_x;
	m_tRecPoint.y = (LONG)_y;
}

bool  CPlayer::Collision_Lava(vector<CObj*>* _Sour)
{
	RECT		rc{};
	int			iIndex = 0;
	for (auto& Sour : *_Sour)
	{
		float	fX = 0.f, fY = 0.f;

		if (PRO_SKILL_RUN == this->Get_ProSKill())
			continue;

		if (CCollisionMgr::Check_Rect((CObj*)this, Sour, &fX, &fY))
		{
			return true;
		}
	}
	return false;
}

void CPlayer::Set_ItemSkill(INVENTORY _eIven, PRO_SKILL _eProSkill)
{
	m_VecInven[_eIven].eSkillState = _eProSkill;
}

float CPlayer::Get_MouseAngle(const INFO& _tInfo)
{
	CObj* pMouse = m_pObjMgr->Get_Target(OBJ_KEY, OBJ_MOUSE, nullptr);
	float fX = pMouse->Get_Info().fX - CScrollMgr::Get_Instance()->Get_ScrollX();
	float fY = pMouse->Get_Info().fY - CScrollMgr::Get_Instance()->Get_ScrollY();
	return Angle<float>(_tInfo.fX, fX, _tInfo.fY, fY);
}

POINT CPlayer::Get_MousePlayerPoint(const INFO& _tInfo)
{
	CObj* pMouse = m_pObjMgr->Get_Target(OBJ_KEY, OBJ_MOUSE, nullptr);
	float fX = pMouse->Get_Info().fX - CScrollMgr::Get_Instance()->Get_ScrollX();
	float fY = pMouse->Get_Info().fY - CScrollMgr::Get_Instance()->Get_ScrollY();
	fX = Displacement<float>(_tInfo.fX, fX);
	fY = Displacement<float>(fY, _tInfo.fY);
	return POINT{ (LONG)fX, (LONG)fY };
}

void CPlayer::Set_PlayerDirection()
{
	float fAngle = Get_Skill_Angle(m_eInven);
	fAngle = fAngle * 180 / PI;
	fAngle = (float)((int)fAngle % 360);
	if (315 <= fAngle || 45 >= fAngle)
	{
		m_eIdleState = IDLE_RIGHT;
		m_pFrameKey = L"PLAYER_RIGHT";
	}
	else if (45 <= fAngle && 135 >= fAngle)
	{
		m_eIdleState = IDLE_UP;
		m_pFrameKey = L"PLAYER_FRONT";
	}

	else if (135 <= fAngle && 225 >= fAngle)
	{
		m_eIdleState = IDLE_LEFT;
		m_pFrameKey = L"PLAYER_LEFT";
	}
	else if (225 <= fAngle && 315 >= fAngle)
	{
		m_eIdleState = IDLE_DOWN;
		m_pFrameKey = L"PLAYER_BACK";
	}
}

void CPlayer::Update_FrameStun()
{	
	switch (m_eIdleState)
	{
	case IDLE_LEFT:
		m_pFrameKey = L"PLAYER_LEFT";
		break;
	case IDLE_UP:
		m_pFrameKey = L"PLAYER_BACK";
		break;
	case IDLE_RIGHT:
		m_pFrameKey = L"PLAYER_RIGHT";
		break;
	case IDLE_DOWN:
		m_pFrameKey = L"PLAYER_FRONT";
		break;
	}
	Set_Frame(0, 50, 5, 1);
}

void CPlayer::Get_Item(ITEM&  _tItem)
{
	if (m_VecInven[_tItem.eInven].eSkillState == _tItem.eProSKill)
		return;
	
	CSoundMgr::Get_Instance()->Set_Sound(L"GET_SKILL.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
	m_VecInven[_tItem.eInven].eSkillState = _tItem.eProSKill;
	m_VecInven[_tItem.eInven].InvenImageKey = _tItem.pInvenImage;
	m_VecInven[_tItem.eInven].SKillImagekey = _tItem.pSkillImage;
	m_VecInven[_tItem.eInven].ImageEX = _tItem.pInvenEx;

	m_pPlayerSkillBar->Set_SKillICon(_tItem.eInven, _tItem.eProSKill, _tItem.pSkillImage);
}

void CPlayer::Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;
	for (auto& Sour : *_Sour)
	{
		if (PRO_SKILL_RUN == Get_ProSKill())
		{
			if (CCollisionMgr::Check_Sphere(_Dest, Sour, &fR, &fAngle))
			{
				if (
					CObj::STATE_CREATE != Sour->Get_State() &&
					CObj::STATE_STUN != Sour->Get_State() &&
					CObj::STATE_DEAD != Sour->Get_State() &&
					CObj::GOD_MODE_ACTIVE != Sour->Get_GodMode() &&
					CObj::STATE_CREATE != _Dest->Get_State())
				{
					Sour->Set_State(CObj::STATE_STUN);
					Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
					Sour->Plus_Hp(-_Dest->Get_Attack());
					fAngle = fAngle * 180 / PI;
					fAngle = float((int)fAngle % 360);

					float fX = (float)(Sour->Get_Info().fX + fR * 0.5 * cosf(fAngle * PI / 180));
					float fY = (float)(Sour->Get_Info().fY - fR * 0.5 * sinf(fAngle * PI / 180));
					CObjMgr::Get_Instance()->AddObject(EFFECT_KEY, EFFECT_HIT, CAbstactFactory<CHitEffect>::Create(fX, fY));

					if (0 >= Sour->Get_Hp())
						CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_DIED_1.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
					else
					{
						static int iSoundIndex = 0;
						if (0 == iSoundIndex)
						{
							CSoundMgr::Get_Instance()->Set_Sound(L"HIT_SOUND_NORMAL_1.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
							iSoundIndex = 1;
						}
						else if (1 == iSoundIndex)
						{
							CSoundMgr::Get_Instance()->Set_Sound(L"HIT_SOUND_NORMAL_2.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
							iSoundIndex = 0;
						}

					}
				}
			}
		}
	}
}
void CPlayer::Collision_Sphere(CObj* _Dest, CObj* Sour)
{
	static int iSoundIndex = 0;
	float fR = 0.f;
	float fAngle = 0.f;

	if (40 <= abs(Sour->Get_Info().fX - _Dest->Get_Info().fX) &&
		40 <= abs(Sour->Get_Info().fY - _Dest->Get_Info().fY))
		return;

	if (CCollisionMgr::Check_Sphere(_Dest, Sour, &fR, &fAngle))
	{
		if (
			CObj::STATE_CREATE != Sour->Get_State() &&
			CObj::STATE_STUN != Sour->Get_State() &&
			CObj::STATE_DEAD != Sour->Get_State() &&
			CObj::GOD_MODE_ACTIVE != Sour->Get_GodMode() &&
			CObj::STATE_CREATE != _Dest->Get_State())
		{
			Sour->Set_State(CObj::STATE_STUN);
			Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
			Sour->Plus_Hp(-_Dest->Get_Attack());

			fAngle = fAngle * 180 / PI;
			fAngle = float((int)fAngle % 360);

			float fX = (float)(Sour->Get_Info().fX + fR * 0.5 * cosf(fAngle * PI / 180));
			float fY = (float)(Sour->Get_Info().fY - fR * 0.5 * sinf(fAngle * PI / 180));
			CObjMgr::Get_Instance()->AddObject(EFFECT_KEY, EFFECT_HIT, CAbstactFactory<CHitEffect>::Create(fX, fY));


			if (0 == iSoundIndex)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_ICE_HIT_1.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 1;
			}
			else if (1 == iSoundIndex)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_ICE_HIT_2.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 2;
			}
			else if (2 == iSoundIndex)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_ICE_HIT_3.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 0;
			}
		}
	}
}

void CPlayer::Set_PlayerGold(int iGold) {
	m_iGold += iGold;
	m_pGoldBar->Set_Gold(iGold);
}

void CPlayer::ProcessCombo()
{
	if (COMBOX_COUNT <= m_ComboQue.size())
	{
		m_iComboCount = 0;
		int iQueCount = m_ComboQue.size();

		for (int i = 0; i < iQueCount; ++i)
		{
			TCHAR* ch = m_ComboQue.front();

			if (0 == lstrcmp(ch, m_szVKL) && 0 == m_iComboCount)
			{
				m_iComboCount = 1;
			}
			else if (0 == lstrcmp(ch, m_szVKL) && 1 == m_iComboCount)
			{
				m_iComboCount = 2;
			}
			else if (0 == lstrcmp(ch, m_szSPACE) && 2 == m_iComboCount)
			{
				m_iComboCount = 3;
			}
			else if (0 == lstrcmp(ch, m_szSPACE) && 3 == m_iComboCount)
			{
				m_iComboCount = 4;
			}
			else if (0 == lstrcmp(ch, m_szVKL) && 4 == m_iComboCount)
			{
					m_iComboCount = 0;
				
				float fX = 0.f, fY = 0.f, fR = 0.f;
				for (int i = 0; i < BOSS; ++i)
				{
					list<CObj*>* plist = m_pObjMgr->Get_TargetList(MONSTER_KEY, i);
					for (auto& elem : *plist)
					{
						fX = Displacement<float>(m_tInfo.fX, elem->Get_Info().fX);
						fY = Displacement<float>(elem->Get_Info().fX, m_tInfo.fY);
						fR = Distance<float>(fX, fY);

							m_pObjMgr->AddObject(SKILL_KEY, SKILL_COMBO, CAbstactFactory<CCombo>::Create((float)(elem->Get_Info().fX - elem->Get_Info().fCX * 0.5 - FCX_PLAYER * 0.5),
								(float)(elem->Get_Info().fY)));
							m_pObjMgr->Get_Target(SKILL_KEY, SKILL_COMBO, nullptr)->Set_Target(elem);

					}
				}

				CSoundMgr::Get_Instance()->Set_Sound(L"ULT_USE.mp3", SOUND_PLAYER, PLAYER_VOLUME);

				m_iComboCount = 0;
				queue<TCHAR*>().swap(m_ComboQue);
			}
			else
			{
				m_iComboCount = 0;
			}
			if(!m_ComboQue.empty())
				m_ComboQue.pop();
		}
		
	}
}

void	CPlayer::Process_Potion(int _iHp) 
{
	Set_Hp(_iHp);
	m_pPlayerHpBar->Process_HpBar(m_iHp);
}
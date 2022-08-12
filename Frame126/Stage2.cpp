#include "stdafx.h"
#include "Stage2.h"
#include "ObjMgr.h"
#include "AbstactFactory.h"
#include "Player.h"
#include "SwordMan.h"
#include "ArcherMan.h"
#include "Wizard.h"
#include "ScrollMgr.h"
#include "DObjMgr.h"
#include "TileMgr.h"
#include "WizardBall.h"
#include "MiddleBoss.h"
#include "Boss.h"
#include "KeyMgr.h"
#include "ItemFDragon.h"
#include "ItemIceBlast.h"
#include "ItemIceCrystal.h"
#include "TelePort.h"
#include "UserHeader.h"
//헤더 정리 필요
CStage2::CStage2()
{
}


CStage2::~CStage2()
{
	Release();
}

void CStage2::Initialize(void)
{
	Insert_Text();
	CObjMgr::Get_Instance()->Initialize(MONSTER_KEY);
	//CObjMgr::Get_Instance()->Initialize(UI_KEY);
	CObjMgr::Get_Instance()->Initialize(SKILL_KEY);
	CObjMgr::Get_Instance()->Initialize(EFFECT_KEY);

	//CScrollMgr::Get_Instance()->Set_ScrollX(-FX_STAGE_2_PLAYER + WINCX * 0.5);
	//CScrollMgr::Get_Instance()->Set_ScrollY(-FY_STAGE_2_PLAYER + WINCY * 0.5);
	//CObjMgr::Get_Instance()->AddObject(OBJ_KEY, OBJ_PLAYER, CAbstactFactory<CPlayer>::Create(FX_STAGE_2_PLAYER, FY_STAGE_2_PLAYER));
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(FX_STAGE_2_PLAYER, FY_STAGE_2_PLAYER);
	CObjMgr::Get_Instance()->AddObject(UI_KEY, UI_TELEPORT, CAbstactFactory<CTelePort>::Create(FX_STAGE_2_PLAYER, FY_STAGE_2_PLAYER));
	CObjMgr::Get_Instance()->AddObject(UI_KEY, UI_TELEPORT, CAbstactFactory<CTelePort>::Create(FX_TELEPORT_END, FY_TELEPORT_END));
	dynamic_cast<CTelePort*>(CObjMgr::Get_Instance()->Get_Target(UI_KEY, UI_TELEPORT, nullptr))->Set_Opt(CTelePort::TELE_OPT_ENDING);
	CTileMgr::Get_Instance()->Initialize();
	CTileMgr::Get_Instance()->Load_Process2();
	InitializeSound();
	//CScrollMgr::Get_Instance()->Update_ScrollX(FX_STAGE_2_PLAYER );
	//CScrollMgr::Get_Instance()->Update_ScrollY(FY_STAGE_2_PLAYER );

}
void CStage2::InitializeSound(void)
{
	CSoundMgr::Get_Instance()->Set_Bgm(L"DUNGEON_BGM.mp3", 1.f);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/DASH_1.mp3", SOUND_PLAYER, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/DASH_2.mp3", SOUND_PLAYER, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/DASH_3.mp3", SOUND_PLAYER, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/DASH_4.mp3", SOUND_PLAYER, PLAYER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/RUN_1.mp3", SOUND_PLAYER, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/RUN_2.mp3", SOUND_PLAYER, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/RUN_3.mp3", SOUND_PLAYER, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/RUN_4.mp3", SOUND_PLAYER, PLAYER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/NORMAL_ATTACK_1.mp3", SOUND_DASH, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/NORMAL_ATTACK_2.mp3", SOUND_DASH, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/NORMAL_ATTACK_3.mp3", SOUND_DASH, PLAYER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ICE_BLAST_1.mp3", SOUND_ICEBLAST, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ICE_BLAST_2.mp3", SOUND_ICEBLAST, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ICE_BLAST_3.mp3", SOUND_ICEBLAST, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ICE_BLAST_4.mp3", SOUND_ICEBLAST, PLAYER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ICE_KRYSTAL.mp3", SOUND_ISHIELD, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ICE_KRYSTAL_START.mp3", SOUND_PLAYER, PLAYER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_1.mp3", SOUND_FDRAGONE, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_2.mp3", SOUND_FDRAGONE, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_3.mp3", SOUND_FDRAGONE, PLAYER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/WALL_HITTED_FIREDRAGON_1.mp3", SOUND_EFFECT_BACK, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/WALL_HITTED_FIREDRAGON_2.mp3", SOUND_EFFECT_BACK, PLAYER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/PLAYER_HITED_1.mp3", SOUND_PLAYER, PLAYER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_DIE_1.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_DIE_2.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_DIE_3.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_HITTED_ENEMY_1.mp3", SOUND_SWORDMAN, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_HITTED_ENEMY_2.mp3", SOUND_SWORDMAN, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_HITTED_ENEMY_3.mp3", SOUND_SWORDMAN, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_HITTED_ENEMY_4.mp3", SOUND_SWORDMAN, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_HITTED_ENEMY_5.mp3", SOUND_SWORDMAN, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_HITTED_ENEMY_6.mp3", SOUND_SWORDMAN, MONSTER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ENEMY_HITTED_WIND_1.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ENEMY_HITTED_WIND_2.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ENEMY_HITTED_WIND_3.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ENEMY_HITTED_WIND_4.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ENEMY_HITTED_ICE_1.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ENEMY_HITTED_ICE_2.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ENEMY_HITTED_ICE_3.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ENEMY_HITTED_ICE_4.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/HIT_SOUND_NORMAL_1.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/HIT_SOUND_NORMAL_2.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
	//Monster
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/SWORDMAN_ATTACK.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME_BACK);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/SWORDMAN_RUN_1.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME_BACK);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/SWORDMAN_RUN_2.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME_BACK);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ARCHER_AIM.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME_BACK);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ARCHER_SHOOT.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME_BACK);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BALL_ATTACK.mp3", SOUND_BALL, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BALL_ATTACKMODE.mp3", SOUND_BALL, MONSTER_VOLUME);

	//Boss

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_BGM.mp3", SOUND_BGM, BGM_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_DROP.mp3", SOUND_BOSS, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_POSE.mp3", SOUND_BOSS, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_READYJUMP.mp3", SOUND_BOSS, BOSS_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ROCK_THROW_START.mp3", SOUND_BOSS, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ROCK_THROW_1.mp3", SOUND_BOSS, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ROCK_THROW_2.mp3", SOUND_BOSS, BOSS_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/COLUMN_1.mp3", SOUND_BOSS_BACK, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/COLUMN_2.mp3", SOUND_BOSS_BACK, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/COLUMN_3.mp3", SOUND_BOSS_BACK, BOSS_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_FIRE_HIT_1.mp3", SOUND_BOSS, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_FIRE_HIT_2.mp3", SOUND_BOSS, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_FIRE_HIT_3.mp3", SOUND_BOSS, BOSS_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_ICE_HIT_1.mp3", SOUND_BOSS, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_ICE_HIT_2.mp3", SOUND_BOSS, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_ICE_HIT_3.mp3", SOUND_BOSS, BOSS_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_WIND_HIT_1.mp3", SOUND_BOSS, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_WIND_HIT_2.mp3", SOUND_BOSS, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_WIND_HIT_3.mp3", SOUND_BOSS, BOSS_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_WIND_HIT_4.mp3", SOUND_BOSS, BOSS_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ROCK_THROW_START.mp3", SOUND_BOSS, BOSS_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/BOSS_FIRE_HIT_1.mp3", SOUND_BOSS_BACK, BOSS_VOLUME_BACK);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ENEMY_DIED_1.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ENEMY_DIED_2.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ENEMY_DIED_3.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/CARD_SUMMON.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
	//etc
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/CHANGE_SKILL.mp3", SOUND_UI, UI_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/CLOSE_INVENTORY.mp3", SOUND_UI, UI_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/OPEN_INVENTORY.mp3", SOUND_UI, UI_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/Success.mp3", SOUND_UI, UI_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/WIN.mp3", SOUND_UI, UI_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/GAIA_ARMOR_START.mp3", SOUND_GAIA_ARMOR, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/GAIA_ARMOR_HIT_1.mp3", SOUND_GAIA_ARMOR, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/GAIA_ARMOR_HIT_2.mp3", SOUND_GAIA_ARMOR, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/GAIA_ARMOR_HIT_3.mp3", SOUND_GAIA_ARMOR, PLAYER_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/GAIA_ARMOR_HIT_4.mp3", SOUND_GAIA_ARMOR, PLAYER_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/ULT_USE.mp3", SOUND_PLAYER, PLAYER_VOLUME);


	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/GET_SKILL.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/FIRE_DRAGON_DIE_2.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);

	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/MONEY_TAKE.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);

}

void CStage2::Insert_Text()
{
	//Player
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/Player_WIND.bmp", L"Player_WIND");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/FRONT_COMPLETE.bmp", L"PLAYER_BACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/BACK_COMPLETE.bmp", L"PLAYER_FRONT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/LEFT_COMPLETE.bmp", L"PLAYER_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/RIGHT_COMPLETE.bmp", L"PLAYER_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/UI_PLAYERBAR.bmp", L"UI_PLAYER_BAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/UI_HPBAR.bmp", L"UI_PLAYER_HP_BAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/UI_MANABAR.bmp", L"UI_PLAYER_MANA_BAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/UI_SKILLBAR.bmp", L"UI_SKILLBAR");
	//Monster
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/SUMMON_CARD_SWORDMAN.bmp", L"SWORDMAN_CARD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/SWORDMAN_RIGHT.bmp", L"SWORDMAN_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/SWORDMAN_LEFT.bmp", L"SWORDMAN_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/SUMMON_CARD_ARCHER.bmp", L"SUMMON_CARD_ARCHER");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/ARCHER_RIGHT.bmp", L"ARCHER_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/ARCHER_LEFT.bmp", L"ARCHER_LEFT");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/SUMMON_CARD_WIZARD.bmp", L"SUMMON_CARD_WIZARD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/WIZARD_RIGHT.bmp", L"WIZARD_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/WIZARD_LEFT.bmp", L"WIZARD_LEFT");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/SUMMON_CARD_ARCHER.bmp", L"SUMMON_CARD_ARCHER");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/ARCHER_RIGHT.bmp", L"ARCHER_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/ARCHER_LEFT.bmp", L"ARCHER_LEFT");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/SUMMON_CARD_WIZARDBALL.bmp", L"SUMMON_CARD_WIZARDBALL");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/WIZARD_BALL.bmp", L"WIZARD_BALL");

	//MiddleBoss
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/MIDDLE_BOSS_RIGHT.bmp", L"MIDDLE_BOSS_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/MIDDLE_BOSS_LEFT.bmp", L"MIDDLE_BOSS_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/MIDDLE_BOSS_RIGHT.bmp", L"MIDDLE_BOSS_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/MIDDLE_BOSS_LEFT.bmp", L"MIDDLE_BOSS_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/MIDDLEBOSS_NAMEBAR.bmp", L"MIDDLEBOSS_NAMEBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/BOSS_HPBAR.bmp", L"BOSS_HPBAR");
	//Boss
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BOSS_ATTACK.bmp", L"BOSS_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BOSS_LEFT.bmp", L"BOSS_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BOSS_RIGHT.bmp", L"BOSS_RIGHT");

	//Boss UI and Effect
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BOSS_BOXATTACK.bmp", L"BOSS_BOXATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BOSS_HPBAR.bmp", L"BOSS_HPBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BOSS_NAMEBAR.bmp", L"BOSS_NAMEBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BOSS_SKILL.bmp", L"BOSS_SKILL");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/BOTTOM_HOLE.bmp", L"BOTTOM_HOLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/SOIL_EFFECT.bmp", L"SOIL_EFFECT");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/MIDDLEBOSS_NAMEBAR.bmp", L"MIDDLEBOSS_NAMEBAR");
	//CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/BOSS_HPBAR.bmp", L"BOSS_HPBAR");

	//Skill
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/Lightning.bmp", L"Skill_THUNDER");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/IceCrystals.bmp", L"Skill_ICECRYSTAL");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/ICE_CRYSTAL.bmp", L"Skill_ICESHEILD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/SKILL_FIREDRAGON_COM.bmp", L"FIREDRAGON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/ICE_BLAST.bmp", L"ICE_BLAST");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/PLAYER_ATTACK.bmp", L"SKILL_SLASH");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/SWORDMAN_ATTACK.bmp", L"SWORDMAN_ATTACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/WIZARD_FIRE.bmp", L"WIZARD_FIRE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/FIRE_BLAST.bmp", L"FIRE_BLAST");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/GRAND_CRYSTAL.bmp", L"GRAND_CRYSTAL");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Skill/GAIA_ARMOR.bmp", L"GAIA_ARMOR");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/STATUE.bmp", L"STATUE");
	//효과
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FIRE_PARTICLE.bmp", L"FIRE_PARTICLE");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/SUMMON.bmp", L"SUMMON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/HITEFFECT.bmp", L"HITEFFECT");
	//UI
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/SUMMON.bmp", L"SUMMON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/HITEFFECT.bmp", L"HITEFFECT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/ARCHER_BOW_RIGHT.bmp", L"ARCHER_BOW_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/ARCHER_BOW_LEFT.bmp", L"ARCHER_BOW_LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/INVENTORY.bmp", L"INVENTORY");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/DASH_ICON.bmp", L"DASH_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/DASH_SKILLBAR.bmp", L"DASH_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/NORMAL_ATTACK_ICON.bmp", L"NORMAL_ATTACK_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/ICE_KRYSTAL_ICON.bmp", L"ICE_KRYSTAL_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FIRE_DRAGON_ICON.bmp", L"FIRE_DRAGON_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/ICE_BLAST_ICON.bmp", L"ICE_BLAST_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/SELECT_ICON.bmp", L"SELECT_ICON");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/GAIA_ARMOR_ICON.bmp", L"GAIA_ARMOR_ICON");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/NORMAL_ATTACK_SKILLBAR.bmp", L"NORMAL_ATTACK_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FIRE_DRAGON_SKILLBAR.bmp", L"FIRE_DRAGON_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/ICE_KRYSTAL_SKILLBAR.bmp", L"ICE_KRYSTAL_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/ICE_BLAST_SKILLBAR.bmp", L"ICE_BLAST_SKILLBAR");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/COOLTIME_SHADOW.bmp", L"COOLTIME_SHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/GAIA_ARMOR_SKILLBAR.bmp", L"GAIA_ARMOR_SKILLBAR");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FIRE_DRAGON_CARD.bmp", L"FIRE_DRAGON_CARD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/ICE_KRYSTAL_CARD.bmp", L"ICE_KRYSTAL_CARD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/ICE_BLAST_CARD.bmp", L"ICE_BLAST_CARD");
	//타일
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Edit/FireTileSet_1024.bmp", L"Tile");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/TELEPORT.bmp", L"TELEPORT");

	//기타
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FONT0.bmp", L"FONT_0");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FONT1.bmp", L"FONT_1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FONT2.bmp", L"FONT_2");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FONT3.bmp", L"FONT_3");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FONT4.bmp", L"FONT_4");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FONT5.bmp", L"FONT_5");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FONT6.bmp", L"FONT_6");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FONT7.bmp", L"FONT_7");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FONT8.bmp", L"FONT_8");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FONT9.bmp", L"FONT_9");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/UI_MONEY.bmp", L"UI_MONEY");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/MONEY.bmp", L"MONEY");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/GAIA_ARMOR_CARD.bmp", L"GAIA_ARMOR_CARD");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/ITEMSHOP_NPC.bmp", L"ITEMSHOP_NPC");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/POTION.bmp", L"POTION");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/SHOP_TABLE.bmp", L"SHOP_TABLE");

	//아이템 설명
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/NORMAL_ATTACK_EX.bmp", L"NORMAL_ATTACK_EX");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/DASH_EX.bmp", L"DASH_EX");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/ICE_KRYSTAL_EX.bmp", L"ICE_KRYSTAL_EX");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/ICE_BLAST_EX.bmp", L"ICE_BLAST_EX");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/FIRE_DRAGON_EX.bmp", L"FIRE_DRAGON_EX");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/GAIA_ARMOR_EX.bmp", L"GAIA_ARMOR_EX");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/NORMAL_SHADOW.bmp", L"NORMAL_SHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/BIG_SHADOW.bmp", L"BIG_SHADOW");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Etc/BIG_SHADOW_VER.bmp", L"BIG_SHADOW_VER");

}
void CStage2::Update(void)
{
	CObjMgr::Get_Instance()->Update(OBJ_KEY);
	CObjMgr::Get_Instance()->Update(SKILL_KEY);
	CObjMgr::Get_Instance()->Update(MONSTER_KEY);
	CObjMgr::Get_Instance()->Update(EFFECT_KEY);
	CObjMgr::Get_Instance()->Update(UI_KEY);
	CTileMgr::Get_Instance()->Update();
}

void CStage2::Late_Update(void)
{
	CObjMgr::Get_Instance()->Late_Update(OBJ_KEY);
	CObjMgr::Get_Instance()->Late_Update(SKILL_KEY);
	CObjMgr::Get_Instance()->Late_Update(MONSTER_KEY);
	CTileMgr::Get_Instance()->Late_Update();
	CObjMgr::Get_Instance()->Late_Update(UI_KEY);
	CObjMgr::Get_Instance()->Late_Update(EFFECT_KEY);
}

void CStage2::Render(HDC _DC)
{
	CTileMgr::Get_Instance()->Render(_DC);
	CObjMgr::Get_Instance()->Render(_DC, MONSTER_KEY);
	CObjMgr::Get_Instance()->Render(_DC, UI_KEY);
	CObjMgr::Get_Instance()->Render(_DC, OBJ_KEY);
	CObjMgr::Get_Instance()->Render(_DC, SKILL_KEY);
	CObjMgr::Get_Instance()->Render(_DC, EFFECT_KEY);
}

void CStage2::Release(void)
{
	CTileMgr::Get_Instance()->Release();
	CObjMgr::Get_Instance()->Release(SKILL_KEY);
	CObjMgr::Get_Instance()->Release(EFFECT_KEY);
	CObjMgr::Get_Instance()->Release(UI_KEY);
	CObjMgr::Get_Instance()->Release(MONSTER_KEY);
	CScrollMgr::Get_Instance()->Destroy_Instance();
}

#pragma once

#define			WINCX						800
#define			WINCY						600

#define			PI							3.141592f
#define			PURE						= 0

#define			OBJ_NOEVENT					0
#define			OBJ_DEAD					1

#define			VK_MAX						0xff

#define			TILECX						32
#define			TILECY						32
#define			CTILECX						32
#define			CTILECY						32
#define			TILEX						114
#define			TILEY						80

#define			COLLISION_TILECX			64
#define			COLLISION_TILECY			64

#define			CAL_TILECX					0.03125
#define			CAL_TILECY					0.03125

#define			CULLING_DISTANCE			64
#define			DECO_CAPACITY               100

#define			_USER_DEBUG					1
#define			_DEBUG_ACITVE_TICK			1000
#define			_DEBUG_UNACITVE_TICK		10000

#define			RANGE_FIRE_DRAGON			600

#define			DASH_CNT					3
#define			RUN_CNT					    10
#define			FDRAGONE_CNT			    8
#define			ICE_BLAST_CNT				18
#define			ISHEILD_CNT					40
#define			GAIA_ARMOR_CNT				40

#define			DASH_TIME				    500
#define			RUN_TIME				    25
#define			FDRAGONE_TIME				700
#define			ICEBLAST_TIME				300
#define			ISHEILD_TIME				500
#define			GAIA_ARMOR_TIME				500
#define			NOT_USED_TIME				10000
//∏ÛΩ∫≈Õ
#define			SWORD_MAN_PLAYER_DETECT		500
#define			SWORD_MAN_PLAYER_ATTCK		50

#define			ARCHER_MAN_PLAYER_DETECT	800
#define			ARCHER_MAN_PLAYER_ATTCK		500
#define			ARROW_DISTNACE				500

#define			WIZARD_MAN_PLAYER_DETECT	600
#define			WIZARD_MAN_PLAYER_ATTCK		400
#define			WIZARD_FIRE_DISTNACE		400

#define			WIZARD_BALL_PLAYER_DETECT	800
#define			WIZARD_BALL_PLAYER_ATTCK	400
#define			WIZARD_BALL_DISTNACE		400

#define			SWORD_MAN_ATTACK_TICK       1500
#define			SWORD_MAN_MOVE_TICK			2000

#define			ARCHER_MAN_ATTACK_TICK		3500
#define			WIZARD_ATTKC_TICK			2000
#define			WIZARD_BALL_ATTACK_TICK		2000

#define			MIDDLE_BOSS_PLAYER_DETECT   1000
#define			MIDDLE_BOSS_PLAYER_ATTCK    500
#define			MIDDLE_BOSS_ATTACK_TICK     1000

#define			BOSS_PLAYER_DETECT   1000
#define			BOSS_PLAYER_ATTCK    500
#define			BOSS_ATTACK_TICK     1000

#define			MIDDLE_BOSS_GODMODE_TICK    6000
#define			BOSS_GODMODE_TICK			6000

#define			FIRE_BLAST_BULLET_TICK		2000
#define			CREATE_FIRE_BLAST_TICK      100

#define			PLAYER_STUN_TICK			150
#define			MONSTER_STUN_TICK			150

#define			MIDDLE_BOSS_STATUE_DISTNACE		500

#define			PLAYER_HP					100
#define			MIDDLE_BOSS_HP				80
#define			BOSS_HP						140
#define			BOSS_HALF_HP				 70

#define         DISTANCE_SOIL_COLUM_EFFECT	33
#define         DISTANCE_JUMP_EFFECT		100.f
#define			DISTANCE_COLUM_SKILL		800

#define			BOTTOM_HOLE_TICK            8000

#define			BOSS_STON_TICK				500
#define         DISTANCE_SOIL_BOX_EFFECT	50
#define			DISTANCE_SOIL_BOX			800

#define			PRISON_TICK					1000

#define			PRISON_MONSTER_RESPONE		30000
#define			TILE_EVENT_SPPED			5.f

#define			GAIA_ARMOR_GOLD				150
#define			ITEM_POTIN_GOLD				100

#define			COMBOX_COUNT				5

#define			COMBOX_COLLISION_RADIUS    40.f

#define			DISTANCE_ICE_BLAST			300.f
#define			DISTANCE_DEYSTROY_ICE_BLAST	 30.f

#define			MONSTER_RESPONE_TIME       5000

#define			SWORD_MAN_HP			   20
#define			ARCHER_MAN_HP			   20
#define			WIZARD_HP				   20
#define			WIZARD_BALL_HP			   20

#define			PLAYER_INIT_GOLD           100

#define			PRISON_RELASE_TIME		   15000
extern		HWND	g_hWnd;
extern      HINSTANCE hInst;
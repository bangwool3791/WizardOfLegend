#pragma once

#include "Include.h"

class CObjMgr;
class CObj abstract
{
public:
	CObj();
	virtual ~CObj();

public:
	static float g_fSound;
protected:
	CObj* m_pPlayer;
	CObjMgr* m_pObjMgr;
	TCHAR* m_pPrePrimaryKey;
protected:
	DWORD				m_dAttackTick;
	DWORD				m_dStunTick;
public :
	void	Set_AttackTick(const DWORD& _tick) { m_dAttackTick = _tick; }
	const DWORD&	Get_AttackTick() { return m_dAttackTick; }
	void	Set_StunTick(const DWORD& _tick) { m_dStunTick = _tick; }
	const DWORD&	Get_StunkTick() { return m_dStunTick; }
public :
	enum DEAD_ITEM{DEAD_ITEM_GOLD, DEAD_ITEM_FIRE_DRAGONE, DEAD_ITEM_ICE_BLAST, DEAD_ITEM_ICE_CRYSTAL, DEAD_ITEM_END};
	void Set_DeadItem(DEAD_ITEM _item) { m_eDeadItem = _item; }
	DEAD_ITEM GEt_DeadItem() { return m_eDeadItem; }
	int Get_Gold() { return m_iGold; }
	void Set_Gold(int _iGold) { m_iGold += _iGold; }
protected :
	DEAD_ITEM m_eDeadItem = DEAD_ITEM_GOLD;
	int			m_iGold = 0;
public:
	enum GOD_MODE_STATE { GOD_MODE_ACTIVE, GOD_MODE_UNACTIVE };
protected:
	GOD_MODE_STATE m_eGodModeState = GOD_MODE_UNACTIVE;
	GOD_MODE_STATE m_ePreGodModeState = GOD_MODE_UNACTIVE;
public :
	void Set_GodMode(const GOD_MODE_STATE& _eState) { m_eGodModeState = _eState; }
	const GOD_MODE_STATE& Get_GodMode() { return m_eGodModeState; }
	void Set_PreGodMode(const GOD_MODE_STATE& _eState) { m_ePreGodModeState = _eState; }
	const GOD_MODE_STATE& Get_PreGodMode() { return m_ePreGodModeState; }
public:
public:

	enum STATE { STATE_CREATE, STATE_MOVE ,STATE_DETECT, STATE_READY, STATE_ATTACK, STATE_SKILL, STATE_STUN, STATE_GOD_MODE, STATE_DEAD, STATE_END };
	enum DAMAGE_STATE{DAMAGE_STATE_STUNED, DAMAGE_STATE_DEAD, DAMAGE_STATE_WAKE};
	enum IDLE_STATE { IDLE_UP, IDLE_LEFT, IDLE_DOWN, IDLE_RIGHT, IDLE_END };
	enum MOVE {
		IDLE,
		MOVE_LEFT, MOVE_RIGHT, MOVE_UP, MOVE_DOWN,
		MOVE_LEFT_UP, MOVE_LEFT_DOWN,
		MOVE_UP_LEFT, MOVE_UP_RIGHT,
		MOVE_RIGHT_UP, MOVE_RIGHT_DOWN,
		MOVE_DOWN_LEFT, MOVE_DOWN_RIGHT, MOVE_READY,
		MOVE_END = 0xFF
	};
protected:
	STATE			m_eState = STATE_END;
	STATE			m_ePreState = STATE_END;
	MOVE			m_eMoveState = MOVE_END;
	MOVE			m_eMovePreState = MOVE_END;
	IDLE_STATE		m_eIdleState = IDLE_END;
	IDLE_STATE		m_eIdlePreState = IDLE_END;
	IDLE_STATE		m_eRunState = IDLE_END;
	DAMAGE_STATE			m_eDamageState = DAMAGE_STATE_WAKE;
	DAMAGE_STATE			m_ePreDamageState = DAMAGE_STATE_WAKE;
	void			Clear_Damage_PreState() {

		if (DAMAGE_STATE_WAKE != m_ePreDamageState)
			m_ePreDamageState = DAMAGE_STATE_WAKE;
	}
	void			Clear_Move_PreState(){

		if (MOVE_END != m_eMovePreState)
			m_eMovePreState = MOVE_END;
	}
	void			Set_IdleState(IDLE_STATE _eState) { m_eIdleState = _eState; }
public :
	void Set_DamageState(const DAMAGE_STATE& _eState) { m_eDamageState = _eState; Clear_Damage_PreState();}
	void Set_MoveState(const MOVE& _eState) { m_eMoveState = _eState; }
	void Set_State(const STATE& _eState);
	const STATE& Get_State()const { return m_eState; }
	void Set_PreState(const STATE& _eState) { m_ePreState = _eState; }
	const STATE& Get_PreState()const { return m_ePreState; }
public:
	void			Set_Pos(float _fX, float _fY)
	{
		m_tInfo.fX = _fX;
		m_tInfo.fY = _fY;
		Update_Rect();
	}

	void		Set_PosX(const float& _fX) {m_tInfo.fX += _fX; }
	void		Set_PosY(const float& _fY) { m_tInfo.fY += _fY; }

	void		Set_TilePosX(float _fX) { m_tInfo.fX = _fX; }
	void		Set_TilePosY(float _fY) { m_tInfo.fY = _fY; }

	void		Set_Direction(DIR eDir) { m_eDir = eDir; }
	void		Set_Dead(void) { m_bDead = true; }
	bool		Get_Dead(void) { return m_bDead; }
	void		Set_Angle(const float& _fAngle) { m_fAngle = _fAngle; }
	void		Set_Target(CObj* pTarget) { m_pTarget = pTarget; }
	void		Set_FrameKey(TCHAR* pFrameKey) { m_pFrameKey = pFrameKey; }


	const INFO&			Get_Info(void) const { return m_tInfo; }
	void				Set_Area(const float& _fCX, const float& _fCY) { m_tInfo.fCX = (float)_fCX; m_tInfo.fCY = (float)_fCY; }
	const RECT&			Get_Rect(void) const { return m_tRect; }

public:
	virtual void		Initialize(void)PURE;
	virtual int			Update(void)PURE;
	virtual void		Late_Update(void)PURE;
	virtual void		Render(HDC hDC)PURE;
	virtual void		Release(void)PURE;

public:
	void			Update_Rect(void);
	bool			InCrease_X_Frame(FRAME& _tFrame);
	bool			InCrease_Init_X_Frame(FRAME& _tFrame);
	bool			InCrease_X_FrameWithY(FRAME& _tFrame);
	bool			DeCrease_X_Frame(FRAME& _tFrame);
public :
	void Set_Frame(const int& _start, const DWORD& _speed, const int& _motion, const int& _end)
	{
		m_tFrame.iFrameStart = _start;
		m_tFrame.dwFrameSpeed = _speed;
		m_tFrame.iFrameMotion = _motion;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.iFrameEnd = _end;
	}

	void Set_Frame(const int& _start, const DWORD& _speed, const int& _motion, const int& _end, const int& _init)
	{
		m_tFrame.iFrameStart = _start;
		m_tFrame.dwFrameSpeed = _speed;
		m_tFrame.iFrameMotion = _motion;
		m_tFrame.dwFrameTime = GetTickCount();
		m_tFrame.iFrameInit = _init;
		m_tFrame.iFrameEnd = _end;
	}
protected:
	INFO			m_tInfo;
	RECT			m_tRect;	
	FRAME			m_tFrame;

	DIR				m_eDir;
	float			m_fSpeed;
	bool			m_bDead;
	float			m_fAngle;
	public:
	CObj*			m_pTarget;

	TCHAR*			m_pFrameKey = nullptr;

	SKILL			m_eSkillState = SKILL_END;
	SKILL			m_eSKillPreState = SKILL_END;
protected :
	int				m_iHp;
	int				m_iAttack;
	//CObj Á¤º¸ Get Set
public :
	const int    Get_PreSKillState() { return m_eSKillPreState; }
	void		Set_Option(const TILE_OPT& iOption) { m_tInfo.iOption = iOption; }
	const int	Get_Option() { return  m_tInfo.iOption; }
	void		Set_DrawXID(const int& iDrawID) { m_tInfo.iDrawXID = iDrawID; }
	void		Set_DrawYID(const int& iDrawID) { m_tInfo.iDrawYID = iDrawID; }
	int			Get_Hp() const { return m_iHp; }
	void		Plus_Hp(const int& _hp) { m_iHp += _hp; }
	void		Set_Hp(int _hp) { m_iHp = _hp; }
	int			Get_Attack() const { return m_iAttack; }
	void		Plus_Attack(const int& _Attack) { m_iAttack += _Attack; }
	void		Set_Attack(const int& _Attack) { m_iAttack = _Attack; }
	void		Set_PrimaryKey(TCHAR* _key) { m_pFrameKey = _key; }

public :
public:
	bool operator > (const CObj& obj) const
	{
		return m_tInfo.iOption > obj.Get_Info().iOption;
	}
public :
	const RENDERID	Get_RenderID() const { return m_eGroup; }
protected :
	RENDERID m_eGroup;
public :
	enum DEAD_EVENT {DEAD_EVENT_GOLD, DEAD_EVENT_CRYSTAL, DEAD_EVENT_FDRAGONE, DEAD_EVENT_ICEBLAST, DEAD_EVENT_END };
private :
	DEAD_EVENT m_eDeadEvent = DEAD_EVENT_GOLD;
public:
	const DEAD_EVENT& Get_DeadEvent() { return m_eDeadEvent; }
	void Set_DeadEvent(const DEAD_EVENT& _flag) { m_eDeadEvent = _flag; }
public :
	void Set_Width(float _fCX, float  _fCY) { m_tInfo.fCX = _fCX; m_tInfo.fCY = _fCY; }
};


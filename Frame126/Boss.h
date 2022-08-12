#pragma once
#include "Obj.h"
class CBossHpBar;
class CShadow;
class CBoss :
	public CObj
{
private :
	CShadow * m_pShadow = nullptr;
private:
	HDC m_hMemDC;
public:
	enum HP_STATE{HP_STATE_FULL, HP_STATE_HALF};
	HP_STATE m_eHpState;
	enum ATTACK_STATE { ATTACK_SKILL_COLUMN, ATTCK_CREATE_STONE, ATTCK_SHOOT_STONE, ATTACK_SKILL_JUMP_READY, ATTACK_SKILL_JUMP, ATTACK_SKILL_JUMP_STOP, ATTACK_SKILL_FALL, ATTACK_SKILL_JUMP_COLUMN, ATTACK_SKILL_STONE_COLUMN, ATTACK_STATE_END};
	ATTACK_STATE m_eAttackState = ATTACK_STATE_END;
	ATTACK_STATE m_ePreAttackState = ATTACK_STATE_END;
private :
	void Set_AttackState(const ATTACK_STATE& _eState) { m_eAttackState = _eState; }
	void Set_PreAttackState(const ATTACK_STATE& _eState) { m_ePreAttackState = _eState; }
public :
	enum BOSS_SKILL { BOSS_SKILL_COLUMN, BOSS_SKILL_STONE, BOSS_SKILL_JUMP, BOSS_SKILL_STONE_COLUMN, BOSS_SKILL_JUMP_COLUMN, BOSS_SKILL_END };
	BOSS_SKILL m_eBossSkill = BOSS_SKILL_END;
	BOSS_SKILL m_ePreBossSkill = BOSS_SKILL_END;
private:
	void Set_BossSkillMode(const BOSS_SKILL& _eState) { m_eBossSkill = _eState; }
	const BOSS_SKILL& Get_BossSkillMode() { return m_eBossSkill; }
private:
	list<CObj*> m_listBlast;
	DWORD m_dReadyTick;
public:
	CBoss();
	CBoss(float _fX, float _fY);
	virtual ~CBoss();
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private:
	DWORD					m_dSkillTick;
	DWORD					m_dGodModeTick;
	vector<SkillElem>		m_vecSKillElem;
public:
	float		m_fPlayerAngle;
	float		m_fPlayerRadius;
	POINT		m_tPlayerPoint;
	//UI
private:
	void Update_AttackFrame();
	void ProcessAttack();
	void ProcessMove();
	void ProcessDetect();
	void ProcessGodMode();
	void ProcessReady();
	void Update_Frame_Move();
	void Update_Frame_Damage(const DAMAGE_STATE& _eState);
	void Process_Dead();
	void Get_PlayerInfo(float * _fR, float* _fAngle);
	void Clear_Stun();
	void Clear_MoveState();
	void Clear_DetectState();
	void Clear_AttackState();
	void Clear_PreAttackState() { if (ATTACK_STATE_END != m_ePreAttackState)m_ePreAttackState = ATTACK_STATE_END; }
	bool Is_PlayerRange();
	void Collision_Sphere();
private:
	CBossHpBar* m_pBossHpBar;


	void CBoss::Collision_RectEx(vector<CObj*>* _Sour, CObj* _Dest);

};


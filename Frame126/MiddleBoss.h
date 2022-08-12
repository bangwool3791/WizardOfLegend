#pragma once
#include "Obj.h"
class CMiddleBossHpBar;
class CShadow;
class CMiddleBoss :
	public CObj
{
private :
	CShadow* m_pShadow = nullptr;
private:
	HDC m_hMemDC;
public:
	enum ATTACK_STATE { ATTACK_ACTIVE, ATTACK_UNACTIVE };
	ATTACK_STATE m_eAttackState = ATTACK_UNACTIVE;
	ATTACK_STATE m_ePreAttackState = ATTACK_UNACTIVE;
	enum WIZARD_BOSS_SKILL {BOSS_SKILL_CREAT_WIZARD_BALL, BOSS_SKILL_ICEBLAST, BOSS_SKILL_STATUE,BOSS_SKILL_END };
	WIZARD_BOSS_SKILL m_eBossSkill = BOSS_SKILL_END;
	WIZARD_BOSS_SKILL m_ePreBossSkill = BOSS_SKILL_END;
private :
	void Set_BossSkillMode(const WIZARD_BOSS_SKILL& _eState) { m_eBossSkill = _eState; }
	const WIZARD_BOSS_SKILL& Get_BossSkillMode() { return m_eBossSkill; }
private :
	list<CObj*> m_listBlast;
	DWORD m_dReadyTick;
public:
	CMiddleBoss();
	CMiddleBoss(float _fX, float _fY);
	virtual ~CMiddleBoss();
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
	void Clear_PreAttackState() { if (ATTACK_UNACTIVE != m_ePreAttackState)m_ePreAttackState = ATTACK_UNACTIVE; }
private :
	CMiddleBossHpBar* m_pBossHpBar;
};


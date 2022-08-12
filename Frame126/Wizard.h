#pragma once
#include "Obj.h"
class CWizard :
	public CObj
{
private:
	HDC m_hMemDC;
public:
	enum ATTACK_STATE { ACTIVE, UNACTIVE };
	ATTACK_STATE m_eAttackState = UNACTIVE;
	ATTACK_STATE m_ePreAttackState = UNACTIVE;
public:
	CWizard();
	CWizard(float _fX, float _fY);
	virtual ~CWizard();
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private:
	DWORD		m_dArrowTick;
	int			m_iFireBallCount;
public:
	void		Plus_FireBallCnt(const int& _iCount) { m_iFireBallCount += _iCount; }
	void		Set_FireBallCnt(const int& _iCount) { m_iFireBallCount = _iCount; }
	const int& Get_FireBallCnt() { return m_iFireBallCount; }
	float		m_fPlayerAngle;
	float		m_fWizardFireAngle;
	float		m_fPlayerRadius;
	POINT		m_tPlayerPoint;
	//UI
private:
	void Update_AttackFrame();
	void ProcessAttack();
	void ProcessMove();
	void ProcessDetect();
	void ProcessReady();
	void Update_Frame_Move();
	void Update_Frame_Damage(const DAMAGE_STATE& _eState);
	void Process_Dead();
	void Get_PlayerInfo(float * _fR, float* _fAngle);
	void Clear_Stun();
	void Clear_MoveState();
	void Clear_DetectState();
	void Clear_AttackState();
	void Clear_PreAttackState() { if (UNACTIVE != m_ePreAttackState)m_ePreAttackState = UNACTIVE; }
	bool Is_PlayerRange();

};


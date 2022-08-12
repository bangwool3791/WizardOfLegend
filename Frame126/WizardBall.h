#pragma once
#include "Obj.h"
class CWizardBall :
	public CObj
{
private:
	HDC m_hMemDC;
private :
	float m_fPlayerAngle;
	float m_fPlayerRaidus;
private :
	enum ATTACK_STATE{ACTIVE, UNACITVE};
	ATTACK_STATE m_eAttackState = UNACITVE;
public:
	CWizardBall();
	CWizardBall(float _fX, float _fY);
	virtual ~CWizardBall();
private:
	HPEN	m_hPen;
	HPEN	m_hOldPen;
	HDC		m_hDC;
public:
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private:
	DWORD		m_dReadyTick;
	POINT		m_tPlayerPoint;
private :
	void CWizardBall::Collision_Sphere(CObj* _Dest, CObj* _Sour);
	//void Collision_Player(CObj* _Dest, CObj* _Sour);

private :
	int			m_iYFrame;
	//UI
private:
	void ProcessAttack();
	void ProcessReady();
	void ProcessMove();
	void ProcessDetect();
	void ProcessStun();
	void Update_Frame_Object();
	void Process_Dead();
	void Get_PlayerInfo(float * _fR, float* _fAngle);
	void Clear_Stun();
	void Collision_Sphere(CObj* _Dest, list<CObj*>* Sour);

};


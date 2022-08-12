#pragma once
#include "Obj.h"
class CSwordMan :
	public CObj
{
private :
	DWORD m_dMoveTick = 0;
private :
	HDC m_hMemDC;
public :
	enum ATTACK_STATE{ACTIVE, UNACTIVE};
	ATTACK_STATE m_eAttackState = UNACTIVE;
	ATTACK_STATE m_ePreAttackState = UNACTIVE;
public:
	CSwordMan();
	CSwordMan(float _fX, float _fY);
	virtual ~CSwordMan();
public :
	virtual void		Initialize(void);
	virtual int			Update(void);
	virtual void		Late_Update(void);
	virtual void		Render(HDC hDC);
	virtual void		Release(void);
private :
	//플레이어 
private :
	void Update_AttackFrame();
	void ProcessAttack_SwordMan();
	void ProcessMove_Detect_Player();
	void ProcessAttck_Detect_Player();
	void Update_Frame_Move();
	void Update_Frame_Damage(const DAMAGE_STATE& _eState);
	void Process_Dead();
	void Get_PlayerInfo(float * _fR, float* _fAngle);
	void Clear_Stun();
	void Collision_RectEx(CObj* _Dest, list<CObj*>* _Sour);
};


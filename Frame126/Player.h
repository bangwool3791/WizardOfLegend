#pragma once
#include "Obj.h"
#include "BmpMgr.h"

class CInventory;
class CScrollMgr;
class CPlayerSKillBar;
class CPlayerBar;
class CGoldBar;
class CObjMgr;
class CShadow;
class CPlayer : public CObj
{
private :
	CShadow* m_pShadow = nullptr;
	TCHAR* m_pNormalShadow = L"NORMAL_SHADOW";
	TCHAR* m_pBigShadow = L"BIG_SHADOW";
	TCHAR* m_pBigVShadow = L"BIG_SHADOW_VER";
private :
	queue<TCHAR*> m_ComboQue;
	void ProcessCombo();
	int	m_iComboCount = 0;
	TCHAR* m_szVKL = L"VKL";
	TCHAR* m_szSPACE = L"SPACE";
	TCHAR* m_szVKR = L"VKR";
	TCHAR* m_szQ = L"Q";
	TCHAR* m_szE = L"E";
	TCHAR* m_szR = L"R";
private :
	CGoldBar* m_pGoldBar;
public:
	void Set_PlayerGold(int iGold);
private :
	HDC m_hMemDC;
private :
	//����� ���� �Լ�
	float Get_MouseAngle(const INFO& _tInfo);
	POINT Get_MousePlayerPoint(const INFO& _tInfo);
	//��� �̺�Ʈ �� �÷��̾� ���� �۾�
private:
	POINT m_tRecPoint;
public:
	void Process_Recover_Point();
	void Set_RecoverPoint(float _x, float _y);
	//�÷��̾��� ��ü���� ��ų �Լ�
private:
	void Process_Skill(INVENTORY _eInven, PRO_SKILL _eProSkill);
	bool Check_Process_Skil();
	//��ų ���� ����
private:
	float m_fDragonAngle;
	DWORD m_dSkillTick;
	DWORD m_dSkillMaxTick;
private:
	PRO_SKILL m_eProSKill;
	vector<INVEN> m_VecInven;
public :
	const PRO_SKILL& Get_ProSKill() { return m_eProSKill; }
	//�κ��丮 
private:
	vector<PRO_SKILL> m_vecSkill;
	//������ ���� �� ��ų ����
	void Set_ItemSkill(INVENTORY _eIven, PRO_SKILL _eProSkill);
	//��ų ��Ҹ� ���� ���� ���� �Լ�
public:
	void Plus_SkillCnt(INVENTORY _eInven, int _iCount);
	void Set_SkillCnt(INVENTORY _eInven, int _iCount);
public :
	const int Get_SkillMaxCnt(INVENTORY _eInven);
private:
	const DWORD Get_Skill_MaxTick(INVENTORY _eInven);
	const int Get_SkillCnt(INVENTORY _eInven);
	void Set_SkillTick(INVENTORY _eInven);
	const DWORD Get_SkillTick(INVENTORY _eInven);
	const SKILLID Get_Skill_ID(INVENTORY _eInven);
	void Set_Skill_ID(INVENTORY _eInven, SKILLID _eSKill_ID);
	void Set_Skill_Radius(INVENTORY _eInven, float _fRadius);
	void Plus_Skill_Radius(INVENTORY _eInven, float _fRadius);
	const float Get_Skill_Radius(INVENTORY _eInven);
	void Set_Skill_Angle(INVENTORY _eInven, float _fRadius);
	const float Get_Skill_Angle(INVENTORY _eInven);
	void Init_SkillCnt(INVENTORY _eInven, int _iCount);
public:
	CPlayer();
	CPlayer(float _fX, float _fY);
	virtual ~CPlayer();

public:
	virtual void Initialize(void) override;
	virtual int	 Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:
	void		Set_Skill_Motion(INVENTORY _eInven, SKILL _eSkillMotion);
	const SKILL& Get_Skill_Motion(INVENTORY _eInven);
	void		Create_Skill(SKILL _eState, int _iObj);
	void		Process_PlayerState(void);
	void		Update_Frame_Skill(SKILL _eState);
	void		Update_Frame_Move(MOVE _eState);
	void		Update_FrameStun();
	void		Key_Input(void);
	void		OffSet(void);
	void		Motion_Change(void);
	void		Insert_Texture(void);
	void		First_State(void);
	//�÷��̾��� ����, �̵� �Լ�
private:
	void Set_IdleState(IDLE_STATE _State);
	void Set_SkillState(SKILL _State, SKILLID _eSkillID);
	void Set_Move(float _xSpeed, float _ySpeed, float _fAngle, MOVE eMove);
	void Clear_Skill();
	void Clear_Move();
	void Clear_Stun();
	void Process_Detect_Mouse();
	void Set_InvenState(INVENTORY _eInven) { m_eInven = _eInven; }
	const INVENTORY Get_InvenState()const { return m_eInven; }
	void Set_PlayerDirection();
	//�÷��̾�, �̵�, ��ų ����, �κ��丮 ����
private:
	CBmpMgr*		m_pBmpMgr;
	INVENTORY		m_eInven = INVENTORY::INVEN_END;
	const float		m_SkillMoveSpeed = 2.0f;
	const float     m_SkillDashSpeed = 0.3f;
	void			Clear_Skill_State()
	{
		if (SKILL_END != m_eSKillPreState)
		{
			m_eSKillPreState = SKILL_END;
		}
		if (SKILL_END != m_eSkillState)
		{
			m_eSkillState = SKILL_END;
		}
	}

private :
	CPlayerBar* m_pPlayerHpBar;
	CPlayerSKillBar * m_pPlayerSkillBar;
	CScrollMgr*	m_pScroll;
	CInventory* m_pInven;
public :
	void Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour);
	void Collision_Sphere(CObj* _Dest, CObj* Sour);
	void Get_Item(ITEM&  _tItem);
private :
		bool  Collision_Lava(vector<CObj*>* _Sour);
public :
	void	Process_Potion(int _iHp);
};


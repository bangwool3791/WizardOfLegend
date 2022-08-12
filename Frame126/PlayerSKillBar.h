#pragma once
#include "Obj.h"
#include "ImageDefine.h"
class CPlayerSKillBar :
	public CObj
{
private :
	HDC m_hMemDC;
private :
	bool m_bProcess = false;
	void Set_ProcessFlag(const bool& _flag) { m_bProcess = _flag; }
	const bool& Get_ProcessFlag() { return m_bProcess; }
private :
	vector<INVEN>* m_pVecInven;
	vector<CObj*>  m_VecIconSkill;
public :
	void Set_Inven(vector<INVEN>* _pVecInven);
public:
	CPlayerSKillBar();
	virtual ~CPlayerSKillBar();
public:
	virtual void Initialize(void) override;
	virtual int	 Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:
	void Process_Skill_Timer();
public :
	void CPlayerSKillBar::Set_SKillICon(const INVENTORY& _eInven, PRO_SKILL& _eProSkill, TCHAR* _pSkillImage);
};


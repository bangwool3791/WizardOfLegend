#pragma once
#include "Obj.h"
#include "Struct.h"
class CFireDragon :
	public CObj
{
private :
	HDC m_hMemDC;
public :
	enum eDDIR{DDIR_LEFT, DDIR_DOWN, DDIR_RIGHT, DDIR_UP, DDIR_END};
	enum eDPRO{DPRO_UP, DPRO_DOWN, DPRO_END};
	enum eDFRAME{DFRAME_UP, DFRAME_DOWN, DFRAME_END};
private :
	DRAGONFRAME m_tDFrame;
	void Frame_Update(DRAGONFRAME& _tFrame);
	void Set_DragonFrame(int _Min, int _Mid, int _Max, int _Motion, DWORD _Speed)
	{
		m_tDFrame.iFrameMin = _Min;
		m_tDFrame.iFrameMid = _Mid;
		m_tDFrame.iFrameMax = _Max;
		m_tDFrame.dwFrameSpeed = _Speed;
		m_tDFrame.iFrameMotion = _Motion;
		m_tDFrame.dwFrameTime = GetTickCount();
	}
	eDDIR			m_eDdir;
	eDFRAME			m_eDFrame;
	static eDPRO    m_eDPro;
	void Process_Detect();
private :
	list<CObj*>* m_pListMosnter;
protected:
	CObj*	m_pPlayer;
	POINT	m_tPoint;
	float	m_fRadius;
	float	m_fDraonAngle;
	bool	m_bProcess;
	DWORD	m_dTick;
public:
	CFireDragon();
	CFireDragon(float fX, float fY, float _fAngle);
	virtual ~CFireDragon();

public:
	virtual void Initialize(void)override;
	virtual int Update(void)override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;
private :
	void Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour);
	void Collision_Sphere(CObj* _Dest, CObj* _Sour);
};


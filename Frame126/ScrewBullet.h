#pragma once
#include "Obj.h"
class CScrewBullet :
	public CObj
{
public:
	CScrewBullet();
	virtual ~CScrewBullet();

public:
	virtual void Initialize(void) override;
	virtual int Update(void) override;
	virtual void Late_Update(void) override;
	virtual void Render(HDC hDC) override;
	virtual void Release(void) override;

private:
	POINT		m_tCenter;

	float		m_fRotAngle;	// 스크류 미사일의 회전 각도
	float		m_fRotSpeed;	// 미사일의 이동 속도
	float		m_fRotDistance;	// 중점으로부터 떨어진 거리

	bool		m_bStart;		// 중심 축은 한 번만 적용후 진행 방향대로 이동시켜야해서 생성 시점에 한 번만이란 조건을 주기 위해 선언

};


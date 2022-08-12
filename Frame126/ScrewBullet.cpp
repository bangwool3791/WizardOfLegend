#include "stdafx.h"
#include "ScrewBullet.h"

CScrewBullet::CScrewBullet()
{
}

CScrewBullet::~CScrewBullet()
{
	Release();
}

void CScrewBullet::Initialize(void)
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;

	m_fSpeed = 5.f;

	m_fRotAngle = 0.f;
	m_fRotSpeed = 30.f;
	m_fRotDistance = 20.f;

	m_bStart = true;
}

int CScrewBullet::Update(void)
{
	if (m_bDead)
		OBJ_DEAD;

	if (m_bStart)
	{
		m_tCenter.x = (long)m_tInfo.fX;
		m_tCenter.y = (long)m_tInfo.fY;
		m_bStart = false;
	}

	m_tCenter.x += long(m_fSpeed * cosf(m_fAngle * (PI / 180.f)));
	m_tCenter.y -= long(m_fSpeed * sinf(m_fAngle * (PI / 180.f)));

	m_fRotAngle += m_fRotSpeed;

	m_tInfo.fX = m_tCenter.x + m_fRotDistance * cosf(m_fRotAngle * (PI / 180.f));
	m_tInfo.fY = m_tCenter.y - m_fRotDistance * sinf(m_fRotAngle * (PI / 180.f));


	Update_Rect();

	return OBJ_NOEVENT;
}

void CScrewBullet::Late_Update(void)
{
	
}

void CScrewBullet::Render(HDC hDC)
{
	Ellipse(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CScrewBullet::Release(void)
{
	
}

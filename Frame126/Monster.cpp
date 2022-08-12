#include "stdafx.h"
#include "Monster.h"
#include "ObjMgr.h"
#include "UserHeader.h"

CMonster::CMonster()
{
}


CMonster::~CMonster()
{
	Release();
}

void CMonster::Initialize(void)
{
	m_tInfo.fX = 200.f;		// ���� X
	m_tInfo.fY = 550.f;		// ���� Y

	m_tInfo.fCX = 50.f;	// �÷��̾� ���� ������
	m_tInfo.fCY = 50.f;	// �÷��̾� ���� ������

	m_fSpeed = 3.f;
}

int CMonster::Update(void)
{
	if (true == m_bDead)
		return OBJ_DEAD;

	/*m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	
	if (nullptr == m_pTarget)
		return OBJ_NOEVENT;


	float	fWidth = 0.f, fHeight = 0.f, fDiagonal = 0.f;

	fWidth = m_pTarget->Get_Info().fX - m_tInfo.fX;
	fHeight = m_pTarget->Get_Info().fY - m_tInfo.fY;

	fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);

	// ����
	m_fAngle = acosf(fWidth / fDiagonal);

	// ���� -> ��׸��� ��ȯ
	m_fAngle = m_fAngle * (180.f / PI);

	if (m_pTarget->Get_Info().fY >= m_tInfo.fY)
	{
		//m_fAngle = 360.f - m_fAngle;		
		m_fAngle *= -1.f;
	}
		
	m_tInfo.fX += m_fSpeed * cosf(m_fAngle * (PI / 180.f));
	m_tInfo.fY -= m_fSpeed * sinf(m_fAngle * (PI / 180.f));*/

	Update_Rect();

	return OBJ_NOEVENT;
}

void CMonster::Late_Update(void)
{
	
}

void CMonster::Render(HDC hDC)
{
	Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}

void CMonster::Release(void)
{
	
}

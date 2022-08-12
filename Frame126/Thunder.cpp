#include "stdafx.h"
#include "Thunder.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
CThunder::CThunder()
{
}

CThunder::CThunder(float _x, float _y, float _fAngle)
{
	m_tInfo.fX = (float)_x;
	m_tInfo.fY = (float)_y;
	m_fAngle = _fAngle;
}

CThunder::~CThunder()
{
}

void CThunder::Initialize(void)
{
	m_pFrameKey = L"Skill_THUNDER";
	m_tInfo.fCX = 74.f;
	m_tInfo.fCY = 403.f;

	m_tFrame.dwFrameTime = GetTickCount();
	m_tFrame.iFrameStart = 4;
	m_tFrame.iFrameEnd = 15;
	m_tFrame.iFrameMotionEnd = 1;
	m_tFrame.dwFrameSpeed = 25;

	m_fRadius = 10;

	Update_Rect();
}
int CThunder::Update(void)
{
	m_fRadius+= (float)0.01;

	m_tInfo.fX = m_tInfo.fX + m_fRadius * cosf(m_fAngle);
	m_tInfo.fY = m_tInfo.fY - m_fRadius * sinf(m_fAngle);

	Update_Rect();
	if (InCrease_X_FrameWithY(m_tFrame))
	{
		return OBJ_DEAD;
	}
	return OBJ_NOEVENT;
}
void CThunder::Late_Update(void)
{

}
void CThunder::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	//BitBlt(hDC, m_tRect.left + iScrollX, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hMemDC, 0, 0, SRCCOPY);
	// 1���� : ���� ���� ���������� �׸��� �׸� DC
	// 2, 3���� : ���� ���� ��ġ�� ��ǥ�� ����(L, T ��ǥ)
	// 4, 5���� : ���� ���� �ؽ�ó�� X, Y ������
	// 6 ���� : ��Ʈ���� ������ �ִ� DC
	// 7, 8���� : ��Ʈ���� ����� ���� ��ǥ(L, T��ǥ)
	// 9���� : ��� ȿ���� ���� SRCCOPY : �״�� ���
	GdiTransparentBlt(hDC,							// 1���� : ���� ���� ���������� �׸��� �׸� DC
		(int)m_tRect.left,	// 2,3���� : 2, 3���� : ���� ���� ��ġ�� ��ǥ�� ����(L, T ��ǥ)
		(int)m_tRect.top - 190,
		(int)m_tInfo.fCX,				// 4,5 ���� : ���� ���� �ؽ�ó�� X, Y ������
		(int)m_tInfo.fCY,
		hMemDC,							// 6���� : ��Ʈ���� ������ �ִ� DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,			// 7, 8���� : ��Ʈ���� ����� ���� ��ǥ
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,		// 9, 10���� : ������ ��Ʈ���� X,Y ������	
		(int)m_tInfo.fCY,
		RGB(0, 0, 0));			// 11���� : �����ϰ��� �ϴ� ����
}
void CThunder::Release(void)
{

}

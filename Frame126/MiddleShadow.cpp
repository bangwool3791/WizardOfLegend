#include "stdafx.h"
#include "MiddleShadow.h"
#include "UserHeader.h"

CMiddleShadow::CMiddleShadow()
{
}


CMiddleShadow::~CMiddleShadow()
{
	Release();
}

CMiddleShadow::CMiddleShadow(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

void CMiddleShadow::Initialize(void)
{
	m_tInfo.fCX = FCX_UI_SHADOW;
	m_tInfo.fCY = FCY_UI_SHADOW;

	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(L"NORMAL_SHADOW");
	m_pTarget = m_pObjMgr->Get_Player();
}
int CMiddleShadow::Update(void)
{
	m_tInfo.fX = m_pTarget->Get_Info().fX;
	m_tInfo.fY = m_pTarget->Get_Info().fY + m_pTarget->Get_Info().fCY * 0.5;
	Update_Rect();
	return OBJ_NOEVENT;
}
void CMiddleShadow::Late_Update(void)
{

}

void CMiddleShadow::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	//BitBlt(hDC, m_tRect.left + iScrollX, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, m_hMemDC, 0, 0, SRCCOPY);
	// 1���� : ���� ���� ���������� �׸��� �׸� DC
	// 2, 3���� : ���� ���� ��ġ�� ��ǥ�� ����(L, T ��ǥ)
	// 4, 5���� : ���� ���� �ؽ�ó�� X, Y ������
	// 6 ���� : ��Ʈ���� ������ �ִ� DC
	// 7, 8���� : ��Ʈ���� ����� ���� ��ǥ(L, T��ǥ)
	// 9���� : ��� ȿ���� ���� SRCCOPY : �״�� ���
	GdiTransparentBlt(hDC,							// 1���� : ���� ���� ���������� �׸��� �׸� DC
		(int)(m_tRect.left + iScrollX),	// 2,3���� : 2, 3���� : ���� ���� ��ġ�� ��ǥ�� ����(L, T ��ǥ)
		(int)(m_tRect.top + iScrollY),
		(int)m_tInfo.fCX,				// 4,5 ���� : ���� ���� �ؽ�ó�� X, Y ������
		(int)m_tInfo.fCY,
		m_hMemDC,							// 6���� : ��Ʈ���� ������ �ִ� DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,			// 7, 8���� : ��Ʈ���� ����� ���� ��ǥ
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,		// 9, 10���� : ������ ��Ʈ���� X,Y ������	
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));			// 11���� : �����ϰ��� �ϴ� ����
}

void CMiddleShadow::Release(void)
{

}

#include "stdafx.h"
#include "Deco.h"
#include "ScrollMgr.h"
#include "BmpMgr.h"
#include "UserHeader.h"

CDeco::CDeco()
{
}

CDeco::CDeco(float fX, float fY) :CObj()
{
	m_tInfo.fX = fX;
	m_tInfo.fY = fY;
}

CDeco::CDeco(const CDeco& Rhs)
{
	memcpy_s(&m_tInfo, sizeof(INFO), &Rhs.m_tInfo, sizeof(INFO));
	m_tInfo.iDrawXID = Rhs.Get_Info().iDrawXID;
	m_tInfo.iDrawYID = Rhs.Get_Info().iDrawYID;
	m_tInfo.iOption = Rhs.Get_Info().iOption;
}

CDeco::CDeco(const INFO& Rhs)
{
	memcpy_s(&m_tInfo, sizeof(INFO), &Rhs, sizeof(INFO));
}

CDeco::~CDeco()
{
	Release();
}

void CDeco::Initialize(void)
{
	m_pFrameKey = L"Tile";
	m_eGroup = GAMEOBJECT;
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	Update_Rect();
}

int CDeco::Update(void)
{
	Update_Rect();

	return OBJ_NOEVENT;
}

void CDeco::Late_Update(void)
{
	
}

void CDeco::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();
	//BitBlt(hDC, m_tRect.left + iScrollX, m_tRect.top, m_tInfo.fCX, m_tInfo.fCY, hMemDC, 0, 0, SRCCOPY);
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
		TILECX * m_tInfo.iDrawXID,			// 7, 8���� : ��Ʈ���� ����� ���� ��ǥ
		TILECY * m_tInfo.iDrawYID,
		(int)m_tInfo.fCX,		// 9, 10���� : ������ ��Ʈ���� X,Y ������	
		(int)m_tInfo.fCY,
		RGB(0, 0 , 0));			// 11���� : �����ϰ��� �ϴ� ����
}

void CDeco::Release(void)
{
	
}

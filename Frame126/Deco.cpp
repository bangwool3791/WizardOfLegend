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
	// 1인자 : 복사 받을 최종적으로 그림을 그릴 DC
	// 2, 3인자 : 복사 받을 위치의 좌표를 전달(L, T 좌표)
	// 4, 5인자 : 복사 받을 텍스처의 X, Y 사이즈
	// 6 인자 : 비트맵을 가지고 있는 DC
	// 7, 8인자 : 비트맵을 출력할 시작 좌표(L, T좌표)
	// 9인자 : 출력 효과를 설정 SRCCOPY : 그대로 출력
	GdiTransparentBlt(hDC,							// 1인자 : 복사 받을 최종적으로 그림을 그릴 DC
		(int)(m_tRect.left + iScrollX),	// 2,3인자 : 2, 3인자 : 복사 받을 위치의 좌표를 전달(L, T 좌표)
		(int)(m_tRect.top + iScrollY),
		(int)m_tInfo.fCX,				// 4,5 인자 : 복사 받을 텍스처의 X, Y 사이즈
		(int)m_tInfo.fCY,
		m_hMemDC,							// 6인자 : 비트맵을 가지고 있는 DC
		TILECX * m_tInfo.iDrawXID,			// 7, 8인자 : 비트맵을 출력할 시작 좌표
		TILECY * m_tInfo.iDrawYID,
		(int)m_tInfo.fCX,		// 9, 10인자 : 복사할 비트맵의 X,Y 사이즈	
		(int)m_tInfo.fCY,
		RGB(0, 0 , 0));			// 11인자 : 제거하고자 하는 색상
}

void CDeco::Release(void)
{
	
}

#include "stdafx.h"
#include "GameFont.h"
#include "UserHeader.h"

CGameFont::CGameFont()
{
}

CGameFont::CGameFont(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CGameFont::~CGameFont()
{

}

void CGameFont::Initialize(void)
{
	m_tInfo.fCX = FCX_UI_FONT;
	m_tInfo.fCY = FCY_UI_FONT;

	m_pFrameKey = L"FONT_0";
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	Update_Rect();
}

int	CGameFont::Update(void)
{
	return OBJ_NOEVENT;
 }
void CGameFont::Late_Update(void)
{

 }
void CGameFont::Render(HDC hDC)
{
	GdiTransparentBlt(hDC,							// 1인자 : 복사 받을 최종적으로 그림을 그릴 DC
		(int)m_tRect.left,	// 2,3인자 : 2, 3인자 : 복사 받을 위치의 좌표를 전달(L, T 좌표)
		(int)m_tRect.top,
		(int)m_tInfo.fCX,				// 4,5 인자 : 복사 받을 텍스처의 X, Y 사이즈
		(int)m_tInfo.fCY,
		m_hMemDC,							// 6인자 : 비트맵을 가지고 있는 DC
		0,								// 7, 8인자 : 비트맵을 출력할 시작 좌표
		0,
		(int)m_tInfo.fCX,								// 9, 10인자 : 복사할 비트맵의 X,Y 사이즈	
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));
 }
void CGameFont::Release(void)
{

 }

void CGameFont::Set_Font(TCHAR* _prim) {
	m_pFrameKey = _prim;
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
}

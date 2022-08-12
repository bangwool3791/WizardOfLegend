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
	GdiTransparentBlt(hDC,							// 1���� : ���� ���� ���������� �׸��� �׸� DC
		(int)m_tRect.left,	// 2,3���� : 2, 3���� : ���� ���� ��ġ�� ��ǥ�� ����(L, T ��ǥ)
		(int)m_tRect.top,
		(int)m_tInfo.fCX,				// 4,5 ���� : ���� ���� �ؽ�ó�� X, Y ������
		(int)m_tInfo.fCY,
		m_hMemDC,							// 6���� : ��Ʈ���� ������ �ִ� DC
		0,								// 7, 8���� : ��Ʈ���� ����� ���� ��ǥ
		0,
		(int)m_tInfo.fCX,								// 9, 10���� : ������ ��Ʈ���� X,Y ������	
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

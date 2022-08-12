#include "stdafx.h"
#include "Summon.h"
#include "UserHeader.h"

CSummon::CSummon()
{
}


CSummon::~CSummon()
{
}

CSummon::CSummon(float _x, float _y)
{
	m_tInfo.fX = _x;
	//730 소환 이미지 사이즈 절반, 182 플레이어 이미지 사이즈 절반
	//소환 이미지가 플레이어의 발 아래부터 시작되기 위한 Y값 처리
	m_tInfo.fY = (float)(_y - FCY_SUMMON * 0.5 + FCY_PLAYER * 0.5);
}
void CSummon::Initialize(void)
{
	Set_Frame(0, 50, 0, 7);
	m_pFrameKey = L"SUMMON";
	m_tInfo.fCX = FCX_SUMMON;
	m_tInfo.fCY = FCY_SUMMON;
	CSoundMgr::Get_Instance()->Set_Sound(L"teleport.mp3", SOUND_EFFECT, EFFECT_VOLUME);
	Update_Rect();
}
int	CSummon::Update(void)
{
	if (InCrease_X_Frame(m_tFrame))
		return OBJ_DEAD;

	return OBJ_NOEVENT;
}
void CSummon::Late_Update(void)
{

}
void CSummon::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

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
		hMemDC,							// 6인자 : 비트맵을 가지고 있는 DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,			// 7, 8인자 : 비트맵을 출력할 시작 좌표
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,		// 9, 10인자 : 복사할 비트맵의 X,Y 사이즈	
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));			// 11인자 : 제거하고자 하는 색상

}
void CSummon::Release(void)
{

}

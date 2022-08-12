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
	//730 ��ȯ �̹��� ������ ����, 182 �÷��̾� �̹��� ������ ����
	//��ȯ �̹����� �÷��̾��� �� �Ʒ����� ���۵Ǳ� ���� Y�� ó��
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
		hMemDC,							// 6���� : ��Ʈ���� ������ �ִ� DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,			// 7, 8���� : ��Ʈ���� ����� ���� ��ǥ
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,		// 9, 10���� : ������ ��Ʈ���� X,Y ������	
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));			// 11���� : �����ϰ��� �ϴ� ����

}
void CSummon::Release(void)
{

}

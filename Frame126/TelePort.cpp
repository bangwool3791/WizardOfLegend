#include "stdafx.h"
#include "TelePort.h"
#include "UserHeader.h"
#include "SceneMgr.h"
#include "Summon.h"

CTelePort::CTelePort()
{
}

CTelePort::CTelePort(float _fX, float _fY) : m_eTeleOpt(TELE_OPT_END)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

void CTelePort::Initialize(void)
{
	m_pFrameKey = L"TELEPORT";

	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();

	m_tInfo.fCX = FCX_UI_TELEPORT;
	m_tInfo.fCY = FCY_UI_TELEPORT;

	Update_Rect();
}
int CTelePort::Update(void)
{
	return OBJ_NOEVENT;
}
	
void CTelePort::Late_Update(void)
{
	Collision_RectEx(this, m_pPlayer);
}

void CTelePort::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

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
void CTelePort::Release(void)
{

}
CTelePort::~CTelePort()
{
}

void CTelePort::Collision_RectEx(CObj* _Dest, CObj* _Sour)
{
	RECT		rc{};

	float	fX = 0.f, fY = 0.f;

	if (CCollisionMgr::Check_Rect(_Dest, _Sour, &fX, &fY))
	{
		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_F1))
		{
			switch (Get_Opt())
			{
			case TELE_OPT_BOSS:
				m_pObjMgr->AddObject(EFFECT_KEY, SUMMON, CAbstactFactory<CSummon>::Create(m_tInfo.fX, m_tInfo.fY));
				CSceneMgr::Get_Instance()->Set_ScenID(SCENE_STAGE2);
				break;
			case TELE_OPT_ENDING:
				m_pObjMgr->AddObject(EFFECT_KEY, SUMMON, CAbstactFactory<CSummon>::Create(m_tInfo.fX, m_tInfo.fY));
				CSceneMgr::Get_Instance()->Set_ScenID(SCEN_ENDING);
				break;
			}
		}
	}
}


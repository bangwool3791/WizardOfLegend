#include "stdafx.h"
#include "Arrow.h"
#include "UserHeader.h"

CArrow::CArrow() :m_iYFrame(0)
{
}

CArrow::CArrow(float _fX, float _fY, float _fAngle) : m_iYFrame(0)
{
	ZeroMemory(&m_tInitInfo, sizeof(INFO));
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
	m_tInitInfo.fX = _fX;
	m_tInitInfo.fY = _fY;
	m_fAngle = _fAngle;

	m_hPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
}


CArrow::~CArrow()
{
	Release();
}

void CArrow::Initialize()
{
	m_tInfo.fCX = FCX_UI_ARROW;
	m_tInfo.fCY = FCY_UI_ARROW;

	m_fSpeed = 15.f;

	m_eState = STATE_CREATE;
	m_eGroup = RENDER_SKILL;
	m_pPlayer = m_pObjMgr->Get_Player();
	CSoundMgr::Get_Instance()->Set_Sound(L"ARCHER_AIM.mp3", SOUND_ACHERMAN_BACK, MONSTER_VOLUME_BACK);
	Update_Rect();
}

int CArrow::Update()
{
	if (InCrease_Init_X_Frame(m_tFrame))
	{
		switch (m_eState)
		{
		case STATE_CREATE:
			m_eState = STATE_ATTACK;
			CSoundMgr::Get_Instance()->Set_Sound(L"ARCHER_SHOOT.mp3", SOUND_ACHERMAN_BACK, MONSTER_VOLUME_BACK + 2);
			Set_Frame(4, 500, m_iYFrame, 4, 4);
			break;
		case STATE_ATTACK:
			break;
		}
	}

	if (m_bDead)
		return OBJ_DEAD;

	switch (m_eState)
	{
	case STATE_CREATE:
		m_tInfo.fX = m_pMonster->Get_Info().fX;
		m_tInfo.fY = m_pMonster->Get_Info().fY;
		break;
	case STATE_ATTACK:
		m_tInfo.fX += m_fSpeed*cosf(m_fAngle);
		m_tInfo.fY -= m_fSpeed*sinf(m_fAngle);
		break;
	}
	Update_Rect();

	return OBJ_NOEVENT;
}

void CArrow::Late_Update()
{
	//느려지는 문제 발생할 수도
	static DWORD dTick = GetTickCount();


	Collision_Sphere(this, m_pPlayer);
	Collision_Sphere(this, m_pObjMgr->Get_TargetList(SKILL_KEY, SKILL_GAIA_ARMOR));


	float fX = Displacement<float>(m_tInfo.fX, m_tInitInfo.fX);
	float fY = Displacement<float>(m_tInitInfo.fY, m_tInfo.fY);
	float fR = Distance<float>(fX, fY);

	if (ARROW_DISTNACE <= fR)
		Set_Dead();

}

void CArrow::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	m_hDC = hDC;

	switch (m_eState)
	{
	case STATE_CREATE:

		m_tInfo.fX += m_fSpeed*cosf(m_fAngle);
		m_tInfo.fY -= m_fSpeed*sinf(m_fAngle);
		m_hOldPen = (HPEN)::SelectObject(hDC, m_hPen);
		MoveToEx(hDC, (int)m_tInfo.fX + iScrollX, (int)m_tInfo.fY + iScrollY, nullptr);
		LineTo(hDC, (int)(m_tInfo.fX + ARROW_DISTNACE * cosf(m_fAngle) + iScrollX), (int)(m_tInfo.fY - ARROW_DISTNACE * sinf(m_fAngle) + iScrollY));
		break;
	}

	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);

	GdiTransparentBlt(hDC,
		(int)(m_tRect.left + iScrollX),
		(int)(m_tRect.top + iScrollY),
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		m_hMemDC,
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY,
		RGB(0, 255, 255));
}

void CArrow::Release()
{
	m_hPen = (HPEN)::SelectObject(m_hDC, m_hOldPen);
	DeleteObject(m_hPen);
	ReleaseDC(g_hWnd, m_hDC);

}

void CArrow::Collision_Sphere(CObj* _Dest, CObj* _Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;

	if (CCollisionMgr::Check_Sphere(_Sour, _Dest, &fR, &fAngle))
	{
		if (
			CObj::STATE_CREATE != _Sour->Get_State() &&
			CObj::STATE_STUN != _Sour->Get_State() &&
			CObj::STATE_DEAD != _Sour->Get_State() &&
			CObj::STATE_SKILL != _Sour->Get_State() &&
			CObj::STATE_CREATE != _Dest->Get_State())
		{
			_Sour->Set_State(CObj::STATE_STUN);
			_Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
			_Sour->Plus_Hp(-_Dest->Get_Attack());

			fAngle = fAngle * 180 / PI;
			fAngle = (float)((int)fAngle % 360);
			CSoundMgr::Get_Instance()->Set_Sound(L"PLAYER_HITED_1.mp3", SOUND_PLAYER_BACK, PLAYER_VOLUME_BACK);
		}
	}
}

void CArrow::Collision_Sphere(CObj* _Dest, list<CObj*>* Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;

	if (0 >= Sour->size())
		return;
	for (auto& _Sour : *Sour)
	{
		float fX = Displacement<float>(m_tInfo.fX, _Sour->Get_Info().fX);
		float fY = Displacement<float>(_Sour->Get_Info().fY, m_tInfo.fY);
		float fR = Distance<float>(fX, fY);

		if (100 <= fR)
			continue;

		if (CCollisionMgr::Check_Sphere(_Sour, _Dest, &fR, &fAngle))
		{
			this->Set_Dead();
			break;
		}
	}
}
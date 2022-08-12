#include "stdafx.h"
#include "Combo.h"
#include "UserHeader.h"
#include "Dash.h"
#include "HitEffect.h"

CCombo::CCombo()
{
}

CCombo::CCombo(float _fX, float _fY)
{
	m_tInfo.fX = _fX + COMBOX_COLLISION_RADIUS;
	m_tInfo.fY = _fY;
}
CCombo::~CCombo()
{
}

void CCombo::Initialize()
{
	m_tInfo.fCX = (float)FCX_PLAYER;
	m_tInfo.fCY = (float)FCY_PLAYER;

	m_pFrameKey = L"PLAYER_LEFT";
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	Set_Frame(0, 30, 3, 7);

	m_bf.AlphaFormat = 0;
	m_bf.BlendFlags = 0;
	m_bf.BlendOp = AC_SRC_OVER;
	m_bf.SourceConstantAlpha = 100;
}

int CCombo::Update()
{
	if (InCrease_X_Frame(m_tFrame))
	{
		switch (m_eComboState)
		{
		case COMBO_STATE_LEFT:
			if (0 == m_iComboCount)
			{
				Set_Frame(0, 30, 4, 7);
				m_iComboCount = 1;
			}
			else
			{
				m_pFrameKey = L"PLAYER_BACK";
				Set_Frame(0, 30, 3, 7);
				m_iComboCount = 0;
				m_eComboState = COMBO_STATE_UP;
				m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
				m_tInfo.fX = m_pTarget->Get_Info().fX;
				m_tInfo.fY = (float)(m_pTarget->Get_Info().fY - m_pTarget->Get_Info().fCY * 0.5 - m_tInfo.fCY * 0.5 + COMBOX_COLLISION_RADIUS);
			}
			break;
		case COMBO_STATE_UP:
			if (0 == m_iComboCount)
			{
				Set_Frame(0, 30, 4, 7);
				m_iComboCount = 1;
			}
			else
			{
				m_pFrameKey = L"PLAYER_RIGHT";
				Set_Frame(0, 30, 3, 7);
				m_iComboCount = 0;
				m_eComboState = COMBO_STATE_RIGHT;
				m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
				m_tInfo.fX = (float)(m_pTarget->Get_Info().fX + m_pTarget->Get_Info().fCY * 0.5 + m_tInfo.fCY * 0.5 - COMBOX_COLLISION_RADIUS);
				m_tInfo.fY = m_pTarget->Get_Info().fY;
			}
			break;
		case COMBO_STATE_RIGHT:
			if (0 == m_iComboCount)
			{
				Set_Frame(0, 30, 4, 7);
				m_iComboCount = 1;
			}
			else
			{
				m_pFrameKey = L"PLAYER_FRONT";
				Set_Frame(0, 30, 3, 7);
				m_iComboCount = 0;
				m_eComboState = COMBO_STATE_DOWN;
				m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
				m_tInfo.fX = m_pTarget->Get_Info().fX;
				m_tInfo.fY = (float)(m_pTarget->Get_Info().fY + m_pTarget->Get_Info().fCY * 0.5 + m_tInfo.fCY * 0.5 - COMBOX_COLLISION_RADIUS);
			}
			break;
		case COMBO_STATE_DOWN:
			if (0 == m_iComboCount)
			{
				Set_Frame(0, 30, 4, 7);
				m_iComboCount = 1;
			}
			else
			{
				m_iComboCount = 0;
				return OBJ_DEAD;
			}
			break;
		}
	}

	switch (m_eComboState)
	{
	case COMBO_STATE_LEFT:
		m_tInfo.fX = (float)(m_pTarget->Get_Info().fX - m_pTarget->Get_Info().fCX * 0.5 - m_tInfo.fCX * 0.5 + COMBOX_COLLISION_RADIUS);
		m_tInfo.fY = m_pTarget->Get_Info().fY;
		break;
	case COMBO_STATE_UP:
		m_tInfo.fX = m_pTarget->Get_Info().fX;
		m_tInfo.fY = (float)(m_pTarget->Get_Info().fY - m_pTarget->Get_Info().fCY * 0.5 - m_tInfo.fCY * 0.5 + COMBOX_COLLISION_RADIUS);
		break;
	case COMBO_STATE_RIGHT:
		m_tInfo.fX = (float)(m_pTarget->Get_Info().fX + m_pTarget->Get_Info().fCX * 0.5 + m_tInfo.fCX * 0.5 - COMBOX_COLLISION_RADIUS);
		m_tInfo.fY = m_pTarget->Get_Info().fY;
		break;
	case COMBO_STATE_DOWN:
		m_tInfo.fX = m_pTarget->Get_Info().fX;
		m_tInfo.fY = (float)(m_pTarget->Get_Info().fY + m_pTarget->Get_Info().fCY * 0.5 + m_tInfo.fCY * 0.5 - COMBOX_COLLISION_RADIUS);
	}
	Update_Rect();
	return OBJ_NOEVENT;
}

void CCombo::Late_Update()
{
	if(m_pTarget)
		Collision_Sphere(this, m_pTarget);
}
void CCombo::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	GdiTransparentBlt(hDC,							// 1인자 : 복사 받을 최종적으로 그림을 그릴 DC
		(int)(m_tRect.left + iScrollX),	// 2,3인자 : 2, 3인자 : 복사 받을 위치의 좌표를 전달(L, T 좌표)
		(int)(m_tRect.top + iScrollY),
		(int)m_tInfo.fCX,				// 4,5 인자 : 복사 받을 텍스처의 X, Y 사이즈
		(int)m_tInfo.fCY,
		m_hMemDC,							// 6인자 : 비트맵을 가지고 있는 DC
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,			// 7, 8인자 : 비트맵을 출력할 시작 좌표
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,		// 9, 10인자 : 복사할 비트맵의 X,Y 사이즈	
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));			// 11인자 : 제거하고자 하는 색상
}

void CCombo::Release()
{

}
void CCombo::Collision_Sphere(CObj* _Dest, CObj* _Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;

	if (CCollisionMgr::Check_Sphere(_Sour, _Dest, &fR, &fAngle))
	{
		if (
			CObj::STATE_CREATE != _Sour->Get_State() &&
			CObj::STATE_DEAD != _Sour->Get_State() &&
			CObj::STATE_CREATE != _Dest->Get_State())
		{
			_Sour->Set_State(CObj::STATE_STUN);
			_Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
			_Sour->Plus_Hp(-_Dest->Get_Attack());

			fAngle = fAngle * 180 / PI;
			fAngle = float((int)fAngle % 360);

			float fX = (float)(_Sour->Get_Info().fX + fR * 0.5 * cosf(fAngle * PI / 180));
			float fY = (float)(_Sour->Get_Info().fY - fR * 0.5 * sinf(fAngle * PI / 180));
			CObjMgr::Get_Instance()->AddObject(EFFECT_KEY, EFFECT_HIT, CAbstactFactory<CHitEffect>::Create(fX, fY));

		}
	}
}

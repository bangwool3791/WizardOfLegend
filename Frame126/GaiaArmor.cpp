#include "stdafx.h"
#include "GaiaArmor.h"
#include "Player.h"
#include "UserHeader.h"
#include "BottomHole.h"
#include "SoilEffect.h"

CGaiaArmor::CGaiaArmor() :m_eState(SHEILD_STATE_END)
{
}

CGaiaArmor::CGaiaArmor(float _x, float _y, float _fAngle) : m_eState(SHEILD_STATE_END)
{
	m_fAngle = _fAngle;
	m_tInfo.fX = _x + m_fDistance * cosf(m_fAngle * (PI / 180.f));
	m_tInfo.fY = _y - m_fDistance * sinf(m_fAngle * (PI / 180.f));
	Update_Rect();
}

CGaiaArmor::~CGaiaArmor()
{
	Release();
}

void CGaiaArmor::Initialize(void)
{
	m_pFrameKey = L"GAIA_ARMOR";

	m_eState = SHEILD_STATE_CREATE;

	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	m_dTick = GetTickCount();

	m_tInfo.fCX = (float)FCX_GAIA_ARMOR;
	m_tInfo.fCY = (float)FCY_GAIA_ARMOR;

	m_fSpeed = 4.f;

	
	m_eGroup = RENDER_SKILL;
	Set_Frame(0, 300, 0, 0);
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);


	m_pObjMgr->AddObject(UI_KEY, EFFECT_BOTTOM_HOLE,

		CAbstactFactory<CBottomHole>::Create(m_pTarget->Get_Info().fX + m_fDistance * cosf(m_fAngle),
			m_pTarget->Get_Info().fY - m_fDistance * sinf(m_fAngle) + 50));

	m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_pTarget->Get_Info().fX + m_fDistance * cosf(m_fAngle) - DISTANCE_SOIL_COLUM_EFFECT,
		m_pTarget->Get_Info().fY - m_fDistance * sinf(m_fAngle)));

	m_pObjMgr->AddObject(EFFECT_KEY, EFFECT_SOIL, CAbstactFactory<CSoilEffect>::Create(m_pTarget->Get_Info().fX + m_fDistance * cosf(m_fAngle) + DISTANCE_SOIL_COLUM_EFFECT,
		m_pTarget->Get_Info().fY - m_fDistance * sinf(m_fAngle)));

	m_dIntervalState = 2000;
}

int CGaiaArmor::Update(void)
{
	switch (m_eState)
	{
	case SHEILD_STATE_CREATE:
		break;
	case SHEILD_STATE_HOLD:
		m_fAngle += 0.05f;
		break;
	case SHEILD_STATE_DESTROY:
		return OBJ_DEAD;
	}

	if (m_dTick + m_dIntervalState < GetTickCount())
	{
		switch (m_eState)
		{
		case SHEILD_STATE_CREATE:
			m_dIntervalState = 14000;
			Set_Frame(0, 300, 0, 11);
			m_eState = SHEILD_STATE_HOLD;
			break;
		case SHEILD_STATE_HOLD:
			m_eState = SHEILD_STATE_DESTROY;
			break;
		case SHEILD_STATE_DESTROY:
			return OBJ_DEAD;
		}
		m_dTick = GetTickCount();
	}

	m_tInfo.fX = m_pTarget->Get_Info().fX + m_fDistance * cosf(m_fAngle);

	m_tInfo.fY = m_pTarget->Get_Info().fY - m_fDistance * sinf(m_fAngle);
	InCrease_X_Frame(m_tFrame);
	Update_Rect();

	return OBJ_NOEVENT;
}

void CGaiaArmor::Late_Update(void)
{

}

void CGaiaArmor::Render(HDC hDC)
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
		m_tFrame.iFrameStart * (int)m_tInfo.fCX,			// 7, 8인자 : 비트맵을 출력할 시작 좌표
		m_tFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,		// 9, 10인자 : 복사할 비트맵의 X,Y 사이즈	
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));			// 11인자 : 제거하고자 하는 색상
}

void CGaiaArmor::Release(void)
{

}


void CGaiaArmor::Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;
	float fX = 0.f, fY = 0.f;
	if (_Sour->empty())
		return;

	for (auto& Sour : *_Sour)
	{
		fX = Displacement<float>(Sour->Get_Info().fX, _Dest->Get_Info().fX);
		fY = Displacement<float>(_Dest->Get_Info().fY, Sour->Get_Info().fY);

		if (100 <= Distance<float>(fX, fY))
			continue;

		if (CCollisionMgr::Check_Sphere(_Dest, Sour, &fR, &fAngle))
		{
			if (
				CObj::STATE_CREATE != Sour->Get_State() &&
				CObj::STATE_STUN != Sour->Get_State() &&
				CObj::STATE_DEAD != Sour->Get_State() &&
				CObj::GOD_MODE_ACTIVE != Sour->Get_GodMode() &&
				CObj::STATE_CREATE != _Dest->Get_State())
			{
				Sour->Set_State(CObj::STATE_STUN);
				Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
				Sour->Plus_Hp(-_Dest->Get_Attack());

				fAngle = fAngle * 180 / PI;
				fAngle = float((int)fAngle % 360);

				float fX = (float)(Sour->Get_Info().fX + fR * 0.5 * cosf(fAngle * PI / 180));
				float fY = (float)(Sour->Get_Info().fY - fR * 0.5 * sinf(fAngle * PI / 180));
				CObjMgr::Get_Instance()->AddObject(EFFECT_KEY, EFFECT_HIT, CAbstactFactory<CHitEffect>::Create(fX, fY));

				if (0 <= Sour->Get_Hp())
					CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_DIED_2.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
				else
				{
					static int iSoundIndex = 0;
					if (0 == iSoundIndex)
					{
						CSoundMgr::Get_Instance()->Set_Sound(L"GAIA_ARMOR_HIT_1.mp3", SOUND_GAIA_ARMOR, EFFECT_VOLUME);
						iSoundIndex = 1;
					}
					else if (1 == iSoundIndex)
					{
						CSoundMgr::Get_Instance()->Set_Sound(L"GAIA_ARMOR_HIT_2.mp3", SOUND_GAIA_ARMOR, EFFECT_VOLUME);
						iSoundIndex = 2;
					}
					else if (2 == iSoundIndex)
					{
						CSoundMgr::Get_Instance()->Set_Sound(L"GAIA_ARMOR_HIT_3.mp3", SOUND_GAIA_ARMOR, EFFECT_VOLUME);
						iSoundIndex = 0;
					}

				}
			}
		}
	}
}

void CGaiaArmor::Collision_Sphere(CObj* _Dest, CObj* Sour)
{
	static int iSoundIndex = 0;
	float fR = 0.f;
	float fAngle = 0.f;

	if (40 <= abs(Sour->Get_Info().fX - _Dest->Get_Info().fX) &&
		40 <= abs(Sour->Get_Info().fY - _Dest->Get_Info().fY))
		return;

	if (CCollisionMgr::Check_Sphere(_Dest, Sour, &fR, &fAngle))
	{
		if (
			CObj::STATE_CREATE != Sour->Get_State() &&
			CObj::STATE_STUN != Sour->Get_State() &&
			CObj::STATE_DEAD != Sour->Get_State() &&
			CObj::GOD_MODE_ACTIVE != Sour->Get_GodMode() &&
			CObj::STATE_CREATE != _Dest->Get_State())
		{
			Sour->Set_State(CObj::STATE_STUN);
			Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
			Sour->Plus_Hp(-_Dest->Get_Attack());

			fAngle = fAngle * 180 / PI;
			fAngle = float((int)fAngle % 360);

			float fX = (float)(Sour->Get_Info().fX + fR * 0.5 * cosf(fAngle * PI / 180));
			float fY = (float)(Sour->Get_Info().fY - fR * 0.5 * sinf(fAngle * PI / 180));
			CObjMgr::Get_Instance()->AddObject(EFFECT_KEY, EFFECT_HIT, CAbstactFactory<CHitEffect>::Create(fX, fY));


			if (0 == iSoundIndex)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_ICE_HIT_1.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 1;
			}
			else if (1 == iSoundIndex)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_ICE_HIT_2.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 2;
			}
			else if (2 == iSoundIndex)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_ICE_HIT_3.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 0;
			}
		}
	}
}
#include "stdafx.h"
#include "IShield.h"
#include "Player.h"
#include "UserHeader.h"

CIShield::CIShield() :m_eState(SHEILD_STATE_END)
{
}

CIShield::CIShield(float _x, float _y, float _fAngle):m_eState(SHEILD_STATE_END)
{
	m_fAngle = _fAngle;
	m_tInfo.fX = _x + m_fDistance * cosf(m_fAngle * (PI / 180.f));
	m_tInfo.fY = _y - m_fDistance * sinf(m_fAngle * (PI / 180.f));
	Update_Rect();
}

CIShield::~CIShield()
{
	Release();
}

void CIShield::Initialize(void)
{
	m_pFrameKey = L"Skill_ICESHEILD";

	m_eState = SHEILD_STATE_CREATE;

	m_pTarget = CObjMgr::Get_Instance()->Get_Player();
	m_dTick = GetTickCount();

	m_tInfo.fCX = (float)FCX_ICESHIELD;
	m_tInfo.fCY = (float)FCY_ICESHIELD;

	m_fSpeed = 0;
	m_fDistance = 50.f;

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 17;
	m_tFrame.iFrameMotion = 0;
	m_tFrame.dwFrameSpeed = 50;
	m_tFrame.dwFrameTime = GetTickCount();
	m_eGroup = RENDER_SKILL;
	m_pListMonster = m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN);

	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
}

int CIShield::Update(void)
{
	switch (m_eState)
	{
	case SHEILD_STATE_CREATE:
	case SHEILD_STATE_HOLD:
		if (130 > m_fDistance)
			++m_fDistance;
		if (0.15 > m_fSpeed)
			m_fSpeed += (float)0.0005;
		m_fAngle += m_fSpeed;
		break;
	case SHEILD_STATE_DESTROY:
		if (0 < m_fSpeed)
			m_fSpeed -= (float)0.1;
		if (0 < m_fDistance)
			--m_fDistance;
		else
		{
			CPlayer* pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
			return OBJ_DEAD;
		}
		m_fAngle -= m_fSpeed;
		break;
	}
	
	if (m_dTick + 10000 < GetTickCount())
	{
		switch (m_eState)
		{
		case SHEILD_STATE_CREATE:
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

void CIShield::Late_Update(void)
{
	for (int i = SWORDMAN; i < BOSS; ++i)
	{
		Collision_Sphere(this, &m_pListMonster[i]);
	}
	if (!m_pListMonster[BOSS].empty())
		Collision_Sphere(this, m_pListMonster[BOSS].front());
}

void CIShield::Render(HDC hDC)
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
		RGB(0xC8, 0xE6, 0xFA));			// 11인자 : 제거하고자 하는 색상
}

void CIShield::Release(void)
{
	
}


void CIShield::Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour)
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

				if(0 <= Sour->Get_Hp())
					CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_DIED_2.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
				else
				{
					if (rand() % 4 == 0)
						CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_HITTED_ICE_1.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
					else if (rand() % 4 == 1)
						CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_HITTED_ICE_2.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
					else if (rand() % 4 == 2)
						CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_HITTED_ICE_3.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
					else if (rand() % 4 == 3)
						CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_HITTED_ICE_4.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);

				}
			}
		}
	}
}

void CIShield::Collision_Sphere(CObj* _Dest, CObj* Sour)
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
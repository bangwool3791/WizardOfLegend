#include "stdafx.h"
#include "Dash.h"
#include "UserHeader.h"

CDash::CDash()
{
}

CDash::CDash(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}


CDash::~CDash()
{
	Release();
}


void CDash::Initialize()
{
	m_tInfo.fCX = FCX_UI_SLASH;
	m_tInfo.fCY = FCY_UI_SLASH;

	m_pFrameKey = L"SKILL_SLASH";
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	m_pListMonster = m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN);
	m_eGroup = RENDER_SKILL;
	Update_Rect();
}

int CDash::Update()
{
	if (m_bDead)
		return OBJ_DEAD;

	if (InCrease_X_Frame(m_tFrame))
		return OBJ_DEAD;

	Update_Rect();
	return OBJ_NOEVENT;
}

void CDash::Late_Update()
{
	//몬스터와 충돌 처리 필요
	for(int i = SWORDMAN; i < BOSS; ++i)
		Collision_Sphere(this, &m_pListMonster[i]);

	if (!m_pListMonster[BOSS].empty())
		Collision_Sphere(this, m_pListMonster[BOSS].front());
}

void CDash::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

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
		RGB(255, 0, 255));
}

void CDash::Release()
{

}


void CDash::Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour)
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

				if (0 >= Sour->Get_Hp())
					CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_DIED_1.mp3", SOUND_SWORDMAN, MONSTER_VOLUME);
				else
				{
					if (rand() % 4 == 0)
						CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_HITTED_WIND_1.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
					else if (rand() % 4 == 1)
						CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_HITTED_WIND_2.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
					else if (rand() % 4 == 2)
						CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_HITTED_WIND_3.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
					else if (rand() % 4 == 3)
						CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_HITTED_WIND_4.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);

				}
			}
		}
	}
}

void CDash::Collision_Sphere(CObj* _Dest, CObj* Sour)
{
	static int iSoundIndex = 0;
	float fR = 0.f;
	float fAngle = 0.f;

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
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_WIND_HIT_1.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 1;
			}
			else if (1 == iSoundIndex)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_WIND_HIT_2.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 2;
			}
			else if (2 == iSoundIndex)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_WIND_HIT_3.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 3;
			}
			else if (3 == iSoundIndex)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_WIND_HIT_4.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 0;
			}
		}
	}
}
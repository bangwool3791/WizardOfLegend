#include "stdafx.h"
#include "IceBlast.h"
#include "UserHeader.h"

CIceBlast::CIceBlast()
{
}

CIceBlast::CIceBlast(float _fX, float _fY)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
}

CIceBlast::CIceBlast(float _fX, float _fY, float _fAngle)
{
	m_tInfo.fX = _fX;
	m_tInfo.fY = _fY;
	m_fAngle = _fAngle;
}

CIceBlast::~CIceBlast()
{
	Release();
}

void CIceBlast::Initialize(void)
{
	m_tInfo.fCX = (float)FCX_ICE_BLAST;
	m_tInfo.fCY = (float)FCY_ICE_BLAST;

	Set_Frame(0, 35, 0, 7);

	m_pFrameKey = L"ICE_BLAST";
	m_eGroup = RENDER_SKILL;
	m_pListMonster = m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN);

	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
}
int	CIceBlast::Update(void)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (InCrease_X_Frame(m_tFrame))
		return OBJ_DEAD;
	
	Update_Rect();
	return OBJ_NOEVENT;
}

void CIceBlast::Late_Update(void)
{
	for (int i = SWORDMAN; i < BOSS; ++i)
	{
		if (m_pListMonster[i].empty())
			continue;
		CCollisionMgr::Collision_Sphere(this, &m_pListMonster[i]);
	}
	if (!m_pListMonster[BOSS].empty())
		Collision_Sphere(this, m_pListMonster[BOSS].front());
}
void CIceBlast::Render(HDC hDC)
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
void CIceBlast::Release(void)
{

}

void CIceBlast::Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;
	if (_Sour->empty())
		return;
	for (auto& Sour : *_Sour)
	{
		if (40 <= abs(Sour->Get_Info().fX - _Dest->Get_Info().fX) &&
			40 <= abs(Sour->Get_Info().fY - _Dest->Get_Info().fY))
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
void CIceBlast::Collision_Sphere(CObj* _Dest, CObj* Sour)
{
	static int iSoundIndex = 0;
	float fR = 0.f;
	float fAngle = 0.f;
	float fX = 0.f, fY = 0.f;

	fX = Displacement<float>(Sour->Get_Info().fX, _Dest->Get_Info().fX);
	fY = Displacement<float>(_Dest->Get_Info().fY, Sour->Get_Info().fY);

	if (100 <= Distance<float>(fX, fY))
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
#include "stdafx.h"
#include "FireDragon.h"
#include "FireParticle.h"
#include "UserHeader.h"

CFireDragon::eDPRO CFireDragon::m_eDPro = CFireDragon::eDPRO::DPRO_UP;
CFireDragon::CFireDragon() :m_pPlayer(nullptr), m_fRadius(0)
{
	ZeroMemory(&m_tPoint, sizeof(POINT));
	ZeroMemory(&m_tDFrame, sizeof(DRAGONFRAME));
}

CFireDragon::CFireDragon(float fX, float fY, float _fAngle) :m_pPlayer(nullptr), m_fRadius(0), m_eDdir(DDIR_END)
{
	ZeroMemory(&m_tPoint, sizeof(POINT));
	ZeroMemory(&m_tDFrame, sizeof(DRAGONFRAME));
	m_tPoint.x = (LONG)fX;
	m_tPoint.y = (LONG)fY;
	m_tInfo.fX = fX;
	m_tInfo.fY = fY;
	m_fAngle = _fAngle;
}
void CFireDragon::Initialize()
{
	m_pFrameKey = L"FIREDRAGON";

	m_tInfo.fCX = (float)FCX_FIREDRAGON;
	m_tInfo.fCY = (float)FCY_FIREDRAGON;

	m_fSpeed = 12.f;
	m_fRadius = 40;
	m_fDraonAngle = 0.f;
	Update_Rect();
	m_eDFrame = DFRAME_DOWN;
	m_dTick = GetTickCount();
	m_eGroup = RENDER_SKILL;
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player();
	Process_Detect();
	m_hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pFrameKey);
	m_pListMosnter = m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN);
}

void CFireDragon::Frame_Update(DRAGONFRAME& _tFrame)
{
	if (_tFrame.dwFrameTime + _tFrame.dwFrameSpeed < GetTickCount())
	{
		switch (m_eDFrame)
		{
		case DFRAME_DOWN:
			if (_tFrame.iFrameMin < _tFrame.iFrameMid)
			{
				--_tFrame.iFrameMid;

				if (_tFrame.iFrameMin == _tFrame.iFrameMid)
					m_eDFrame = DFRAME_UP;
			}
			break;
		case DFRAME_UP:
			if (_tFrame.iFrameMax > _tFrame.iFrameMid)
			{
				++_tFrame.iFrameMid;

				if (_tFrame.iFrameMax == _tFrame.iFrameMid)
					m_eDFrame = DFRAME_DOWN;
			}
			break;
		}
		_tFrame.dwFrameTime = GetTickCount();
	}
}

void CFireDragon::Process_Detect()
{
	CObj* pMouse = CObjMgr::Get_Instance()->Get_Target(OBJ_KEY, OBJ_MOUSE, nullptr);

	float _fX = (float)m_tPoint.x;
	float _fY = (float)m_tPoint.y;
	if (m_pPlayer)
	{
		float fX = Displacement(m_pPlayer->Get_Info().fX, _fX);
		float fY = Displacement(_fY, m_pPlayer->Get_Info().fY);
		float fAngle = Angle(m_pPlayer->Get_Info().fX, _fX, _fY, m_pPlayer->Get_Info().fY);
		fAngle = fAngle / PI * 180;

		//360도 방향 추가
		if (abs(fY) < abs(fX))
		{
			if (0 > fX)
				m_eDdir = DDIR_LEFT;
			else
				m_eDdir = DDIR_RIGHT;
		}
		else
		{
			if (0 < fY)
				m_eDdir = DDIR_UP;
			else
				m_eDdir = DDIR_DOWN;
		}


		if (DPRO_UP == m_eDPro)
			m_eDPro = DPRO_DOWN;
		else
			m_eDPro = DPRO_UP;


		switch (m_eDdir)
		{
		case DDIR_LEFT:
			switch (m_eDPro)
			{
			case DPRO_DOWN:
				Set_DragonFrame(6, 8, 10, 1, 200);
				m_eDFrame = DFRAME_DOWN;
				break;
			case DPRO_UP:
				Set_DragonFrame(6, 8, 10, 1, 200);
				m_eDFrame = DFRAME_UP;
				break;
			}
			break;
		case DDIR_UP:
			switch (m_eDPro)
			{
			case DPRO_DOWN:
				Set_DragonFrame(14, 16, 18, 0, 200);
				m_eDFrame = DFRAME_UP;
				break;
			case DPRO_UP:
				Set_DragonFrame(0, 2, 5, 1, 200);
				m_eDFrame = DFRAME_DOWN;
				break;
			}
			break;
		case DDIR_RIGHT:
			switch (m_eDPro)
			{
			case DPRO_DOWN:
				Set_DragonFrame(6, 8, 10, 0, 200);
				m_eDFrame = DFRAME_DOWN;
			case DPRO_UP:
				Set_DragonFrame(6, 8, 10, 0, 200);
				m_eDFrame = DFRAME_UP;
				break;
			}
			break;
		case DDIR_DOWN:
			switch (m_eDPro)
			{
			case DPRO_DOWN:
				Set_DragonFrame(14, 16, 18, 1, 200);
				m_eDFrame = DFRAME_UP;
				break;
			case DPRO_UP:
				Set_DragonFrame(0, 2, 5, 0, 200);
				m_eDFrame = DFRAME_DOWN;
				break;
			}
			break;
		}
	}
}

int CFireDragon::Update()
{
	static int iSoundIndex = 0;
	float fX = Displacement(m_tInfo.fX, m_pPlayer->Get_Info().fX);
	float fY = Displacement(m_pPlayer->Get_Info().fY, m_tInfo.fY);
	float fR = Distance(fX, fY);

	if (m_bDead)
	{
		if (0 == iSoundIndex)
		{
			CSoundMgr::Get_Instance()->Set_Sound(L"WALL_HITTED_FIREDRAGON_1.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
			iSoundIndex = 1;
		}
		else
		{
			CSoundMgr::Get_Instance()->Set_Sound(L"WALL_HITTED_FIREDRAGON_2.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
			iSoundIndex = 0;
		}

		return OBJ_DEAD;
	}

	if (RANGE_FIRE_DRAGON < fR)
	{
		if (rand() % 3 == 0)
			CSoundMgr::Get_Instance()->Set_Sound(L"FIRE_DRAGON_DIE_1.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
		else if (rand() % 3 == 1)
			CSoundMgr::Get_Instance()->Set_Sound(L"FIRE_DRAGON_DIE_2.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
		else
			CSoundMgr::Get_Instance()->Set_Sound(L"FIRE_DRAGON_DIE_3.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);

		return OBJ_DEAD;
	}

	switch (m_eDPro)
	{
	case DPRO_UP:
		m_fDraonAngle += 20;
		break;
	case DPRO_DOWN:
		m_fDraonAngle -= 20;
		break;
	}
	Frame_Update(m_tDFrame);


	m_tPoint.x += (LONG)(m_fSpeed * cosf(m_fAngle));
	m_tPoint.y -= (LONG)(m_fSpeed * sinf(m_fAngle));

	m_tInfo.fX = m_tPoint.x + m_fRadius * cosf(m_fDraonAngle * PI / 180);
	m_tInfo.fY = m_tPoint.y - m_fRadius * sinf(m_fDraonAngle * PI / 180);
	Update_Rect();

	if (m_dTick + 40 < GetTickCount())
	{
		CObjMgr::Get_Instance()->AddObject(UI_KEY, FIRE_PARTICLE, CAbstactFactory<CFireParticle>::Create(m_tInfo.fX, m_tInfo.fY));
		m_dTick = GetTickCount();
	}
	return OBJ_NOEVENT;
}

void CFireDragon::Late_Update()
{
	for (int i = SWORDMAN; i < BOSS; ++i)
	{
		Collision_Sphere(this, &m_pListMosnter[i]);
	}
	if(!m_pListMosnter[BOSS].empty())
		Collision_Sphere(this, m_pListMosnter[BOSS].front());
}

void CFireDragon::Render(HDC hDC)
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
		m_tDFrame.iFrameMid * (int)m_tInfo.fCX,			// 7, 8인자 : 비트맵을 출력할 시작 좌표
		m_tDFrame.iFrameMotion * (int)m_tInfo.fCY,
		(int)m_tInfo.fCX,		// 9, 10인자 : 복사할 비트맵의 X,Y 사이즈	
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));			// 11인자 : 제거하고자 하는 색상
}
void CFireDragon::Release()
{

}

CFireDragon::~CFireDragon()
{

}

void CFireDragon::Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour)
{
	static int iSoundIndex = 0;
	float fR = 0.f;
	float fAngle = 0.f;
	float	fX = 0.f, fY = 0.f;

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
					CSoundMgr::Get_Instance()->Set_Sound(L"ENEMY_DIED_2.mp3", SOUND_SWORDMAN_BACK, MONSTER_VOLUME);
				else
				{
					if (0 == iSoundIndex)
					{
						CSoundMgr::Get_Instance()->Set_Sound(L"FIRE_DRAGON_HITTED_ENEMY_1.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
						iSoundIndex = 1;
					}
					else if (1 == iSoundIndex)
					{
						CSoundMgr::Get_Instance()->Set_Sound(L"FIRE_DRAGON_HITTED_ENEMY_2.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
						iSoundIndex = 2;
					}
					else if (2 == iSoundIndex)
					{
						CSoundMgr::Get_Instance()->Set_Sound(L"FIRE_DRAGON_HITTED_ENEMY_3.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
						iSoundIndex = 3;
					}
					else if (3 == iSoundIndex)
					{
						CSoundMgr::Get_Instance()->Set_Sound(L"FIRE_DRAGON_HITTED_ENEMY_4.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
						iSoundIndex = 4;
					}
					else if (4 == iSoundIndex)
					{
						CSoundMgr::Get_Instance()->Set_Sound(L"FIRE_DRAGON_HITTED_ENEMY_5.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
						iSoundIndex = 5;
					}
					else if (5 == iSoundIndex)
					{
						CSoundMgr::Get_Instance()->Set_Sound(L"FIRE_DRAGON_HITTED_ENEMY_6.mp3", SOUND_EFFECT_BACK, EFFECT_VOLUME);
						iSoundIndex = 0;
					}
				}
			}
		}
	}
}


void CFireDragon::Collision_Sphere(CObj* _Dest, CObj* Sour)
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
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_FIRE_HIT_1.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 1;
			}
			else if (1 == iSoundIndex)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_FIRE_HIT_2.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 2;
			}
			else if (2 == iSoundIndex)
			{
				CSoundMgr::Get_Instance()->Set_Sound(L"BOSS_FIRE_HIT_3.mp3", SOUND_BOSS, BOSS_VOLUME);
				iSoundIndex = 0;
			}
		}
	}
}
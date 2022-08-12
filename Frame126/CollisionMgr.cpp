#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "UserHeader.h"
#include "HitEffect.h"
#include "AbstactFactory.h"

CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}

void CCollisionMgr::Set_SourPoint(float fX, float fY, CObj* _Dest, CObj* _Sour)
{
	if (fX > fY)	// 상하 충돌의 경우
	{
		// 상 충돌
		if (_Sour->Get_Info().fY < _Dest->Get_Info().fY)
		{
			_Sour->Set_PosY(-fY);
		}
		else // 하 충돌
		{
			_Sour->Set_PosY(fY);
		}
	}
	else			// 좌우 충돌의 경우
	{
		// 상 충돌
		if (_Sour->Get_Info().fX < _Dest->Get_Info().fX)
		{
			_Sour->Set_PosX(-fX);
		}
		else // 하 충돌
		{
			_Sour->Set_PosX(fX);
		}
	}
}


void CCollisionMgr::Collision_Rect(list<CObj*> _Dest, list<CObj*> _Sour)
{
	RECT		rc{};

	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			if (IntersectRect(&rc, &(Dest->Get_Rect()), &(Sour->Get_Rect())))
			{
				Dest->Set_Dead();
				Sour->Set_Dead();
			}
		}
	}
}


void CCollisionMgr::Collision_RectEx(CObj* _Dest, CObj* _Sour)
{
	RECT		rc{};

	float	fX = 0.f, fY = 0.f;

	if (Check_Rect(_Dest, _Sour, &fX, &fY))
	{
		if (fX > fY)	// 상하 충돌의 경우
		{
			// 상 충돌
			if (_Dest->Get_Info().fY < _Sour->Get_Info().fY)
			{
				_Dest->Set_PosY(-fY);
			}
			else // 하 충돌
			{
				_Dest->Set_PosY(fY);
			}
		}
		else			// 좌우 충돌의 경우
		{
			// 상 충돌
			if (_Dest->Get_Info().fX < _Sour->Get_Info().fX)
			{
				_Dest->Set_PosX(-fX);
			}
			else // 하 충돌
			{
				_Dest->Set_PosX(fX);
			}
		}
	}
}


void CCollisionMgr::Collision_RectEx(CObj* _Dest, list<CObj*>* _Sour)
{
	RECT		rc{};

	for (auto& Sour : *_Sour)
	{
		float	fX = 0.f, fY = 0.f;

		if (Check_Rect(_Dest, Sour, &fX, &fY))
		{
			if (fX > fY)	// 상하 충돌의 경우
			{
				// 상 충돌
				if (Sour->Get_Info().fY < _Dest->Get_Info().fY)
				{
					Sour->Set_PosY(-fY);
				}
				else // 하 충돌
				{
					Sour->Set_PosY(fY);
				}
			}
			else			// 좌우 충돌의 경우
			{
				// 상 충돌
				if (Sour->Get_Info().fX < _Dest->Get_Info().fX)
				{
					Sour->Set_PosX(-fX);
				}
				else // 하 충돌
				{
					Sour->Set_PosX(fX);
				}
			}
		}
	}
}


void CCollisionMgr::Collision_RectEx(list<CObj*> _Dest, list<CObj*> _Sour)
{
	RECT		rc{};

	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			float	fX = 0.f, fY = 0.f;

			if (Check_Rect(Dest, Sour, &fX, &fY))
			{
				if (fX > fY)	// 상하 충돌의 경우
				{
					// 상 충돌
					if (Dest->Get_Info().fY < Sour->Get_Info().fY)
					{
						Dest->Set_PosY(-fY);
					}
					else // 하 충돌
					{
						Dest->Set_PosY(fY);
					}
				}
				else			// 좌우 충돌의 경우
				{
					// 상 충돌
					if (Dest->Get_Info().fX < Sour->Get_Info().fX)
					{
						Dest->Set_PosX(-fX);
					}
					else // 하 충돌
					{
						Dest->Set_PosX(fX);
					}
				}
			}
		}
	}
}


void CCollisionMgr::Collision_Sphere(CObj* _Dest, CObj* _Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;

	if (Check_Sphere(_Sour, _Dest, &fR, &fAngle))
	{
		if (
			CObj::STATE_CREATE != _Sour->Get_State() &&
			CObj::STATE_DEAD != _Sour->Get_State() &&
			CObj::STATE_CREATE != _Dest->Get_State())
		{
			_Sour->Set_PosX((float)(fR* 0.5 * cosf(fAngle)));
			_Sour->Set_PosY(float(fR* 0.5 * sinf(fAngle)));
			_Sour->Set_State(CObj::STATE_STUN);
			_Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
			_Sour->Plus_Hp(-_Dest->Get_Attack());

			fAngle = fAngle * 180 / PI;
			fAngle = (float)((int)fAngle % 360);
		}
	}
}


void CCollisionMgr::Collision_Sphere(CObj* _Dest, CObj* _Sour, COL_OPTION )
{
	float fR = 0.f;
	float fAngle = 0.f;

	if (Check_Sphere(_Sour, _Dest, &fR, &fAngle))
	{
		if (
			CObj::STATE_CREATE != _Sour->Get_State() &&
			CObj::STATE_DEAD != _Sour->Get_State() &&
			CObj::STATE_CREATE != _Dest->Get_State() &&
			CObj::STATE_SKILL != _Sour->Get_State())
		{
			_Sour->Set_PosX((float)(fR* 0.5 * cosf(fAngle)));
			_Sour->Set_PosY(float(fR* 0.5 * sinf(fAngle)));
			_Sour->Set_State(CObj::STATE_STUN);
			_Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
			_Sour->Plus_Hp(-_Dest->Get_Attack());
			
			_Dest->Set_Dead();
		}
	}
}

void CCollisionMgr::Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;
	for (auto& Sour : *_Sour)
	{
		if (100 <= abs(Sour->Get_Info().fX - _Dest->Get_Info().fX) &&
			100 <= abs(Sour->Get_Info().fY - _Dest->Get_Info().fY))
			continue;

		if (Check_Sphere(_Dest, Sour, &fR, &fAngle))
		{
			if (
				CObj::STATE_CREATE != Sour->Get_State() &&
				CObj::STATE_STUN != Sour->Get_State() &&
				CObj::STATE_DEAD != Sour->Get_State() &&
				CObj::GOD_MODE_ACTIVE != Sour->Get_GodMode() &&
				CObj::STATE_CREATE != _Dest->Get_State())
			{
				Sour->Set_PosX(float(fR* 0.5 * cosf(fAngle)));
				Sour->Set_PosY(float(fR* 0.5 * sinf(fAngle)));
				Sour->Set_State(CObj::STATE_STUN);
				Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
				Sour->Plus_Hp(-_Dest->Get_Attack());
				
				fAngle = fAngle * 180 / PI;
				fAngle = float((int)fAngle % 360);

				float fX = (float)(Sour->Get_Info().fX + fR * 0.5 * cosf(fAngle * PI / 180));
				float fY = (float)(Sour->Get_Info().fY - fR * 0.5 * sinf(fAngle * PI / 180));
				CObjMgr::Get_Instance()->AddObject(EFFECT_KEY, EFFECT_HIT, CAbstactFactory<CHitEffect>::Create(fX, fY));
			}
		}
	}
}


void CCollisionMgr::Collision_Sphere(CObj* _Dest, list<CObj*>* _Sour, COL_OPTION _eOption)
{
	float fR = 0.f;
	float fAngle = 0.f;
	for (auto& Sour : *_Sour)
	{
		if (Check_Sphere(_Dest, Sour, &fR, &fAngle))
		{
			if (
				CObj::STATE_CREATE != Sour->Get_State() &&
				CObj::STATE_DEAD != Sour->Get_State() &&
				CObj::GOD_MODE_ACTIVE != Sour->Get_GodMode() &&
				CObj::STATE_CREATE != _Dest->Get_State())
			{
				Sour->Set_PosX(float(fR* 0.5 * cosf(fAngle)));
				Sour->Set_PosY(float(fR* 0.5 * sinf(fAngle)));
				Sour->Set_State(CObj::STATE_STUN);
				Sour->Set_DamageState(CObj::DAMAGE_STATE_STUNED);
				Sour->Plus_Hp(-_Dest->Get_Attack());

				fAngle = fAngle * 180 / PI;
				fAngle = float((int)fAngle % 360);

				float fX = (float)(Sour->Get_Info().fX + fR * 0.5 * cosf(fAngle * PI / 180));
				float fY = (float)(Sour->Get_Info().fY - fR * 0.5 * sinf(fAngle * PI / 180));
				CObjMgr::Get_Instance()->AddObject(EFFECT_KEY, EFFECT_HIT, CAbstactFactory<CHitEffect>::Create(fX, fY));
			}
		}
	}
}

void CCollisionMgr::Collision_Sphere(list<CObj*> _Dest, list<CObj*> _Sour)
{
	float fR = 0.f;
	float fAngle = 0.f;
	for (auto& Dest : _Dest)
	{
		for (auto& Sour : _Sour)
		{
			if (Check_Sphere(Dest, Sour, &fR, &fAngle))
			{
				Dest->Set_PosX(fR * cosf(fAngle));
				Dest->Set_PosY(fR * sinf(fAngle));
			}
		}
	}
}

bool CCollisionMgr::Check_Sphere(CObj* pDest, CObj* pSour, float* _fR, float* _fAngle)
{
	if (!pSour)
		return false;

	float		fWidth = fabs(pDest->Get_Info().fX - pSour->Get_Info().fX);		// abs 는 절대 값을 구해주는 함수
	float		fHeight = fabs(pDest->Get_Info().fY - pSour->Get_Info().fY);
	
	float		fDiagonal = sqrtf(fWidth * fWidth + fHeight * fHeight);			// 루트를 씌워 값을 구해주는 함수

	//float		fRadius = pDest->Get_Info().fCX / 2.f + pSour->Get_Info().fCX / 2.f;

	float		fRadius = (float)((pDest->Get_Info().fCX + pSour->Get_Info().fCX * 0.5) * 0.5f);

	*_fR = fRadius - fDiagonal;
	*_fAngle = Angle<float>(pDest->Get_Info().fX, pSour->Get_Info().fX, pDest->Get_Info().fY, pSour->Get_Info().fY);
	return fRadius >= fDiagonal;
}

bool CCollisionMgr::Check_Rect(CObj* pDest, CObj* pSour, float* _fX, float* _fY)
{
	float		fWidth = fabs(pDest->Get_Info().fX - pSour->Get_Info().fX);
	float		fHeight = fabs(pDest->Get_Info().fY - pSour->Get_Info().fY);

	float		fCX = (pDest->Get_Info().fCX + pSour->Get_Info().fCX) * 0.5f;
	float		fCY = (pDest->Get_Info().fCY + pSour->Get_Info().fCY) * 0.5f;

	if ((fCX > fWidth) && (fCY > fHeight))
	{
		*_fX = fCX - fWidth;
		*_fY = fCY - fHeight;

		return true;
	}

	return false;
}

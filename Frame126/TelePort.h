#pragma once
#include "Obj.h"
class CTelePort :
	public CObj
{
public :
	enum TELE_OPT{TELE_OPT_BOSS, TELE_OPT_ENDING, TELE_OPT_END};
public :
	TELE_OPT Get_Opt() { return m_eTeleOpt; }
	void	Set_Opt(TELE_OPT _opt) { m_eTeleOpt = _opt; }
private:
	TELE_OPT m_eTeleOpt;
private :
	HDC m_hMemDC;
public:
	CTelePort();
	CTelePort(float _fX, float _fY);
	virtual ~CTelePort();
public :
	void		Initialize(void);
	int			Update(void);
	void		Late_Update(void);
	void		Render(HDC hDC);
	void		Release(void);
private :
	void Collision_RectEx(CObj* _Dest, CObj* _Sour);
	
};


#pragma once

#include "Tile.h"
#include "UserHeader.h"

class CPlayer;
class CScrollMgr;
class CObjMgr;
class CObj;
class CTileMgr
{
private :
	void ProcessEraseFireBall();
private :
	enum TILE_PRO{ PRO_MONSTER_1, PRO_MONSTER_2, PRO_MONSTER_3, PRO_MONSTER_4, PRO_MONSTER_5, PRO_MONSTER_6, PRO_MONSTER_7, PRO_MONSTER_8,
		PRO_MONSTER_9, PRO_MONSTER_10, PRO_END };
	TILE_PRO m_ePro = PRO_END;
	void Update_Process();
public :
	void Set_TilePro(TILE_PRO _ePro) { m_ePro = _ePro; }
	TILE_PRO Get_TilePro() { return m_ePro; }
private :
	bool		m_bProIronProcess = true;
	bool		m_bIronProcess = false;
	void Set_IronProcess(bool _bPro) { m_bIronProcess = _bPro; }
private :
	HDC			m_hDCMiniPlayer;
	HDC			m_hDCMiniMap;
	BLENDFUNCTION m_bf;
	CScrollMgr* m_pScrollMgr;
	CObjMgr* m_pObjMgr;
private :
	void  Collision_Tile(CObj* _Sour, list<CObj*>* _Dest, int i);
	void  Collision_Tile(CObj* _Sour, list<CObj*>* _Dest);
	void  Collision_Lava(vector<CObj*>* _Dest);
public : 
	vector<CObj*>* Get_DecoVec() { return &m_vecDeco; }
	vector<CObj*>* Get_ColVec() { return &m_vecCol; }
	vector<CObj*>* Get_LavaVec() { return &m_vecLava; }
public :
	void	Set_LavalSize(int _Size)
	{

		m_iLavaSize += _Size;
	}
	int		Get_LavaSize() const { return m_iLavaSize; }
	void	Set_ColSize(int _Size) 
	{

			m_iColSize += _Size; 
	}
	int		Get_ColSize() const { return m_iColSize; }

	void	Set_DecoSize(int _Size)
	{

		m_iDecoSize += _Size;
	}
	int		Get_DecoSize() const { return m_iDecoSize; }
	void	Delete_Deco() 
	{ 
		m_vecDeco.pop_back(); 
		Set_DecoSize(-1);

	}
	void	Delete_Col()
	{
		m_vecCol.pop_back();
		Set_ColSize(-1);

	}
	void	Delete_Lava()
	{
		m_vecLava.pop_back();
		Set_LavalSize(-1);

	}
private :
	int		m_iDecoSize;
	int		m_iColSize;
	int		m_iLavaSize;
private :
	template<typename T>
	bool		CompareTileID(const T& Dest, const T& Sour)
	{
		return (int)Dest->Get_Info().iOption > (int)Sour->Get_Info().iOption;
	}
	CObj* Get_MinDistanceTile();
public :
	bool Load_Process(void);
	void Save_Process(void);
	bool Load_Process2(void);
	void Save_Process2(void);
public:
	CTileMgr();
	~CTileMgr();
	//충돌 처리
private :
	void Collision_Tile(CObj* Dest, CObj* Sour);
private :
	CPlayer* m_pPlayer;
public:
	void		Initialize(void);
	void		Update(void);
	void		Late_Update(void);
	void		Render(HDC _DC);
	void		Release(void);

public:
	void		Tile_Picking(POINT& pt, int _iDrawXID, int _iDrawYID, TILE_OPT _iOption);
	void		Tile_Picking(POINT& pt, int _iDrawXID, int _iDrawYID);
	void		Tile_Picking(POINT& pt, TILE_OPT _iOption);
	void		Deco_Picking(POINT& pt, int _iDrawXID, int _iDrawYID, TILE_OPT _iOption);
	void		Deco_Picking(POINT& pt, int _iDrawXID, int _iDrawYID);
	void		Col_Picking(POINT& pt);
	void		Col_Moving(RECT& _tRect, POINT& pt);
public:
	static CTileMgr*		Get_Instance(void)
	{
		if (!m_pInstance)
		{
			m_pInstance = new CTileMgr;
		}

		return m_pInstance;
	}

	static void	Destroy_Instance(void)
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

private:
	static	CTileMgr*		m_pInstance;
	vector<CObj*>			m_vecSortTile;
	vector<CObj*>			m_vecTile;
	vector<CObj*>			m_vecDeco;
	vector<CObj*>			m_vecCol;
	vector<CObj*>			m_vecLava;
private :
	void Collision_RectEx(vector<CObj*>* _Sour, CObj* _Dest);
public :
	void CTileMgr::Lava_Picking(POINT& pt);
private :
	void CTileMgr::Collision_RectEx(CPlayer* _Dest, list<CObj*>* _Sour);
	void CTileMgr::Collision_RectPlayer(vector<CObj*>* _Sour, CObj* _Dest);
	void ProcessPrision()
	{
		CSoundMgr::Get_Instance()->Set_Sound(L"PRISON_START.mp3", SOUND_MAP, EFFECT_VOLUME);
		for (auto& elem : m_vecDeco)
		{
			elem->Set_Area(TILECX, TILECY);
			//elem->Update_Rect();
		}
		Set_IronProcess(false);
	}
	CObj* m_pProTile = nullptr;
	void Set_TargetTile(CObj* _pObj){ m_pProTile = _pObj; }
	CObj* Get_TargetTile() { return m_pProTile; }
	void ProcessIron()
	{
		CSoundMgr::Get_Instance()->Set_Sound(L"PRISON_END.mp3", SOUND_MAP, EFFECT_VOLUME);
		for (auto& elem : m_vecDeco)
		{
			elem->Set_Area(0, 0);
			elem->Update_Rect();
		}
		Set_IronProcess(true);
	}
	void ProcessMonster1(CObj* _Dest);
	void ProcessMonster2(CObj* _Dest);
	void ProcessMonster3(CObj* _Dest);
	void ProcessMonster4(CObj* _Dest);
	void ProcessMonster5(CObj* _Dest);
	void ProcessMonster6(CObj* _Dest);
	void ProcessMonster7(CObj* _Dest);
	void ProcessMonster8(CObj* _Dest);
};


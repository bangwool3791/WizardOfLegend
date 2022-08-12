#include "stdafx.h"
#include "TileMgr.h"
#include "Deco.h"
#include "ChildEditer.h"
#include "Player.h"
//거의 사용하는 헤더
#include "UserHeader.h"
#include "ColisionTile.h"
#include "CollisionMgr.h"
#include "SwordMan.h"
#include "ArcherMan.h"
#include "Wizard.h"
#include "WizardBall.h"
#include "LavaTile.h"
#include "MiddleBoss.h"
#include "Boss.h"
CTileMgr*	CTileMgr::m_pInstance = nullptr;

CTileMgr::CTileMgr():m_iDecoSize(-1), m_iColSize(-1), m_iLavaSize(-1)
{
}

CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.reserve(TILEX * TILEY);
	m_vecDeco.reserve(TILEX * TILEY);
	m_vecCol.reserve(0xFF);
	m_vecLava.reserve(0xFF);
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float	fX = float((TILECX * j) + (TILECX >> 1));
			float	fY = float((TILECY * i) + (TILECY >> 1));
			m_vecTile.push_back(CAbstactFactory<CTile>::Create(fX, fY));
		}
	}
	m_pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	m_pObjMgr = CObjMgr::Get_Instance();
	m_pScrollMgr = CScrollMgr::Get_Instance();
	m_hDCMiniMap = CBmpMgr::Get_Instance()->Find_Image(L"MiniMap");
	m_hDCMiniPlayer = CBmpMgr::Get_Instance()->Find_Image(L"PLAYER_MINIMAP");
	m_bf.AlphaFormat = 0;
	m_bf.BlendFlags = 0;
	m_bf.BlendOp = AC_SRC_OVER;
	m_bf.SourceConstantAlpha = 100;

		//맵 이벤트
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/PRISON_START.mp3", SOUND_MAP, EFFECT_VOLUME);
	CSoundMgr::Get_Instance()->PlaySoundW(L"../Sound/PRISON_END.mp3", SOUND_MAP, EFFECT_VOLUME);

	Set_IronProcess(false);
}

void CTileMgr::Update(void)
{
	//for (auto& elem : m_vecCol)
	//	elem->Update();
	//for (auto& elem : m_vecLava)
	//	elem->Update();

	if (m_bProIronProcess != m_bIronProcess)
	{
		for (auto& elem : m_vecDeco)
			elem->Update();
		m_bProIronProcess = m_bIronProcess;
	}

	Update_Process();
}

void CTileMgr::Late_Update(void)
{
	if (m_pPlayer)
	{
		int iPlayerX = (int)(m_pPlayer->Get_Info().fX / 32);
		int iPlayerY = (int)(m_pPlayer->Get_Info().fY / 32);
		int	iCullX = (int)(iPlayerX - CULLING_DISTANCE / 32);
		int	iCullY = (int)(iPlayerY - CULLING_DISTANCE / 32);

		int	iCullEndX = iCullX + CULLING_DISTANCE / 32 + 2;
		int	iCullEndY = iCullY + CULLING_DISTANCE / 32 + 2;

		//충돌 타일 깔고 벽과 충돌 처리 다시 할 것.
		for (int i = iCullY; i < iCullEndY; ++i)
		{
			for (int j = iCullX; j < iCullEndX; ++j)
			{
				int	iIndex = i * TILEX + j;

				if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
					continue;
				Collision_Tile(m_vecTile[iIndex], m_pPlayer);
			}
		}

		list<CObj*>* pList = CObjMgr::Get_Instance()->Get_TargetList(SKILL_KEY, SKILL_FDRAGON);
		for (vector<CObj*>::iterator iter = m_vecCol.begin(); iter != m_vecCol.end(); ++iter)
		{
			if (pList->empty())
				break;

			Collision_Tile(*iter, pList, 0);
		}


		for (int i = SWORDMAN; i < BOSS; ++i)
		{
			list<CObj*>* pList = CObjMgr::Get_Instance()->Get_TargetList(MONSTER_KEY, i);

			for (list<CObj*>::iterator iter = (*pList).begin(); iter != (*pList).end(); ++iter)
			{
				if (pList->empty())
					break;

				Collision_RectEx(&m_vecCol, *iter);
			}
		}

		for (int i = SWORDMAN; i < BOSS; ++i)
		{
			list<CObj*>* pList = CObjMgr::Get_Instance()->Get_TargetList(MONSTER_KEY, i);

			for (list<CObj*>::iterator iter = (*pList).begin(); iter != (*pList).end(); ++iter)
			{
				if (pList->empty())
					break;

				Collision_RectEx(&m_vecLava, *iter);
			}
		}
		Collision_RectPlayer(&m_vecCol, CObjMgr::Get_Instance()->Get_Player());
		
		if(PRO_SKILL_RUN == m_pPlayer->Get_PreSKillState())
		{
			Collision_Lava(&m_vecLava);
		}else
			Collision_RectPlayer(&m_vecLava, m_pPlayer);

		if (!m_bIronProcess)
				Collision_RectPlayer(&m_vecDeco, m_pPlayer);
	}
}

void CTileMgr::Render(HDC _DC)
{
	int	iCullX = int(abs(CScrollMgr::Get_Instance()->Get_ScrollX() / 32));
	int	iCullY = int(abs(CScrollMgr::Get_Instance()->Get_ScrollY() / 32));

	int	iCullEndX = iCullX + WINCX / 32 + 2;
	int	iCullEndY = iCullY + WINCY / 32 + 2;

	Rectangle(m_hDCMiniMap, 0, 0, 170, 150);
	for (int i = iCullY; i < iCullEndY; ++i)
	{
		for (int j = iCullX; j < iCullEndX; ++j)
		{
			int	iIndex = i * TILEX + j;

			if (0 > iIndex || (size_t)iIndex >= m_vecTile.size())
				continue;

			m_vecTile[iIndex]->Render(_DC);
			if (TILE_OPT_MINIMAP == m_vecTile[iIndex]->Get_Info().iOption)
			{
			Rectangle(m_hDCMiniMap, (int)(((m_vecTile[iIndex]->Get_Info().fX + CScrollMgr::Get_Instance()->Get_ScrollX()) / 800 * 170) -3.4),
					(int)(((m_vecTile[iIndex]->Get_Info().fY + CScrollMgr::Get_Instance()->Get_ScrollY()) / 600 * 150) -3.4),
					(int)(((m_vecTile[iIndex]->Get_Info().fX + CScrollMgr::Get_Instance()->Get_ScrollX()) / 800 * 170) + 3.4),
					(int)(((m_vecTile[iIndex]->Get_Info().fY + CScrollMgr::Get_Instance()->Get_ScrollY()) / 600 * 150) + 3.4));
			}
		}
	}
	if (m_pPlayer)
	{
		GdiTransparentBlt(m_hDCMiniMap, (int)(((m_pPlayer->Get_Info().fX + CScrollMgr::Get_Instance()->Get_ScrollX()) / 800 * 170) - FCX_MINI_PLAYER * 0.5f),
			(int)(((m_pPlayer->Get_Info().fY + CScrollMgr::Get_Instance()->Get_ScrollY()) / 600 * 150) - FCY_MINI_PLAYER * 0.5f),
			(int)FCX_MINI_PLAYER,
			(int)FCY_MINI_PLAYER,
			m_hDCMiniPlayer,
			0,
			0,
			(int)FCX_MINI_PLAYER,
			(int)FCY_MINI_PLAYER,
			RGB(255, 0, 255));
	}
	AlphaBlend(_DC, 630, 450, 170, 150, m_hDCMiniMap, 0, 0, 170, 150, m_bf);
	//장식물 랜더

	for (auto& elem : m_vecDeco)
		elem->Render(_DC);


	//for (auto& elem : m_vecCol)
	//	elem->Render(_DC);

	//for (auto& elem : m_vecLava)
	//	elem->Render(_DC);
}

void CTileMgr::Release(void)
{
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	for_each(m_vecDeco.begin(), m_vecDeco.end(), CDeleteObj());
	m_vecDeco.clear();
	for_each(m_vecCol.begin(), m_vecCol.end(), CDeleteObj());
	m_vecCol.clear();
	for_each(m_vecLava.begin(), m_vecLava.end(), CDeleteObj());
	m_vecLava.clear();
}

void CTileMgr::Tile_Picking(POINT& pt, int _iDrawXID, int _iDrawYID, TILE_OPT _iOption)
{
	if (0 <= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		WINCX >= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		0 <= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY() &&
		WINCY >= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY())
	{
		int		x = pt.x / 32;
		int		y = pt.y / 32;
		int		iIndex = y * TILEX + x;
		if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
			return;


		m_vecTile[iIndex]->Set_DrawXID(_iDrawXID);
		m_vecTile[iIndex]->Set_DrawYID(_iDrawYID);
		m_vecTile[iIndex]->Set_Option(_iOption);
	}
}

void CTileMgr::Tile_Picking(POINT& pt, int _iDrawXID, int _iDrawYID)
{
	if (0 <= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		WINCX >= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		0 <= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY() &&
		WINCY >= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY())
	{
		int		x = pt.x / 32;
		int		y = pt.y / 32;

		int		iIndex = y * TILEX + x;

		if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
			return;

		m_vecTile[iIndex]->Set_DrawXID(_iDrawXID);
		m_vecTile[iIndex]->Set_DrawYID(_iDrawYID);
	}
}

void CTileMgr::Tile_Picking(POINT& pt, TILE_OPT _iOption)
{
	if (0 <= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		WINCX >= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		0 <= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY() &&
		WINCY >= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY())
	{
		int		x = pt.x / 32;
		int		y = pt.y / 32;
		int		iIndex = y * TILEX + x;
		if (0 > iIndex || m_vecTile.size() <= (size_t)iIndex)
			return;

		m_vecTile[iIndex]->Set_Option(_iOption);
	}
}

void CTileMgr::Deco_Picking(POINT& pt, int _iDrawXID, int _iDrawYID, TILE_OPT _iOption)
{
	if (0 <= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		WINCX >= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		0 <= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY() &&
		WINCY >= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY())
	{
		float		x = (TILECX*(pt.x / 32) + TILECX * 0.5f);
		float		y = (TILECY*(pt.y / 32) + TILECY * 0.5f);
		m_vecDeco.push_back(CAbstactFactory<CDeco>::Create(x, y));
		Set_DecoSize(1);
		m_vecDeco[m_iDecoSize]->Set_DrawXID(_iDrawXID);
		m_vecDeco[m_iDecoSize]->Set_DrawYID(_iDrawYID);
		m_vecDeco[m_iDecoSize]->Set_Option(_iOption);
		m_vecDeco[m_iDecoSize]->Set_Area(TILECX, TILECY);
	}
}

void CTileMgr::Deco_Picking(POINT& pt, int _iDrawXID, int _iDrawYID)
{
	if (0 <= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		WINCX >= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		0 <= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY() &&
		WINCY >= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY())
	{
		float		x = (TILECX*(pt.x / 32) + TILECX * 0.5f);
		float		y = (TILECY*(pt.y / 32) + TILECY * 0.5f);
		m_vecDeco.push_back(CAbstactFactory<CDeco>::Create(x, y));
		Set_DecoSize(1);
		m_vecDeco[m_iDecoSize]->Set_DrawXID(_iDrawXID);
		m_vecDeco[m_iDecoSize]->Set_DrawYID(_iDrawYID);
		m_vecDeco[m_iDecoSize]->Set_Area(TILECX, TILECY);
	}
}

void CTileMgr::Col_Picking(POINT& pt)
{
	if (0 <= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		WINCX >= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		0 <= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY() &&
		WINCY >= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY())
	{
		float		x = (float)(COLLISION_TILECX * (pt.x / COLLISION_TILECX));
		float		y = (float)(COLLISION_TILECY * (pt.y / COLLISION_TILECY));
		CObj* m_pCol = CAbstactFactory<CColisionTile>::Create(x, y);
		Collision_RectEx(&m_vecCol, m_pCol);
		m_vecCol.push_back(m_pCol);
		Set_ColSize(1);
	}
}

void CTileMgr::Lava_Picking(POINT& pt)
{
	if (0 <= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		WINCX >= pt.x + CScrollMgr::Get_Instance()->Get_ScrollX() &&
		0 <= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY() &&
		WINCY >= pt.y + CScrollMgr::Get_Instance()->Get_ScrollY())
	{
		float		x = (float)(COLLISION_TILECX * (pt.x / COLLISION_TILECX));
		float		y = (float)(COLLISION_TILECY * (pt.y / COLLISION_TILECY));
		CObj* m_pCol = CAbstactFactory<CLavaTile>::Create(x, y);
		Collision_RectEx(&m_vecLava, m_pCol);
		m_vecLava.push_back(m_pCol);
		Set_LavalSize(1);
	}
}

void CTileMgr::Col_Moving(RECT& _tRect, POINT& pt)
{
	for (vector<CObj*>::iterator iter = m_vecCol.begin(); iter != m_vecCol.end(); ++iter)
	{
		if (_tRect.left <= (*iter)->Get_Info().fX &&
			_tRect.top <= (*iter)->Get_Info().fY &&
			_tRect.right >= (*iter)->Get_Info().fX &&
			_tRect.bottom >= (*iter)->Get_Info().fY
			)
		{
			(*iter)->Set_TilePosX((float)pt.x);
			(*iter)->Set_TilePosY((float)pt.y);
		}
	}
}

void CTileMgr::Collision_Tile(CObj* _Dest, CObj* _Sour)
{
	static DWORD dTick = 0;
	float	fX = 0.f, fY = 0.f;

	if (TILE_OPT_END == _Dest->Get_Option())
		return;

	switch (_Dest->Get_Option())
	{
	case TILE_OPT_IRON:
		ProcessIron();
		_Dest->Set_Option(TILE_OPT_END);
		break;
	case TILE_OPT_MONSTER_1:
		Set_TargetTile(_Dest);
		Set_TilePro(PRO_MONSTER_1);
		m_pPlayer->Set_RecoverPoint(_Dest->Get_Info().fX, _Dest->Get_Info().fY);
		_Dest->Set_Option(TILE_OPT_END);
		break;
	case TILE_OPT_MONSTER_2:
		Set_TargetTile(_Dest);
		Set_TilePro(PRO_MONSTER_2);
		m_pPlayer->Set_RecoverPoint(_Dest->Get_Info().fX, _Dest->Get_Info().fY);
		_Dest->Set_Option(TILE_OPT_END);
		break;
	case TILE_OPT_MONSTER_3:
		Set_TargetTile(_Dest);
		Set_TilePro(PRO_MONSTER_3);
		m_pPlayer->Set_RecoverPoint(_Dest->Get_Info().fX, _Dest->Get_Info().fY);
		_Dest->Set_Option(TILE_OPT_END);
		break;
	case TILE_OPT_MONSTER_4:
		Set_TargetTile(_Dest);
		Set_TilePro(PRO_MONSTER_4);
		m_pPlayer->Set_RecoverPoint(_Dest->Get_Info().fX, _Dest->Get_Info().fY);
		_Dest->Set_Option(TILE_OPT_END);
		break;
	case TILE_OPT_MONSTER_5:
		ProcessPrision();
		m_pProTile = _Dest;
		Set_TilePro(PRO_MONSTER_5);
		m_pPlayer->Set_RecoverPoint(_Dest->Get_Info().fX, _Dest->Get_Info().fY);
		_Dest->Set_Option(TILE_OPT_END);
		break;
	case TILE_OPT_MONSTER_6:
		m_pProTile = _Dest;
		Set_TilePro(PRO_MONSTER_6);
		m_pPlayer->Set_RecoverPoint(_Dest->Get_Info().fX, _Dest->Get_Info().fY);
		_Dest->Set_Option(TILE_OPT_END);
		break;
	case TILE_OPT_MONSTER_7:
		m_pProTile = _Dest;
		Set_TilePro(PRO_MONSTER_7);
		m_pPlayer->Set_RecoverPoint(_Dest->Get_Info().fX, _Dest->Get_Info().fY);
		_Dest->Set_Option(TILE_OPT_END);
		break;
	case TILE_OPT_MONSTER_8:
		m_pProTile = _Dest;
		Set_TilePro(PRO_MONSTER_8);
		m_pPlayer->Set_RecoverPoint(_Dest->Get_Info().fX, _Dest->Get_Info().fY);
		_Dest->Set_Option(TILE_OPT_END);
		break;
	case TILE_OPT_MONSTER_10:
		ProcessPrision();
		Set_TilePro(PRO_MONSTER_10);
		m_pObjMgr->AddObject(MONSTER_KEY, BOSS, CAbstactFactory<CBoss>::Create(_Dest->Get_Info().fX, _Dest->Get_Info().fY));
		_Dest->Set_Option(TILE_OPT_END);
		break;
		
	}
}

CObj* CTileMgr::Get_MinDistanceTile()
{
	float fDistance = FLT_MAX;
	float fX = 0.f;
	float fY = 0.f;
	float fR = 0.f;
	CObj* ReturnObj = nullptr;
	if (m_pPlayer)
	{
		for (auto& elem : m_vecSortTile)
		{
			//버그 : 느려짐 수정 필요
			if (TILE_OPT_IRON != elem->Get_Option())
				break;

			fX = Displacement<float>(elem->Get_Info().fX, m_pPlayer->Get_Info().fX);
			fY = Displacement<float>(elem->Get_Info().fY, m_pPlayer->Get_Info().fY);
			fR = Distance<float>(fX, fY);
			if (fDistance > fR)
			{
				fDistance = fR;
				ReturnObj = elem;
			}

		}
	}
	return ReturnObj;
}
void CTileMgr::Save_Process()
{
	HANDLE		hFile;
	DWORD		dwByte = 0;
	DeleteFileA("../Data/Tile.dat");
	hFile = CreateFile(L"../Data/Tile.dat",		// 저장할 파일의 경로와 이름
		GENERIC_WRITE,			// 파일 접근 모드 (GENERIC_WRITE 파일 출력, GENERIC_READ 파일 입력)
		NULL,					// 공유방식, 파일이 열려 있는 상태에서 다른 프로그램에서 오픈할 때 허가 할 것인가(null인 경우 공유하지 않는다)
		NULL,					// 보안속성, null인 기본값으로 보안상태를 설정
		CREATE_ALWAYS,			// 생성방식, 해당 파일을 열어서 작업할 것인지 새로 만들것인지 결정, CREATE_ALWAYS 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISITING 파일이 있을 경우에만 여는 옵션)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성지정(읽기 전용, 숨김 모드 등등) FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반적인 파일을 지정
		NULL);					// 생성된 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않을 것이기 때문에 null

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키코드 문자열을 유니코드로 전환시켜주는 매크로
		MessageBox(g_hWnd, _T("Save File"), _T("Fail"), MB_OK);
		// 2인자 : 팝업 창에 띄우고자 하는 메세지
		// 3인자 : 팝업 창의 이름
	}

	for (auto& elem : m_vecTile)
	{
		WriteFile(hFile, &elem->Get_Info(), sizeof(INFO), &dwByte, nullptr);
	}

	dwByte = 0;
	CloseHandle(hFile);


	DeleteFileA("../Data/Deco.dat");
	hFile = CreateFile(L"../Data/Deco.dat",	
		GENERIC_WRITE,		
		NULL,					
		NULL,					
		CREATE_ALWAYS,			
		FILE_ATTRIBUTE_NORMAL,	
		NULL);					

	if (INVALID_HANDLE_VALUE == hFile)
		MessageBox(g_hWnd, _T("Save File"), _T("Fail"), MB_OK);

	for (auto& elem : m_vecDeco)
	{
		WriteFile(hFile, &elem->Get_Info(), sizeof(INFO), &dwByte, nullptr);
	}

	dwByte = 0;
	CloseHandle(hFile);

	DeleteFileA("../Data/Col.dat");
	hFile = CreateFile(L"../Data/Col.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		MessageBox(g_hWnd, _T("Save File"), _T("Fail"), MB_OK);

	for (auto& elem : m_vecCol)
	{
		WriteFile(hFile, &elem->Get_Info(), sizeof(INFO), &dwByte, nullptr);
	}

	dwByte = 0;
	CloseHandle(hFile);

	DeleteFileA("../Data/Lava.dat");
	hFile = CreateFile(L"../Data/Lava.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		MessageBox(g_hWnd, _T("Save File"), _T("Fail"), MB_OK);

	for (auto& elem : m_vecLava)
	{
		WriteFile(hFile, &elem->Get_Info(), sizeof(INFO), &dwByte, nullptr);
	}

	dwByte = 0;
	CloseHandle(hFile);
	MessageBox(g_hWnd, _T("Save Succes"), _T("Succes"), MB_OK);
}

bool CTileMgr::Load_Process()
{
	HANDLE		hFile;
	DWORD		dwByte = 0;
	INFO		tInfo;
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.reserve(TILEX * TILEY);

	hFile = CreateFile(L"../Data/Tile.dat",		// 저장할 파일의 경로와 이름
		GENERIC_READ,			// 파일 접근 모드 (GENERIC_WRITE 파일 출력, GENERIC_READ 파일 입력)
		NULL,					// 공유방식, 파일이 열려 있는 상태에서 다른 프로그램에서 오픈할 때 허가 할 것인가(null인 경우 공유하지 않는다)
		NULL,					// 보안속성, null인 기본값으로 보안상태를 설정
		OPEN_EXISTING,			// 생성방식, 해당 파일을 열어서 작업할 것인지 새로 만들것인지 결정, CREATE_ALWAYS 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISITING 파일이 있을 경우에만 여는 옵션)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성지정(읽기 전용, 숨김 모드 등등) FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반적인 파일을 지정
		NULL);					// 생성된 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않을 것이기 때문에 null
	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키코드 문자열을 유니코드로 전환시켜주는 매크로
		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
		return false;
	}

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		m_vecTile.push_back(CAbstactFactory<CTile>::Create(tInfo));
		if (!dwByte)
			break;
	}

	dwByte = 0;
	CloseHandle(hFile);

	m_vecDeco.clear();
	m_vecDeco.reserve(TILEX * TILEY);
	m_iDecoSize = 0-1;
	hFile = CreateFile(L"../Data/Deco.dat",		// 저장할 파일의 경로와 이름
		GENERIC_READ,			// 파일 접근 모드 (GENERIC_WRITE 파일 출력, GENERIC_READ 파일 입력)
		NULL,					// 공유방식, 파일이 열려 있는 상태에서 다른 프로그램에서 오픈할 때 허가 할 것인가(null인 경우 공유하지 않는다)
		NULL,					// 보안속성, null인 기본값으로 보안상태를 설정
		OPEN_EXISTING,			// 생성방식, 해당 파일을 열어서 작업할 것인지 새로 만들것인지 결정, CREATE_ALWAYS 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISITING 파일이 있을 경우에만 여는 옵션)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성지정(읽기 전용, 숨김 모드 등등) FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반적인 파일을 지정
		NULL);					// 생성된 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않을 것이기 때문에 null

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키코드 문자열을 유니코드로 전환시켜주는 매크로
		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
		return false;
	}

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		m_vecDeco.push_back(CAbstactFactory<CDeco>::Create(tInfo));
		Set_DecoSize(1);
		if (!dwByte)
			break;
	}
	dwByte = 0;
	CloseHandle(hFile);

	for_each(m_vecCol.begin(), m_vecCol.end(), CDeleteObj());
	m_vecCol.clear();
	m_vecCol.reserve(0xFF);
	m_iColSize = 0;

	hFile = CreateFile(L"../Data/Col.dat",		// 저장할 파일의 경로와 이름
		GENERIC_READ,			// 파일 접근 모드 (GENERIC_WRITE 파일 출력, GENERIC_READ 파일 입력)
		NULL,					// 공유방식, 파일이 열려 있는 상태에서 다른 프로그램에서 오픈할 때 허가 할 것인가(null인 경우 공유하지 않는다)
		NULL,					// 보안속성, null인 기본값으로 보안상태를 설정
		OPEN_EXISTING,			// 생성방식, 해당 파일을 열어서 작업할 것인지 새로 만들것인지 결정, CREATE_ALWAYS 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISITING 파일이 있을 경우에만 여는 옵션)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성지정(읽기 전용, 숨김 모드 등등) FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반적인 파일을 지정
		NULL);					// 생성된 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않을 것이기 때문에 null

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키코드 문자열을 유니코드로 전환시켜주는 매크로
		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
		return false;
	}

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		m_vecCol.push_back(CAbstactFactory<CColisionTile>::Create(tInfo));
		Set_ColSize(1);
		if (!dwByte)
			break;
	}

	dwByte = 0;
	CloseHandle(hFile);

	for_each(m_vecLava.begin(), m_vecLava.end(), CDeleteObj());
	m_vecLava.clear();
	m_vecLava.reserve(0xFF);
	m_iLavaSize = 0;

	hFile = CreateFile(L"../Data/Lava.dat",		// 저장할 파일의 경로와 이름
		GENERIC_READ,			// 파일 접근 모드 (GENERIC_WRITE 파일 출력, GENERIC_READ 파일 입력)
		NULL,					// 공유방식, 파일이 열려 있는 상태에서 다른 프로그램에서 오픈할 때 허가 할 것인가(null인 경우 공유하지 않는다)
		NULL,					// 보안속성, null인 기본값으로 보안상태를 설정
		OPEN_EXISTING,			// 생성방식, 해당 파일을 열어서 작업할 것인지 새로 만들것인지 결정, CREATE_ALWAYS 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISITING 파일이 있을 경우에만 여는 옵션)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성지정(읽기 전용, 숨김 모드 등등) FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반적인 파일을 지정
		NULL);					// 생성된 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않을 것이기 때문에 null

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키코드 문자열을 유니코드로 전환시켜주는 매크로
		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
		return false;
	}

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		m_vecLava.push_back(CAbstactFactory<CLavaTile>::Create(tInfo));
		Set_LavalSize(1);
		if (!dwByte)
			break;
	}

	dwByte = 0;
	CloseHandle(hFile);
	return true;
}

void CTileMgr::Collision_RectEx(vector<CObj*>* _Sour, CObj* _Dest)
{
	RECT		rc{};

	for (auto& Sour : *_Sour)
	{
		float	fX = 0.f, fY = 0.f;

		fX = Displacement<float>(Sour->Get_Info().fX, _Dest->Get_Info().fX);
		fY = Displacement<float>(_Dest->Get_Info().fY, Sour->Get_Info().fY);

		if (100 <= Distance<float>(fX,fY))
			continue;

			if (CCollisionMgr::Check_Rect(_Dest, Sour, &fX, &fY))
		{
			if (fX > fY)	// 상하 충돌의 경우
			{
				// 상 충돌
				if (_Dest->Get_Info().fY < Sour->Get_Info().fY)
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
				if (_Dest->Get_Info().fX < Sour->Get_Info().fX)
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
}


void CTileMgr::Collision_RectPlayer(vector<CObj*>* _Sour, CObj* _Dest)
{
	RECT		rc{};

	for (vector<CObj*>::iterator iter = _Sour->begin(); iter != _Sour->end();++iter)
	{
		float	fX = 0.f, fY = 0.f;

		fX = Displacement<float>((*iter)->Get_Info().fX, _Dest->Get_Info().fX);
		fY = Displacement<float>(_Dest->Get_Info().fY, (*iter)->Get_Info().fY);

		if (100 <= Distance<float>(fX, fY))
			continue;

		if (CCollisionMgr::Check_Rect(_Dest, (*iter), &fX, &fY))
		{
			if (fX > fY)	// 상하 충돌의 경우
			{
				// 상 충돌
				if (_Dest->Get_Info().fY < (*iter)->Get_Info().fY)
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
				if (_Dest->Get_Info().fX < (*iter)->Get_Info().fX)
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
}


void CTileMgr::Collision_Tile(CObj* _Sour, list<CObj*>* _Dest)
{
	RECT		rc{};

	float	fX = 0.f, fY = 0.f;

	for (list<CObj*>::iterator iter = (*_Dest).begin(); iter != (*_Dest).end(); ++iter)
	{
		if (CCollisionMgr::Check_Rect(*iter, _Sour, &fX, &fY))
		{
			(*iter)->Set_Dead();
			CObjMgr::Get_Instance()->AddObject(EFFECT_KEY, EFFECT_HIT, CAbstactFactory<CHitEffect>::Create((*iter)->Get_Info().fX, (*iter)->Get_Info().fY));
		}
	}
}


void CTileMgr::Collision_Tile(CObj* _Sour, list<CObj*>* _Dest, int i )
{
	RECT		rc{};

	float	fX = 0.f, fY = 0.f;

	for (list<CObj*>::iterator iter = (*_Dest).begin(); iter != (*_Dest).end(); ++iter)
	{
		if (CCollisionMgr::Check_Rect(*iter, _Sour, &fX, &fY))
		{
			(*iter)->Set_Dead();
			CObjMgr::Get_Instance()->AddObject(EFFECT_KEY, EFFECT_HIT, CAbstactFactory<CHitEffect>::Create((*iter)->Get_Info().fX, (*iter)->Get_Info().fY));
			CScrollMgr::Get_Instance()->Set_ShakeTick();
		}
	}
}
void  CTileMgr::Collision_Lava(vector<CObj*>* _Sour)
{
	RECT		rc{};
	int			iIndex = 0;
	for (auto& Sour : *_Sour)
	{
		float	fX = 0.f, fY = 0.f;

		if (PRO_SKILL_RUN == m_pPlayer->Get_ProSKill())
			continue;

		if (CCollisionMgr::Check_Rect((CObj*)m_pPlayer, Sour, &fX, &fY))
		{
			m_pPlayer->Process_Recover_Point();
		}
	}
}

void CTileMgr::Collision_RectEx(CPlayer* _Dest, list<CObj*>* _Sour)
{
	RECT		rc{};

	for (auto& Sour : *_Sour)
	{
		float	fX = 0.f, fY = 0.f;

		if (Sour->Get_Rect().left <= _Dest->Get_Info().fX &&
			Sour->Get_Rect().top <= _Dest->Get_Info().fY &&
			Sour->Get_Rect().right >= _Dest->Get_Info().fX &&
			Sour->Get_Rect().bottom >= _Dest->Get_Info().fX 
			)
		{
			_Dest->Process_Recover_Point();
		}
	}
}

void CTileMgr::ProcessMonster1(CObj* _Dest)
{
	static int iCount = 0;
	static bool bLock = false;

	if (bLock)
	{
		if (m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty())
			bLock = false;
		return;
	}

	switch (iCount)
	{
		case 0:
			ProcessPrision();
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 0.5f, _Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 0.5f));
			bLock = true;
			iCount = 1;
		break;
		case 1:
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 0.5f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 0.5f)));
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 0.5f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 0.5f)));
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 0.5f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 0.5f)));
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 0.5f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 0.5f)));

			bLock = true;
			iCount = 2;
			break;
		case 2:
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 0.5f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 0.5f)));
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 0.5f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 0.5f)));
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 0.5f),( _Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 0.5f)));
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 0.5f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 0.5f)));

			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD, _Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD));
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD, _Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD));
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD, _Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD));
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
				_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD, _Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD));

			bLock = true;
			iCount =3;
			break;
		case 3:
			ProcessIron();
			break;
	}
}
void CTileMgr::ProcessMonster2(CObj* _Dest)
{
	static int iCount = 0;
	static bool bLock = false;
	static float fRadius = 160.f;
	static DWORD dTick = 0;
	if (bLock)
	{
		switch (iCount)
		{
		case 0:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty())
			{
				bLock = false;
				iCount = 1;
			}
			break;
		case 1:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty())
			{
				bLock = false;
				iCount = 2;
			}
			break;
		case 2:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty())
			{
				if (dTick + MONSTER_RESPONE_TIME < GetTickCount())
				{
					bLock = false;
					iCount = 3;
				}
			}
			else
				dTick = GetTickCount();

			break;
		case 3:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty())
			{
				bLock = false;
				iCount = 4;
			}

			break;
		case 4:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty())
			{
				ProcessIron();
				Set_TilePro(PRO_END);
			}

			break;
		}

		return;
	}

	switch (iCount)
	{
	case 0:
		ProcessPrision();
		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 0.5f)));
			m_pObjMgr->Get_Target(MONSTER_KEY, ACHERMAN, nullptr)->Set_DeadEvent(CObj::DEAD_EVENT_ICEBLAST);
		bLock = true;
		break;
	case 1:
		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 2), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 2), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));
		m_pObjMgr->Get_Target(MONSTER_KEY, ACHERMAN, nullptr)->Set_DeadEvent(CObj::DEAD_EVENT_CRYSTAL);
		bLock = true;
		break;
	case 2:
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX), (_Dest->Get_Info().fY - fRadius)));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX), (_Dest->Get_Info().fY + fRadius)));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX - fRadius), (_Dest->Get_Info().fY)));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius), (_Dest->Get_Info().fY)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 45 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 45 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 135 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 135 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 215 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 215 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 315 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 315 / 180))));
		m_pObjMgr->Get_Target(MONSTER_KEY, ACHERMAN, nullptr)->Set_DeadEvent(CObj::DEAD_EVENT_FDRAGONE);
		bLock = true;
		break;
	case 3:
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 2), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 2), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 2), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 2), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));
		bLock = true;
		break;
	case 4:
		ProcessIron();
		Set_TilePro(PRO_END);
		break;
	}
}


void CTileMgr::ProcessMonster3(CObj* _Dest)
{
	static int iCount = 0;
	static bool bLock = false;
	static DWORD dTick = 0;
	if (bLock)
	{
		switch (iCount)
		{
		case 0:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty())
			{
				bLock = false;
				iCount = 1;
			}

			break;
		case 1:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty())
			{
				bLock = false;
				Set_TilePro(PRO_END);
				ProcessIron();
			}
			break;
		}

		return;
	}

	switch (iCount)
	{
	case 0:
		ProcessPrision();
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.f, _Dest->Get_Info().fY));
		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			_Dest->Get_Info().fX - FCX_ARCHER_MONSTER_CARD * 5.0f, _Dest->Get_Info().fY));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.f, _Dest->Get_Info().fY));
		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			_Dest->Get_Info().fX + FCX_ARCHER_MONSTER_CARD * 5.0f, _Dest->Get_Info().fY));
		bLock = true;
		break;
	case 1:
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.f, _Dest->Get_Info().fY));
		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			_Dest->Get_Info().fX - FCX_ARCHER_MONSTER_CARD * 3.f, _Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD));
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 5.0f, _Dest->Get_Info().fY));
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 5.0f, _Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.f, _Dest->Get_Info().fY));
		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			_Dest->Get_Info().fX + FCX_ARCHER_MONSTER_CARD * 3.f, _Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD));
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 5.0f, _Dest->Get_Info().fY));
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 5.0f, _Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD));
		bLock = true;
		break;
	}
}

void CTileMgr::ProcessMonster4(CObj* _Dest)
{
	static int iCount = 0;
	static bool bLock = false;
	static float fRadius = 180.f;
	static DWORD dTick = 0;
	if (bLock)
	{
		switch (iCount)
		{
		case 0:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty())
			{
				bLock = false;
				iCount = 1;
			}
			break;
		case 1:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty())
			{
				bLock = false;
				iCount = 2;
			}
			break;
		case 2:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty())
			{
				if (dTick + MONSTER_RESPONE_TIME < GetTickCount())
				{
					bLock = false;
					Set_TilePro(PRO_END);
					ProcessIron();
				}
			}
			else
				dTick = GetTickCount();
			break;
		}

		return;
	}

	switch (iCount)
	{
	case 0:
		ProcessPrision();

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 0 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 0 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 90 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 90 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 180 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 180 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 270 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 270 / 180))));
		bLock = true;
		break;
	case 1:
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 45 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 45 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 90 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 90 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 135 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 135 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 180 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 180 / 180))));
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 225 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 225 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 270 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 270 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 315 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 315 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 360 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 360 / 180))));
		bLock = true;
		break;
	case 2:
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 0 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 0 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 1/ 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 1 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 2 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 2 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 3 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 3 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 4 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 4 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 5 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 5 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 6 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 6 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 7 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 7 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 8 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 8 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 9 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 9 / 180))));
		bLock = true;
		break;
	}
}


void CTileMgr::ProcessMonster5(CObj* _Dest)
{
	static int iCount = 0;
	static bool bLock = false;
	static float fRadius = 250.f;
	static DWORD dTick = 0;
	if (bLock)
	{
		switch (iCount)
		{
		case 0:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty())
			{
				bLock = false;
				iCount = 1;
			}
			else if (dTick + PRISON_RELASE_TIME < GetTickCount())
			{
				for (auto& elem : m_vecDeco)
				{
					Safe_Delete(elem);
				}
				m_vecDeco.clear();
				dTick = GetTickCount() + 0xFFFF;
			}
			break;
		case 1:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty())
			{
				bLock = false;
				iCount = 2;
			}
			break;
		case 2:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty())
			{
				if (dTick + MONSTER_RESPONE_TIME < GetTickCount())
				{
					bLock = false;
					Set_TilePro(PRO_END);
					ProcessIron();
				}
			}
			else
				dTick = GetTickCount();
			break;
		}

		return;
	}

	switch (iCount)
	{
	case 0:
		ProcessPrision();
		for(float i = 0.f; i < 360.f; i += 30.f)
		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX - 35.f + fRadius * cosf(PI * i / 180)), (_Dest->Get_Info().fY - 30.f - fRadius * sinf(PI * i / 180))));

		dTick = GetTickCount();
		bLock = true;
		break;
	case 1:
		//하나
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 2)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 2)));
		//둘
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 2)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 2)));

		//셋
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 2)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 2)));

		//넷
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 2)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 2)));
		bLock = true;
		break;
	case 2:
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 0 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 0 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 1 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 1 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 2 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 2 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 3 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 3 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 4 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 4 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 5 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 5 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 6 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 6 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 7 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 7 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 8 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 8 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 9 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 9 / 180))));
		bLock = true;
		break;
	}
}


void CTileMgr::ProcessMonster6(CObj* _Dest)
{
	
	static int iCount = 0;

	switch (iCount)
	{
	case 1:
		if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() && m_pObjMgr->Get_TargetList(MONSTER_KEY, MIDDLE_BOSS)->empty())
		{
			Set_TilePro(PRO_END);
		}
		else if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty())
		{
			m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
				2861.f, 173.f));

			m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
				2857.f, 677.f));
		}
		break;
	}


	switch (iCount)
	{
	case 0:
		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			2861.f, 173.f));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			2857.f, 677.f));

		m_pObjMgr->AddObject(MONSTER_KEY, MIDDLE_BOSS, CAbstactFactory<CMiddleBoss>::Create(
			_Dest->Get_Info().fX + FCX_MIDDLE_BOSS * 0.5f, _Dest->Get_Info().fY));
		iCount = 1;
		break;
	}
}


void CTileMgr::ProcessMonster7(CObj* _Dest)
{

	static int iCount = 0;
	static bool bLock = false;
	static float fRadius = 200.f;
	static DWORD dTick = 0;
	if (bLock)
	{
		switch (iCount)
		{
		case 0:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD_BALL)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD)->empty())
			{
				ProcessEraseFireBall();
				bLock = false;
				iCount = 1;
			}
			else if (dTick + PRISON_RELASE_TIME < GetTickCount())
			{
				dTick = GetTickCount() + 0xFFFF;
			}
			break;
		case 1:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD_BALL)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD)->empty())
			{
				ProcessEraseFireBall();
				bLock = false;
				iCount = 2;
			}
			break;
		case 2:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD_BALL)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD)->empty())
			{
				ProcessEraseFireBall();
				if (dTick + MONSTER_RESPONE_TIME < GetTickCount())
				{
					bLock = false;
					iCount = 3;
				}
			}
			else
				dTick = GetTickCount();
			break;
		case 3:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD_BALL)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD)->empty())
			{
				ProcessEraseFireBall();
				if (dTick + MONSTER_RESPONE_TIME < GetTickCount())
				{
					bLock = false;
					Set_TilePro(PRO_END);
					ProcessIron();
				}
			}
			else
				dTick = GetTickCount();
			break;
		}

		return;
	}

	static DWORD dResponTick = GetTickCount();

	if (dResponTick + 90000 < GetTickCount())
	{
		dResponTick = 0xFFFFFFFE;

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 2.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 2.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 1.5f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 1.5f)));
		//둘
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 2.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 2.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 1.5f)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 1.5f)));
	}

	switch (iCount)
	{
	case 0:
		ProcessPrision();
		for (float i = 0.f; i < 360.f; i += 30.f)
			m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX - 35.f + fRadius * cosf(PI * i / 180)), (_Dest->Get_Info().fY - 30.f - fRadius * sinf(PI * i / 180))));

		dTick = GetTickCount();
		bLock = true;
		break;
	case 1:
		//하나
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 2.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 2.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 1.5f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 1.5f)));
		//둘
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 2.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 2.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 1.5f)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.0f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 1.5f)));

		//셋
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 2.0f), (_Dest->Get_Info().fY )));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.0f), (_Dest->Get_Info().fY)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 2.0f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD )));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.0f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));

		//넷
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 2.0f), (_Dest->Get_Info().fY)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.0f), (_Dest->Get_Info().fY)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 2.0f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.0f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD)));
		bLock = true;
		break;
	case 2:
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 0 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 0 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 1 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 1 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 2 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 2 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 3 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 3 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 4 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 4 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 5 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 5 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 6 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 6 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 7 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 7 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 8 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 8 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 36 * 9 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 9 / 180))));
		bLock = true;
		break;
	case 3:
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 45 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 45 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 90 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 90 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 135 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 135 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 180 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 180 / 180))));
		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 225 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 225 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 270 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 270 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 315 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 315 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 360 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 360 / 180))));
		bLock = true;
		break;
	}
}


void CTileMgr::ProcessMonster8(CObj* _Dest)
{

	static int iCount = 0;
	static bool bLock = false;
	static float fRadius = 200.f;
	static DWORD dTick = 0;
	if (bLock)
	{
		switch (iCount)
		{
		case 0:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD_BALL)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD)->empty())
			{
				ProcessEraseFireBall();
				bLock = false;
				iCount = 1;
			}
			else if (dTick + PRISON_RELASE_TIME < GetTickCount())
			{
				dTick = GetTickCount() + 0xFFFF;
			}
			break;
		case 1:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD_BALL)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD)->empty())
			{
				ProcessEraseFireBall();
				bLock = false;
				iCount = 2;
			}
			break;
		case 2:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD_BALL)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD)->empty())
			{
				ProcessEraseFireBall();
				if (dTick + MONSTER_RESPONE_TIME < GetTickCount())
				{
					bLock = false;
					iCount = 3;
				}
			}
			else
				dTick = GetTickCount();
			break;
		case 3:
			if (m_pObjMgr->Get_TargetList(MONSTER_KEY, ACHERMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, SWORDMAN)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD_BALL)->empty() &&
				m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD)->empty())
			{
				ProcessEraseFireBall();
				if (dTick + MONSTER_RESPONE_TIME < GetTickCount())
				{
					bLock = false;
					Set_TilePro(PRO_END);
					ProcessIron();
				}
			}
			else
				dTick = GetTickCount();
			break;
		}

		return;
	}

	switch (iCount)
	{
	case 0:
		ProcessPrision();
		for (float i = 0.f; i < 360.f; i += 30.f)
			m_pObjMgr->AddObject(MONSTER_KEY, SWORDMAN, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX - 35.f + fRadius * cosf(PI * i / 180)), (_Dest->Get_Info().fY - 30.f - fRadius * sinf(PI * i / 180))));

		dTick = GetTickCount();
		bLock = true;
		break;
	case 1:
		//하나
		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 1.f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 4.f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 1.f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 2.f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 4.f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 2.f)));
		//둘
		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 1.f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 4.f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 1.f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 2.f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 4.f), (_Dest->Get_Info().fY - FCY_SWORD_MONSTER_CARD * 2.f)));

		//셋
		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 1.f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 4.f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 1.f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 2.f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX - FCX_SWORD_MONSTER_CARD * 4.f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 2.f)));

		//넷
		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 1.f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 4.f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 1.f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CWizardBall>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 3.f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 2.f)));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + FCX_SWORD_MONSTER_CARD * 4.f), (_Dest->Get_Info().fY + FCY_SWORD_MONSTER_CARD * 2.f)));
		bLock = true;
		break;
	case 2:
		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + (fRadius + 50) * cosf(PI * 36 * 0 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 0 / 180))));


		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + (fRadius + 50) * cosf(PI * 36 * 1 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 1 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + (fRadius + 50) * cosf(PI * 36 * 2 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 2 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + (fRadius - 50) * cosf(PI * 36 * 3 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 3 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + (fRadius - 50) * cosf(PI * 36 * 4 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 4 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + (fRadius - 50) * cosf(PI * 36 * 5 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 5 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + (fRadius - 50) * cosf(PI * 36 * 6 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 6 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + (fRadius - 50) * cosf(PI * 36 * 7 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 7 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD_BALL, CAbstactFactory<CSwordMan>::Create(
			(_Dest->Get_Info().fX + (fRadius + 50) * cosf(PI * 36 * 8 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 8 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + (fRadius + 50) * cosf(PI * 36 * 9 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 36 * 9 / 180))));
		bLock = true;
		break;
	case 3:
		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 45 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 45 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 90 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 90 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 135 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 135 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 180 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 180 / 180))));
		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 225 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 225 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 270 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 270 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, ACHERMAN, CAbstactFactory<CArcherMan>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 315 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 315 / 180))));

		m_pObjMgr->AddObject(MONSTER_KEY, WIZARD, CAbstactFactory<CWizard>::Create(
			(_Dest->Get_Info().fX + fRadius * cosf(PI * 360 / 180)), (_Dest->Get_Info().fY - fRadius * sinf(PI * 360 / 180))));
		bLock = true;
		break;
	}
}

void CTileMgr::Update_Process()
{
	switch (m_ePro)
	{
	case PRO_MONSTER_1:
		ProcessMonster1(Get_TargetTile());
		break;
	case PRO_MONSTER_2:
		ProcessMonster2(Get_TargetTile());
		break;
	case PRO_MONSTER_3:
		ProcessMonster3(Get_TargetTile());
		break;
	case PRO_MONSTER_4:
		ProcessMonster4(Get_TargetTile());
		break;
	case PRO_MONSTER_5:
		ProcessMonster5(Get_TargetTile());
		break;
	case PRO_MONSTER_6:
		ProcessMonster6(Get_TargetTile());
		break;
	case PRO_MONSTER_7:
		ProcessMonster7(Get_TargetTile());
		break;
	case PRO_MONSTER_8:
		ProcessMonster8(Get_TargetTile());
		break;
	}
}

void CTileMgr::Save_Process2()
{
	HANDLE		hFile;
	DWORD		dwByte = 0;
	DeleteFileA("../Data/Tile2.dat");
	hFile = CreateFile(L"../Data/Tile2.dat",		// 저장할 파일의 경로와 이름
		GENERIC_WRITE,			// 파일 접근 모드 (GENERIC_WRITE 파일 출력, GENERIC_READ 파일 입력)
		NULL,					// 공유방식, 파일이 열려 있는 상태에서 다른 프로그램에서 오픈할 때 허가 할 것인가(null인 경우 공유하지 않는다)
		NULL,					// 보안속성, null인 기본값으로 보안상태를 설정
		CREATE_ALWAYS,			// 생성방식, 해당 파일을 열어서 작업할 것인지 새로 만들것인지 결정, CREATE_ALWAYS 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISITING 파일이 있을 경우에만 여는 옵션)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성지정(읽기 전용, 숨김 모드 등등) FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반적인 파일을 지정
		NULL);					// 생성된 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않을 것이기 때문에 null

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키코드 문자열을 유니코드로 전환시켜주는 매크로
		MessageBox(g_hWnd, _T("Save File"), _T("Fail"), MB_OK);
		// 2인자 : 팝업 창에 띄우고자 하는 메세지
		// 3인자 : 팝업 창의 이름
	}

	for (auto& elem : m_vecTile)
	{
		WriteFile(hFile, &elem->Get_Info(), sizeof(INFO), &dwByte, nullptr);
	}

	dwByte = 0;
	CloseHandle(hFile);


	DeleteFileA("../Data/Deco2.dat");
	hFile = CreateFile(L"../Data/Deco2.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		MessageBox(g_hWnd, _T("Save File"), _T("Fail"), MB_OK);

	for (auto& elem : m_vecDeco)
	{
		WriteFile(hFile, &elem->Get_Info(), sizeof(INFO), &dwByte, nullptr);
	}

	dwByte = 0;
	CloseHandle(hFile);

	DeleteFileA("../Data/Col2.dat");
	hFile = CreateFile(L"../Data/Col2.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		MessageBox(g_hWnd, _T("Save File"), _T("Fail"), MB_OK);

	for (auto& elem : m_vecCol)
	{
		WriteFile(hFile, &elem->Get_Info(), sizeof(INFO), &dwByte, nullptr);
	}

	dwByte = 0;
	CloseHandle(hFile);

	DeleteFileA("../Data/Lava2.dat");
	hFile = CreateFile(L"../Data/Lava2.dat",
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (INVALID_HANDLE_VALUE == hFile)
		MessageBox(g_hWnd, _T("Save File"), _T("Fail"), MB_OK);

	for (auto& elem : m_vecLava)
	{
		WriteFile(hFile, &elem->Get_Info(), sizeof(INFO), &dwByte, nullptr);
	}

	dwByte = 0;
	CloseHandle(hFile);
	MessageBox(g_hWnd, _T("Save Succes"), _T("Succes"), MB_OK);
}

bool CTileMgr::Load_Process2()
{
	HANDLE		hFile;
	DWORD		dwByte = 0;
	INFO		tInfo;
	for_each(m_vecTile.begin(), m_vecTile.end(), CDeleteObj());
	m_vecTile.clear();
	m_vecTile.reserve(TILEX * TILEY);

	hFile = CreateFile(L"../Data/Tile2.dat",		// 저장할 파일의 경로와 이름
		GENERIC_READ,			// 파일 접근 모드 (GENERIC_WRITE 파일 출력, GENERIC_READ 파일 입력)
		NULL,					// 공유방식, 파일이 열려 있는 상태에서 다른 프로그램에서 오픈할 때 허가 할 것인가(null인 경우 공유하지 않는다)
		NULL,					// 보안속성, null인 기본값으로 보안상태를 설정
		OPEN_EXISTING,			// 생성방식, 해당 파일을 열어서 작업할 것인지 새로 만들것인지 결정, CREATE_ALWAYS 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISITING 파일이 있을 경우에만 여는 옵션)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성지정(읽기 전용, 숨김 모드 등등) FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반적인 파일을 지정
		NULL);					// 생성된 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않을 것이기 때문에 null
	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키코드 문자열을 유니코드로 전환시켜주는 매크로
		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
		return false;
	}

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		m_vecTile.push_back(CAbstactFactory<CTile>::Create(tInfo));
		if (!dwByte)
			break;
	}

	dwByte = 0;
	CloseHandle(hFile);

	m_vecDeco.clear();
	m_vecDeco.reserve(TILEX * TILEY);
	m_iDecoSize = - 1;
	hFile = CreateFile(L"../Data/Deco2.dat",		// 저장할 파일의 경로와 이름
		GENERIC_READ,			// 파일 접근 모드 (GENERIC_WRITE 파일 출력, GENERIC_READ 파일 입력)
		NULL,					// 공유방식, 파일이 열려 있는 상태에서 다른 프로그램에서 오픈할 때 허가 할 것인가(null인 경우 공유하지 않는다)
		NULL,					// 보안속성, null인 기본값으로 보안상태를 설정
		OPEN_EXISTING,			// 생성방식, 해당 파일을 열어서 작업할 것인지 새로 만들것인지 결정, CREATE_ALWAYS 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISITING 파일이 있을 경우에만 여는 옵션)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성지정(읽기 전용, 숨김 모드 등등) FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반적인 파일을 지정
		NULL);					// 생성된 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않을 것이기 때문에 null

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키코드 문자열을 유니코드로 전환시켜주는 매크로
		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
		return false;
	}

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		m_vecDeco.push_back(CAbstactFactory<CDeco>::Create(tInfo));
		Set_DecoSize(1);
		if (!dwByte)
			break;
	}
	dwByte = 0;
	CloseHandle(hFile);

	for_each(m_vecCol.begin(), m_vecCol.end(), CDeleteObj());
	m_vecCol.clear();
	m_vecCol.reserve(0xFF);
	m_iColSize = 0;

	hFile = CreateFile(L"../Data/Col2.dat",		// 저장할 파일의 경로와 이름
		GENERIC_READ,			// 파일 접근 모드 (GENERIC_WRITE 파일 출력, GENERIC_READ 파일 입력)
		NULL,					// 공유방식, 파일이 열려 있는 상태에서 다른 프로그램에서 오픈할 때 허가 할 것인가(null인 경우 공유하지 않는다)
		NULL,					// 보안속성, null인 기본값으로 보안상태를 설정
		OPEN_EXISTING,			// 생성방식, 해당 파일을 열어서 작업할 것인지 새로 만들것인지 결정, CREATE_ALWAYS 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISITING 파일이 있을 경우에만 여는 옵션)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성지정(읽기 전용, 숨김 모드 등등) FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반적인 파일을 지정
		NULL);					// 생성된 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않을 것이기 때문에 null

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키코드 문자열을 유니코드로 전환시켜주는 매크로
		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
		return false;
	}

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		m_vecCol.push_back(CAbstactFactory<CColisionTile>::Create(tInfo));
		Set_ColSize(1);
		if (!dwByte)
			break;
	}

	dwByte = 0;
	CloseHandle(hFile);

	for_each(m_vecLava.begin(), m_vecLava.end(), CDeleteObj());
	m_vecLava.clear();
	m_vecLava.reserve(0xFF);
	m_iLavaSize = 0;

	hFile = CreateFile(L"../Data/Lava2.dat",		// 저장할 파일의 경로와 이름
		GENERIC_READ,			// 파일 접근 모드 (GENERIC_WRITE 파일 출력, GENERIC_READ 파일 입력)
		NULL,					// 공유방식, 파일이 열려 있는 상태에서 다른 프로그램에서 오픈할 때 허가 할 것인가(null인 경우 공유하지 않는다)
		NULL,					// 보안속성, null인 기본값으로 보안상태를 설정
		OPEN_EXISTING,			// 생성방식, 해당 파일을 열어서 작업할 것인지 새로 만들것인지 결정, CREATE_ALWAYS 파일이 없다면 생성, 있다면 덮어쓰기, OPEN_EXISITING 파일이 있을 경우에만 여는 옵션)
		FILE_ATTRIBUTE_NORMAL,	// 파일 속성지정(읽기 전용, 숨김 모드 등등) FILE_ATTRIBUTE_NORMAL 아무런 속성이 없는 일반적인 파일을 지정
		NULL);					// 생성된 파일의 속성을 제공할 템플릿 파일, 우리는 사용하지 않을 것이기 때문에 null

	if (INVALID_HANDLE_VALUE == hFile)
	{
		// _T : 아스키코드 문자열을 유니코드로 전환시켜주는 매크로
		MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
		return false;
	}

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(INFO), &dwByte, nullptr);
		m_vecLava.push_back(CAbstactFactory<CLavaTile>::Create(tInfo));
		Set_LavalSize(1);
		if (!dwByte)
			break;
	}

	dwByte = 0;
	CloseHandle(hFile);
	return true;
}

void CTileMgr::ProcessEraseFireBall()
{
	for (auto& elem : *m_pObjMgr->Get_TargetList(MONSTER_KEY, WIZARD_BALL))
	{
		Safe_Delete(elem);
	}
}
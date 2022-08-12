#include "stdafx.h"
#include "LineMgr.h"

CLineMgr*		CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
{
}


CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize(void)
{
	Load_File();
}

void CLineMgr::Render(HDC _DC)
{
	for (auto& iter : m_LineList)
		iter->Render(_DC);
}

void CLineMgr::Release(void)
{
	for_each(m_LineList.begin(), m_LineList.end(), CDeleteObj());
	m_LineList.clear();
}

bool CLineMgr::Collision_Line(float& _fX, float* _fY)
{
	if (m_LineList.empty())
		return false;

	CLine*		pLine = nullptr;

	for (auto& iter : m_LineList)
	{
		if (_fX >= iter->Get_Info().tLPoint.fX &&
			_fX <= iter->Get_Info().tRPoint.fX)
		{
			pLine = iter;
		}
	}

	if (nullptr == pLine)
		return false;

	// Y = ((y2 - y1) / (x2 - x1)) * (X - x1) + y1

	float	x1 = pLine->Get_Info().tLPoint.fX;
	float	x2 = pLine->Get_Info().tRPoint.fX;

	float	y1 = pLine->Get_Info().tLPoint.fY;
	float	y2 = pLine->Get_Info().tRPoint.fY;
	
	*_fY = ((y2 - y1) / (x2 - x1)) * (_fX - x1) + y1;

	return true;
}

void CLineMgr::Load_File(void)
{
	HANDLE		hFile = CreateFile(L"../Data/Line.dat",		// 저장할 파일의 경로와 이름
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
		// 2인자 : 팝업 창에 띄우고자 하는 메세지
		// 3인자 : 팝업 창의 이름
	}

	// 파일 출력

	DWORD		dwByte = 0;
	LINE		tInfo{};

	while (true)
	{
		ReadFile(hFile, &tInfo, sizeof(LINE), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		m_LineList.push_back(new CLine(tInfo));
	}


	// 개방한 파일 소멸
	CloseHandle(hFile);
	MessageBox(g_hWnd, _T("Load 완료"), _T("성공"), MB_OK);
}


#include "stdafx.h"
#include "Include.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "AbstactFactory.h"
#include "TempFunctor.h"
#include "FileDefine.h"

class CFileMgr
{
public :
	void Add_FileInfo(const TCHAR* _path, const TCHAR* _key)
	{
		auto& iter = find_if(m_FormateMap.begin(), m_FormateMap.end(), CTagFinder(_key));

		if (iter == m_FormateMap.end())
		{
			HANDLE		hFile = CreateFile(_path,		// 저장할 파일의 경로와 이름
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

			DWORD				dwByte = 0;
			DWORD			fileCount = 0;
			buffer = new uint8_t[0xFFFF];
			FILEHEADER* pFileHeader = nullptr;
			INFOHEADER* pInFoHeader = nullptr;
			uint8_t		state = 0;

			int i = 0;
			while (true)
			{
				ReadFile(hFile, &buffer[i], sizeof(uint8_t), &dwByte, nullptr);
				++i;
				if (60 == i)
					break;
			}

			while (2 > state)
			{
				switch (state)
				{
				case 0:
					pFileHeader = (FILEHEADER*)&buffer[0];
					if (
						pFileHeader->fileMarker1 == 'B'
						&&pFileHeader->fileMarker2 == 'M'
						&&pFileHeader->unused1 == 0)
					{
						state = 1;
					}
					else
					{
						state = 2;
					}
					break;
				case 1:
				{
					pInFoHeader = (INFOHEADER*)&buffer[14];
					FILESIZE tFileSize{ pInFoHeader->width ,pInFoHeader->height };
					m_FormateMap.emplace(_key, tFileSize);
					state = 2;
				}
				break;
				}
			}

			if (!pInFoHeader)
				MessageBox(g_hWnd, L"텍스쳐 정보를 읽어오는데 실패", L"실패", MB_OK);

			Safe_Delete_Array(buffer);
			CloseHandle(hFile);
		}
	}

	const FILESIZE Get_Info(const TCHAR* _key)
	{
		FILESIZE tFileSize{};
		auto& elem = find_if(m_FormateMap.begin(), m_FormateMap.end(), CTagFinder(_key));
		if (elem != m_FormateMap.end())
			return elem->second;
		else
			return tFileSize;
	}
private :
	void Release()
	{

	}
private :
	map<const TCHAR*, FILESIZE> m_FormateMap;
private :
	CFileMgr() {}
	~CFileMgr()
	{
		Release();
	}
private :
	uint8_t *buffer;
public :
	static CFileMgr* m_pFileMgr;
public:
	static CFileMgr* Get_Instance()
	{
		if (!m_pFileMgr)
		{
			m_pFileMgr = new CFileMgr();
		}

		return m_pFileMgr;
	}
	static void Destroy_Instance()
	{
		delete m_pFileMgr;
		m_pFileMgr = nullptr;
	}
};
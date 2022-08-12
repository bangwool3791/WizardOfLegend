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
			HANDLE		hFile = CreateFile(_path,		// ������ ������ ��ο� �̸�
				GENERIC_READ,			// ���� ���� ��� (GENERIC_WRITE ���� ���, GENERIC_READ ���� �Է�)
				NULL,					// �������, ������ ���� �ִ� ���¿��� �ٸ� ���α׷����� ������ �� �㰡 �� ���ΰ�(null�� ��� �������� �ʴ´�)
				NULL,					// ���ȼӼ�, null�� �⺻������ ���Ȼ��¸� ����
				OPEN_EXISTING,			// �������, �ش� ������ ��� �۾��� ������ ���� ��������� ����, CREATE_ALWAYS ������ ���ٸ� ����, �ִٸ� �����, OPEN_EXISITING ������ ���� ��쿡�� ���� �ɼ�)
				FILE_ATTRIBUTE_NORMAL,	// ���� �Ӽ�����(�б� ����, ���� ��� ���) FILE_ATTRIBUTE_NORMAL �ƹ��� �Ӽ��� ���� �Ϲ����� ������ ����
				NULL);					// ������ ������ �Ӽ��� ������ ���ø� ����, �츮�� ������� ���� ���̱� ������ null

			if (INVALID_HANDLE_VALUE == hFile)
			{
				// _T : �ƽ�Ű�ڵ� ���ڿ��� �����ڵ�� ��ȯ�����ִ� ��ũ��
				MessageBox(g_hWnd, _T("Load File"), _T("Fail"), MB_OK);
				// 2���� : �˾� â�� ������ �ϴ� �޼���
				// 3���� : �˾� â�� �̸�
			}

			// ���� ���

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
				MessageBox(g_hWnd, L"�ؽ��� ������ �о���µ� ����", L"����", MB_OK);

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
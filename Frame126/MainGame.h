#pragma once

#include "Include.h"
#include "ObjMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

class CMainGame
{
public:
	CMainGame();
	~CMainGame();

public:
	void		Initialize(void);	// 데이터를 초기화하는 함수
	void		Update(void);		// 매 프레임마다 갱신하는 함수
	void		Late_Update(void);
	void		Render(void);		// 매 프레임마다 출력하는 함수
	void		Release(void);		// 객체 파괴 시 메모리 해제 함수
private :
	void		InitializeSound(void);
private:
	HDC			m_DC;
	CObjMgr*	m_pObjMgr;
private :
	void KeyInput();
private:
	#ifdef _DEBUG
	DWORD		m_dwTime;
	int			m_iFPS;
	TCHAR		m_szFPS[32];
	#endif

};

// 1. 작은 사각형 안에서 좌,우측 벽에 부딪히면 반대 방향으로 이동하는 몬스터 구현하기
// 2. 플레이어를 발사하는 미사일과 몬스터가 충돌할 경우 둘 다 제거하기

/*
힌트: IntersectRect(); // 사각형 충돌 함수
1. 렉트 주소 : 충돌이 발생했을 때는 생기는 렉트의 값을 저장
2. 렉트 주소 : 첫 번째 rect의 주소
3. 렉트 주소 : 두 번째 RECT의 주소*/

// 3. 피타고라스의 정리 증명(알아오라)
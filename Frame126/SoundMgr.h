#pragma once

#include "Enum.h"

class CSoundMgr
{
public:
	static CSoundMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CSoundMgr; 

		return m_pInstance; 
	}
	static void Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance; 
			m_pInstance = nullptr; 
		}
	}


private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize(); 
	void Update()
	{
		PlaySoundTimer();
		PlayBgmTimer();
	}
	void Release();
private :
	TCHAR * m_pSoundKey = nullptr;
	CHANNELID m_eID = MAXCHANNEL;
	float m_fVolume;

	TCHAR * m_pBgmKey = nullptr;
	float m_fBgmVolume;

public :
	void Set_Sound(TCHAR* pSoundKey, CHANNELID eID, float fVolume)
	{
		m_pSoundKey = pSoundKey;
		m_eID = eID;
		m_fVolume = fVolume;
		m_bSound = true;
	}
	void Set_Bgm(TCHAR* pSoundKey, float fVolume)
	{
		m_pBgmKey = pSoundKey;
		m_fBgmVolume = fVolume;
		m_bBgm = true;
	}
private :
	DWORD m_dSoundTick = 0;
	DWORD m_dBgmTick = 0;
	bool m_bSound = false;
	bool m_bBgm = false;
public:
	void PlaySoundTimer()
	{
		if (m_bSound)
		{
			if (0 == m_dSoundTick)

			{
				m_dSoundTick = GetTickCount();
				StopSound(m_eID);
			}
			else
			{
				if (m_dSoundTick + 75 < GetTickCount())
				{
					PlaySound(m_pSoundKey, m_eID, m_fVolume);
					m_bSound = false;
					m_dSoundTick = 0;
				}
			}
		}
	}
	void PlayBgmTimer()
	{
		if (m_bBgm)
		{
			if (0 == m_dBgmTick)

			{
				StopSound(SOUND_BGM);
				m_dBgmTick = GetTickCount();
			}
			else
			{
				if (m_dBgmTick + 50 < GetTickCount())
				{
					PlayBGM(m_pBgmKey, m_fBgmVolume);
					m_bBgm = false;
					m_dBgmTick = 0;
				}
			}
		}
	}
	void PlaySound(TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void PlayBGM(TCHAR* pSoundKey, float fVolume);
	void StopSound(CHANNELID eID);
	void StopAll();
	void SetChannelVolume(CHANNELID eID, float fVolume);

private:
	void LoadSoundFile(); 

private:
	static CSoundMgr* m_pInstance; 

	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL]; 

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 



};


#pragma once
#include "Define.h"

#define MAX_SFX_CHANNEL 31

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
	void Release(); 

public:
	int PlaySFX(const TCHAR* pSoundKey, const float& fVolume); // 빈 채널을 찾아서 사운드를 재생하고, 배치된 채널인덱스를 반환한다.
	void WaitPlaySFX(const TCHAR* pSoundKey, const float& fVolume, const int& _ChannelIndex); // 사운드가 끝날때까지 대기 후 출력
	void PlayBGM(const TCHAR* pSoundKey, const float& fVolume);

	void Stop_BGM();
	void Stop_SFX(const int& _ChannelIndex = -1); // 디폴트 매개변수가 불릴경우에는, 모든 SFX를 정지한다.

	void SetVolume(const SOUND_ID& _ID, const float& fVolume); // 아이디의 볼륨을 전체 조정 일단 보류

private:
	void LoadSoundFile(); 

private:
	static CSoundMgr* m_pInstance; 

	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound; 
	
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	array<FMOD_CHANNEL*, MAX_SFX_CHANNEL> m_arrSFXChannnel;
	FMOD_CHANNEL* m_pBGMChannel;

	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem; 
};


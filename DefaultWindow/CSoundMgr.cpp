#include "pch.h"
#include "CSoundMgr.h"

CSoundMgr* CSoundMgr::m_pInstance = nullptr;
CSoundMgr::CSoundMgr() : m_pBGMChannel(nullptr)
{
	m_pSystem = nullptr; 
}


CSoundMgr::~CSoundMgr()
{
	Release(); 
}

void CSoundMgr::Initialize()
{
	// 사운드를 담당하는 대표객체를 생성하는 함수
	FMOD_System_Create(&m_pSystem,131609U);
	
	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, 32, FMOD_INIT_NORMAL, NULL);

	LoadSoundFile(); 
}
void CSoundMgr::Release()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}
	m_mapSound.clear(); 

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

int CSoundMgr::PlaySFX(const TCHAR* pSoundKey, const float& fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(),
		[&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return -1; // 실패시 -1 반환

	FMOD_BOOL bPlay = FALSE;
	int i = 0; 
	for (; i < MAX_SFX_CHANNEL; i++) // MAX_SFX_CHANNEL만큼의 SFX를 동시 재생 가능
	{
		FMOD_Channel_IsPlaying(m_arrSFXChannnel[i], &bPlay); // 지금 채널이 비어있는지 확인
		if (!bPlay) // 비어있으면 여기서 실행
		{
			FMOD_System_PlaySound(m_pSystem, iter->second, 0, FALSE, &m_arrSFXChannnel[i]);
			FMOD_Channel_SetVolume(m_arrSFXChannnel[i], fVolume);
			break;;
		}
	}

	FMOD_System_Update(m_pSystem);
	return i; // 배치된 채널 인덱스를 반환한다.
}

void CSoundMgr::PlayBGM(const TCHAR* pSoundKey, const float& fVolume)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	// iter = find_if(m_mapSound.begin(), m_mapSound.end(), CTag_Finder(pSoundKey));
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)->bool
		{
			return !lstrcmp(pSoundKey, iter.first);
		});

	if (iter == m_mapSound.end())
		return;

	FMOD_System_PlaySound(m_pSystem, iter->second, 0, FALSE, &m_pBGMChannel);
	FMOD_Channel_SetMode(m_pBGMChannel, FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pBGMChannel, fVolume);
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::Stop_BGM()
{
	FMOD_Channel_Stop(m_pBGMChannel);
}

void CSoundMgr::Stop_SFX(const int& _ChannelIndex)
{
	if (_ChannelIndex < 0)
	{
		for (auto i : m_arrSFXChannnel)
		{
			FMOD_Channel_Stop(i);
		}
	}
	else
	{
		FMOD_Channel_Stop(m_arrSFXChannnel[_ChannelIndex]);
	}
}

void CSoundMgr::SetVolume(const SOUND_ID& _ID, const float& fVolume)
{
	//FMOD_Channel_SetVolume(m_pChannelArr[eID], fVolume);

	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::LoadSoundFile()
{
	// _finddata_t : <io.h>에서 제공하며 파일 정보를 저장하는 구조체
	_finddata_t fd = {0};

	// _findfirst : <io.h>에서 제공하며 사용자가 설정한 경로 내에서 가장 첫 번째 파일을 찾는 함수
	long long handle = _findfirst("../StarCraft/Sound/*.*", &fd);

	if (handle == -1)
		return; 

	int iResult = 0; 

	char szCurPath[128] = "../StarCraft/Sound/";	 // 상대 경로
	char szFullPath[128] = ""; 

	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath); 
		
		// "../Sound/" + "Success.wav"
		strcat_s(szFullPath, fd.name);
		// "../Sound/Success.wav"

		FMOD_SOUND* pSound = nullptr; 

		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);

		if (eRes == FMOD_OK)
		{
			int iLength = strlen(fd.name) + 1; 

			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);

			// 아스키 코드 문자열을 유니코드 문자열로 변환시켜주는 함수 
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);

			m_mapSound.emplace(pSoundKey, pSound);
		}
		//_findnext : <io.h>에서 제공하며 다음 위치의 파일을 찾는 함수, 더이상 없다면 -1을 리턴
		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);

	_findclose(handle);
}

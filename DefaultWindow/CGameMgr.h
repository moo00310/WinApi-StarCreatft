#pragma once
#include "Define.h"

class CGameMgr
{
public:
	CGameMgr(): m_iMineral(0), m_iGas(0), m_iPop(0), m_iMaxPop(0), m_Time(GetTickCount64())
	{
	}
	~CGameMgr() { Release(); }

	void		Initialize();
	void 		Update();
	void		Late_Update();
	void		Render(HDC hDC);
	void		Release();

public:
	// 요건 생성 시점에
	void Add_Mineral(int _num) { m_iMineral += _num; }
	void Add_Gas(int _num) { m_iGas += _num; }
	void Add_Pop(int _num) { m_iPop += _num; }

	// 건물은 Initalize에서
	void Add_MaxPop(int _num)
	{ 
		if(m_iMaxPop < 200)
			m_iMaxPop += _num;
		else
			m_iMaxPop = 200;
	}

	void AddTechCount(Tech _eTech , int count)
	{
		TechnicArray[_eTech] += count;
	}
	int GetTechCount(Tech _eTech)
	{
		return TechnicArray[_eTech];
	}

	bool isBuying(OBJ_TYPE _Type);
	

private:
	void TextPrint(HDC hdc);

public:
	static CGameMgr* Get_Instance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new CGameMgr;

		return m_pInstance;
	}

	static void		Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

	int m_iMineral;
	int m_iGas;
	int m_iPop;
	int m_iMaxPop;

	int TechnicArray[TECH_END] = { 0,0,0,0,0,0 };
	ULONGLONG m_Time;
	static CGameMgr* m_pInstance;
};


/// <summary>
/// 기초 - 커맨드 센터,가스, 서플라이, 배럭, 아카데미/ 마린, SCV
/// (배럭) -> 팩토리, 아카데미
/// (아카데미) -> 메딕
/// (팩토리) -> 스타포트, 아머리
/// (스타포트) ->  사이언스 퍼실리티
/// 사이언스 퍼실리티 코버드옵스-> 고스트, 뉴클리어 
/// </summary>

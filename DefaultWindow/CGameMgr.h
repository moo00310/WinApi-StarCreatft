#pragma once
#include "Define.h"

class CGameMgr
{
public:
	CGameMgr(): m_iMineral(0), m_iGas(0), m_iPop(0), m_iMaxPop(0), m_Time(GetTickCount64()), m_isDeBug(false)
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


public:
	bool Get_UpGrade_Compelate(Upgrade _type) { return UpGrade_Compelate[_type]; }
	void Set_UpGrade_Compelate(Upgrade _type, bool _bool) { UpGrade_Compelate[_type] = _bool; }

public:
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

	int TechnicArray[TECH_END] = { 0 };
	bool UpGrade_Compelate[UG_END] = { false };
	ULONGLONG m_Time;
	static CGameMgr* m_pInstance;


	bool m_isDeBug;
};


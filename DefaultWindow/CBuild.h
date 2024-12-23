#pragma once
#include "CObj.h"

class CBuild : public CObj
{
public:
	CBuild() :m_ePreState_Bulid(BS_END), m_eCurState_Build(BS_END), m_iMyBuildTIme(0), m_iBuildCount(0), m_bTemplate(false),
		m_iTemplateSize(0), m_MaxSpwanTime(0), m_iSpwanTime(0), m_fProgress(0.f)
	{
	}
	~CBuild() {}

	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;
	virtual void KeyInput() PURE;

public:
	list<OBJ_TYPE>* Get_SpawnList() { return &m_listSpawn; }
	BuildSTATE Get_State() { return m_eCurState_Build; }
	float GetProgress() { return m_fProgress; }


protected:
	void Spawn_Uint_CoolDown();
	void Spawn_Uint_Index(OBJ_TYPE _id);

protected:
	BuildSTATE m_ePreState_Bulid;
	BuildSTATE m_eCurState_Build;

	int m_iMyBuildTIme;
	int m_iBuildCount;
	bool m_bTemplate;
	int m_iTemplateSize;

	int m_MaxSpwanTime;
	int m_iSpwanTime;

	list<OBJ_TYPE> m_listSpawn;

	float m_fProgress;
};


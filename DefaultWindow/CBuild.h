#pragma once
#include "CObj.h"

class CBuild : public CObj
{
public:
	CBuild() :m_ePreState_Bulid(BS_END), m_eCurState_Build(BS_END), m_iMyBuildTIme(0), m_iBuildCount(0), m_bTemplate(false),
		m_iTemplateSize(0)
	{
	}
	~CBuild() {}

	virtual void Initialize() PURE;
	virtual int Update() PURE;
	virtual void Late_Update() PURE;
	virtual void Render(HDC hDC) PURE;
	virtual void Release() PURE;

protected:
	BuildSTATE m_ePreState_Bulid;
	BuildSTATE m_eCurState_Build;

	int m_iMyBuildTIme;
	int m_iBuildCount;
	bool m_bTemplate;
	int m_iTemplateSize;

};


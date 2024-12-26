#pragma once
#include "CObj.h"
class CEffect : public CObj
{
public:
	CEffect() : m_iCount(0), m_iDeadImg(0) {}
	~CEffect() {}
	void Initialize() PURE;
	int Update() PURE;
	void Late_Update() PURE;
	void Render(HDC hDC) PURE;
	void Release() PURE;
	void Move_Frame() override;

protected:
	int m_iCount;
	int m_iDeadImg;
	
};
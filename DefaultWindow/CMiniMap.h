#pragma once
#include "CUI.h"
#include "CObj.h"
class CMiniMap : public CUI
{
public:
	CMiniMap() : m_pPlayer(nullptr), m_pBuild(nullptr), m_pEnemy(nullptr), m_pBuild_E(nullptr),
		hOldBrush(nullptr) {}
	~CMiniMap() {}
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hdc) override;
	void Release() override;

private:
	void CreateRect(HDC hdc, float _x, float _y, int size, bool isBuild);

	list <CObj*>* m_pPlayer;
	list <CObj*>* m_pBuild;
	list <CObj*>* m_pEnemy;
	list <CObj*>* m_pBuild_E;

	HBRUSH hOldBrush;
	HPEN hOldPen;
};


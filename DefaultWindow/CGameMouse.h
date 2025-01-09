#pragma once
#include "CMouse.h"
#include "CUnit.h"
#include "CObj.h"

class CGameMouse : public CMouse
{
public:
	CGameMouse();
	~CGameMouse();
public:
	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;

public:
	void SetBuild_Img(OBJ_TYPE _type) { m_eBuildType = _type; isBuildMod = true; }

public:
	// 핵 모드
	void SetNukeMod() { NukeMode = true; }
	
	// 야마토 모드
	void SetYamatoMode() { YamatoMode = true; }

	void ClearList();
	void ClearDrag();

private:
	// 일반 모드
	void MouseInput(POINT ptMouse);
	void ScrollMove(POINT mouse);
	void ColObject();
	void Change_Cursor();
	void ColDrag();
	void MoveSound(OBJ_TYPE type);
	void SelectSound(CObj* _obj);

	//빌드 모드
	void DrawBulid();
	bool AbleBuild();

private:
	POINT ptMouse{};
	CURSERSTATE m_eCurState;
	CURSERSTATE m_ePreState;
	int m_indexY;
	list<CObj*>* m_UnitList;
	list<CObj*>* m_BuildList;
	list<CObj*>* m_UnitList_E;
	list<CObj*>* m_BuildList_E;
	list<CObj*>* m_Select_UnitList;

	bool isDrag;
	POINT	m_DragStart;
	POINT	m_DragEnd;

	bool isBuildMod;
	OBJ_TYPE m_eBuildType;
	const TCHAR* m_pImgKey_build;

	int m_iBuild_Index;
	COLOR m_tColor;

	bool NukeMode;
	bool YamatoMode;
};


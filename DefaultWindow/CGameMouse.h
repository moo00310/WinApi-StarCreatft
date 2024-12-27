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
	void ClearList();
	void ClearDrag();

private:
	void Initailize_Img();

	// 일반 모드
	void MouseInput(POINT ptMouse);
	void ScrollMove(POINT mouse);
	void ColObject();
	void Change_Cursor();
	void ColDrag();

	//빌드 모드
	void DrawBulid();
	bool AbleBuild();
	void SpwanBulid(OBJ_TYPE _type, Pos temp);

private:
	POINT ptMouse{};
	CURSERSTATE m_eCurState;
	CURSERSTATE m_ePreState;
	int m_indexY;
	list<CObj*>* m_UnitList;
	list<CObj*>* m_BuildList;
	list<CObj*>* m_Select_UnitList;

	bool isDrag;
	POINT	m_DragStart;
	POINT	m_DragEnd;

	bool isBuildMod;
	OBJ_TYPE m_eBuildType;
	const TCHAR* m_pImgKey_build;

	int m_iBuild_Index;
	COLOR m_tColor;

};


#pragma once
#include "CUnit.h"
#include "Define.h"

class CMarine: public CUnit
{
public:
	CMarine();
	~CMarine();

	void Initialize() override;
	int Update() override;
	void Late_Update() override;
	void Render(HDC hDC) override;
	void Release() override;
private:
	void Change_Motion();
	void Test_Key_Input();


public:
	// 유닛 클래스로 올릴겨
	void Astar(Pos _tTarget_Index);
	bool CanGo(Pos pos);
	void MoveTo();

private:
	int m_iImgId;
	ATIMESTATE m_ePreState;
	ATIMESTATE m_eCurState;

	ULONGLONG m_dwTime;

	//유닛 클래스로 올릴겨
	vector<vector<int>>* m_Map;
	vector<Pos>	_path;
	int m_iPathIndex;
};


#pragma once

#include "CScene.h"
#include "CObj.h"

class CMyEdit :   public CScene
{
public:
	CMyEdit();
	virtual ~CMyEdit();

public:
	void Initialize()		override;
	int  Update()			override;
	void Late_Update()		override;
	void Render(HDC hDC)	override;
	void Release()			override;

private:
	void	Key_Input();
	void	ChangeTileID(TILE_GROUP eGroup);

private:
	int		m_TileID;
	int		m_TileOption;

	CObj* m_pMouse;
	EditType m_eEditType;
};


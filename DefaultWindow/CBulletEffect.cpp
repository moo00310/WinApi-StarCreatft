#include "pch.h"
#include "CBulletEffect.h"
#include "CBmpMgr.h"
#include "CCollisionMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CSoundMgr.h"

/*-------------------
*  SCV 공격
----------------------*/
void CScvHit::Initialize()
{
	m_pImgKey = L"SCVEffect";
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 48.f;

	m_eRender = RENDER_HIT_EFFECT;
}

int CScvHit::Update()
{
	m_iCount++;
	//테스트s
	if (m_iCount < 5)
	{
		m_iDeadImg = 0;
	}
	else if (m_iCount < 10)
	{
		m_iDeadImg = 1;
	}
	else if (m_iCount < 15)
	{
		m_iDeadImg = 2;
	}
	else if (m_iCount < 20)
	{
		m_iDeadImg = 3;
	}
	else if (m_iCount < 25)
	{
		m_iDeadImg = 4;
	}
	else if (m_iCount < 30)
	{
		m_iDeadImg = 5;
	}
	else if (m_iCount < 35)
	{
		m_iDeadImg = 6;
	}
	else if (m_iCount < 40)
	{
		m_iDeadImg = 7;
	}
	else if (m_iCount < 45)
	{
		m_iDeadImg = 8;
	}
	else if (m_iCount < 50)
	{
		m_iDeadImg = 9;
	}
	else if (m_iCount < 55)
	{
		m_iDeadImg = 10;
	}
	else if (m_iCount > 55)
	{
		return OBJ_DEAD;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CScvHit::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		0, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * m_iDeadImg,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));		// 제거할 색상
}

/*-------------------
*  마린 공격
----------------------*/

void CMarineHit::Initialize()
{
	m_pImgKey = L"MarineGunSpark";
	m_tInfo.fCX = 40.f;
	m_tInfo.fCY = 40.f;

	m_eRender = RENDER_HIT_EFFECT;
}

int CMarineHit::Update()
{
	m_iCount++;
	//테스트s
	if (m_iCount < 5)
	{
		m_iDeadImg = 0;
	}
	else if (m_iCount < 10)
	{
		m_iDeadImg = 1;
	}
	else if (m_iCount < 15)
	{
		m_iDeadImg = 2;
	}
	else if (m_iCount < 20)
	{
		m_iDeadImg = 3;
	}
	else if (m_iCount < 25)
	{
		m_iDeadImg = 4;
	}
	else if (m_iCount < 30)
	{
		m_iDeadImg = 5;
	}
	else if (m_iCount < 35)
	{
		m_iDeadImg = 6;
	}
	else if (m_iCount < 40)
	{
		m_iDeadImg = 7;
	}
	else if (m_iCount < 45)
	{
		m_iDeadImg = 8;
	}
	else if (m_iCount < 50)
	{
		m_iDeadImg = 9;
	}
	else if (m_iCount < 55)
	{
		m_iDeadImg = 10;
	}
	else if (m_iCount < 60)
	{
		m_iDeadImg = 11;
	}
	else if (m_iCount < 65)
	{
		m_iDeadImg = 12;
	}
	else if (m_iCount < 70)
	{
		m_iDeadImg = 13;
	}
	else if (m_iCount < 75)
	{
		m_iDeadImg = 13;
	}
	else if (m_iCount > 80)
	{
		return OBJ_DEAD;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CMarineHit::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		0, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * m_iDeadImg,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(255, 0, 255));		// 제거할 색상
}

/*-------------------
*  고스트 공격
----------------------*/

void CGhostHit::Initialize()
{
	
	m_pImgKey = L"GhostShot";
	m_tInfo.fCX = 20.f;
	m_tInfo.fCY = 20.f;

	m_eRender = RENDER_HIT_EFFECT;
}

int CGhostHit::Update()
{
	m_iCount++;
	//테스트s
	if (m_iCount < 5)
	{
		m_iDeadImg = 0;
	}
	else if (m_iCount < 10)
	{
		m_iDeadImg = 1;
	}
	else if (m_iCount < 15)
	{
		m_iDeadImg = 2;
	}
	else if (m_iCount < 20)
	{
		m_iDeadImg = 3;
	}
	else if (m_iCount < 25)
	{
		m_iDeadImg = 4;
	}
	else if (m_iCount < 30)
	{
		m_iDeadImg = 5;
	}
	else if (m_iCount < 35)
	{
		m_iDeadImg = 6;
	}
	else if (m_iCount < 40)
	{
		m_iDeadImg = 7;
	}
	else if (m_iCount < 45)
	{
		m_iDeadImg = 8;
	}
	else if (m_iCount < 50)
	{
		m_iDeadImg = 9;
	}
	else if (m_iCount < 55)
	{
		m_iDeadImg = 10;
	}
	else if (m_iCount > 60)
	{
		return OBJ_DEAD;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGhostHit::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		0, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * m_iDeadImg,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 255, 0));		// 제거할 색상
}



/*-------------------
*  탱크 퉁퉁포 공격
----------------------*/

void CTankHit::Initialize()
{
	m_pImgKey = L"TankHit";
	m_tInfo.fCX = 56.f;
	m_tInfo.fCY = 56.f;

	m_eRender = RENDER_HIT_EFFECT;
}

int CTankHit::Update()
{
	m_iCount++;
	//테스트s
	if (m_iCount < 5)
	{
		m_iDeadImg = 0;
	}
	else if (m_iCount < 10)
	{
		m_iDeadImg = 1;
	}
	else if (m_iCount < 15)
	{
		m_iDeadImg = 2;
	}
	else if (m_iCount < 20)
	{
		m_iDeadImg = 3;
	}
	else if (m_iCount < 25)
	{
		m_iDeadImg = 4;
	}
	else if (m_iCount < 30)
	{
		m_iDeadImg = 5;
	}
	else if (m_iCount < 35)
	{
		m_iDeadImg = 6;
	}
	else if (m_iCount < 40)
	{
		m_iDeadImg = 7;
	}
	else if (m_iCount < 45)
	{
		m_iDeadImg = 8;
	}
	else if (m_iCount > 100)
	{
		return OBJ_DEAD;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CTankHit::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		0 , // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * m_iDeadImg,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 255, 0));		// 제거할 색상
}

/*-------------------
*  시즈탱크 공격
----------------------*/

void CSiegeTankHit::Initialize()
{
	E_list = CObjMgr::Get_Instance()->Get_ObjList(OBJ_MONSTER);
	E_build_list = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD_E);
	m_pImgKey = L"SiegeTankHit";
	m_tInfo.fCX = 100.f;
	m_tInfo.fCY = 108.f;

	m_eRender = RENDER_HIT_EFFECT;
}

int CSiegeTankHit::Update()
{
	m_iCount++;
	//테스트
	if (m_iCount < 5)
	{
		m_iDeadImg = 0;
	}
	else if (m_iCount < 10)
	{
		m_iDeadImg = 1;
	}
	else if (m_iCount < 15)
	{
		m_iDeadImg = 2;
	}
	else if (m_iCount < 20)
	{
		m_iDeadImg = 3;
	}
	else if (m_iCount < 25)
	{
		m_iDeadImg = 4;
	}
	else if (m_iCount < 30)
	{
		m_iDeadImg = 5;
	}
	else if (m_iCount < 35)
	{
		m_iDeadImg = 6;
		if (!m_bIsDamage)
		{
			CCollisionMgr::Collision_Explosion(this, *E_list, *E_build_list, 50);
			m_bIsDamage = true;
		}
	}
	else if (m_iCount < 40)
	{
		m_iDeadImg = 7;
	}
	else if (m_iCount < 45)
	{
		m_iDeadImg = 8;
	}
	else if (m_iCount < 50)
	{
		m_iDeadImg = 9;
	}
	else if (m_iCount < 55)
	{
		m_iDeadImg = 10;
	}
	else if (m_iCount < 60)
	{
		m_iDeadImg = 11;
	}
	else if (m_iCount < 65)
	{
		m_iDeadImg = 12;
	}
	else if (m_iCount < 70)
	{
		m_iDeadImg = 13;
	}
	else if (m_iCount > 100)
	{
		return OBJ_DEAD;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CSiegeTankHit::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_iDeadImg, // 비트맵 출력 시작 좌표(Left, top)
		0,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 0, 0));		// 제거할 색상
}

/*-------------------
*  핵 미사일
----------------------*/

void CNukeMissile::Initialize()
{
	m_pImgKey = L"NukeMissile";
	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 40.f;

	m_eRender = RENDER_GAMEOBJECT;
	BoomY = (int)m_tInfo.fY;
	m_tInfo.fY -= 700.f;
	m_iDeadImg = 1;
}

int CNukeMissile::Update()
{
	if (m_iCount > 250)
	{
		m_tInfo.fY += 3.f;
	}
	if (m_tInfo.fY > BoomY + 25.f)
	{
		// 펑
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CNukeMissileBoom>::CreateFX(m_tInfo.fX, m_tInfo.fY));
		return OBJ_DEAD;
	}

	m_iCount++;
	__super::Update_Rect();
	return 0;
}

void CNukeMissile::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_iDeadImg, // 비트맵 출력 시작 좌표(Left, top)
		0,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 0, 0));		// 제거할 색상

	//Rectangle(hDC, m_tRect.left, m_tRect.top, m_tRect.right, m_tRect.bottom);
}


/*-------------------
*  핵 폭팔
----------------------*/

void CNukeMissileBoom::Initialize()
{
	E_list = CObjMgr::Get_Instance()->Get_ObjList(OBJ_MONSTER);
	E_build_list = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD_E);

	m_pImgKey = L"NukeBoom";
	m_tInfo.fCX = 252.f;
	m_tInfo.fCY = 252.f;

	m_eRender = RENDER_GAMEOBJECT;
}

int CNukeMissileBoom::Update()
{
	m_iCount++;
	//테스트
	if (m_iCount < 5)
	{
		m_iDeadImg = 0;
	}
	else if (m_iCount < 10)
	{
		m_iDeadImg = 1;
		if (!m_bIsDamage)
		{
			CCollisionMgr::Collision_Explosion(this, *E_list, *E_build_list, 300);
			CSoundMgr::Get_Instance()->PlaySFX(L"GhostNukeEnd.mp3", 1.0f);
			m_bIsDamage = true;
		}
	}
	else if (m_iCount < 15)
	{
		m_iDeadImg = 2;
	}
	else if (m_iCount < 20)
	{
		m_iDeadImg = 3;	
	}
	else if (m_iCount < 25)
	{
		m_iDeadImg = 4;
	}
	else if (m_iCount < 30)
	{
		m_iDeadImg = 5;
	}
	else if (m_iCount < 35)
	{
		m_iDeadImg = 6;
	}
	else if (m_iCount < 40)
	{
		m_iDeadImg = 7;
	}
	else if (m_iCount < 45)
	{
		m_iDeadImg = 8;
	}
	else if (m_iCount < 50)
	{
		m_iDeadImg = 9;
	}
	else if (m_iCount < 55)
	{
		m_iDeadImg = 10;
	}
	else if (m_iCount < 60)
	{
		m_iDeadImg = 11;
	}
	else if (m_iCount < 66)
	{
		m_iDeadImg = 12;
	}
	else if (m_iCount < 71)
	{
		m_iDeadImg = 13;
	}
	else if (m_iCount < 78)
	{
		m_iDeadImg = 14;
	}
	else if (m_iCount < 83)
	{
		m_iDeadImg = 15;
	}
	else if (m_iCount < 90)
	{
		m_iDeadImg = 16;
	}
	else if (m_iCount < 95)
	{
		m_iDeadImg = 17;
	}
	else if (m_iCount < 100)
	{
		m_iDeadImg = 18;
	}
	else if (m_iCount < 110)
	{
		m_iDeadImg = 19;
	}
	else if (m_iCount < 120)
	{
		m_iDeadImg = 20;
	}
	else if (m_iCount < 130)
	{
		m_iDeadImg = 21;
	}
	else if (m_iCount < 140)
	{
		m_iDeadImg = 22;
	}
	else if (m_iCount < 150)
	{
		m_iDeadImg = 23;
	}
	else if (m_iCount < 160)
	{
		m_iDeadImg = 24;
		}
	else if (m_iCount < 170)
	{
		m_iDeadImg = 25;
		}
	else if (m_iCount > 180)
	{
		return OBJ_DEAD;
	}

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CNukeMissileBoom::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_iDeadImg, // 비트맵 출력 시작 좌표(Left, top)
		0,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 0, 0));		// 제거할 색상
}

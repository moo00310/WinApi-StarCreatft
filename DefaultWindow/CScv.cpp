#include "pch.h"
#include "CSCV.h"
#include "CMapMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"
#include "CBulletEffect.h"
#include "CKeyMgr.h"
#include "CMouseMgr.h"

void CScv::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	// 리스트 할당
	m_pMonsterList = CObjMgr::Get_Instance()->Get_MonsterList();
	m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/SCV/SCV.bmp", L"Scv");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_1(40.40).bmp", L"Select_1");


	m_pImgKey = L"Scv";
	m_eObjID = OT_Scv;
	m_tStat = { 60.f, 60.f, 5, 0, 32, 2.3f, 625 , DF_SAMLL, AT_NORMAL };

	m_iAttackFrame = 2;

	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 72.f;
	m_tInfo.fCY = 72.f;

}

int CScv::Update()
{
	if (m_bDead || m_tStat.m_iHp <= 0)
	{
		// 죽음 이펙트
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CSCVDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));
		//CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
		//CSoundMgr::Get_Instance()->PlaySound(L"Marine_Dead_1.mp3", SOUND_EFFECT, 0.5f, true);

		return OBJ_DEAD;
	}
	Update_State();
	KeyInput();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CScv::Late_Update()
{
	Change_Motion();
	CUnit::Move_Frame();
}

void CScv::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_1");

	if (m_bSelect)
	{
		GdiTransparentBlt(hDC,			// 복사 받을 DC
			m_tRect.left + iScrollX + 17,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + iScrollY + 20,
			40,			// 복사 받을 이미지의 가로, 세로
			40,
			hFxDC,						// 복사할 이미지 DC	
			0, // 비트맵 출력 시작 좌표(Left, top)
			0,
			40,										// 복사할 이미지의 가로, 세로
			40,
			RGB(255, 0, 255));		// 제거할 색상
	}

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hMemDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_tFrame.iCurCount, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * (int)m_eDir,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 255, 0));		// 제거할 색상


}

void CScv::Release()
{
}

void CScv::Change_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE_IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_MOVE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_ATTACK:
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iCurCount = 1;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_SHOOT:
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iCurCount = 1;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
		}

		m_ePreState = m_eCurState;
	}

}

void CScv::KeyInput()
{
	if (!m_bSelect) return;

	if (m_bBuildStructure == false && m_BuildAdvancedStructure == false)
	{
		if (CKeyMgr::Get_Instance()->Key_Down('B'))
		{
			m_bBuildStructure = true;
		}

		if (CKeyMgr::Get_Instance()->Key_Down('V'))
		{
			m_BuildAdvancedStructure = true;
		}
	}
	

	if (m_bBuildStructure == true)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
		{
			m_bBuildStructure = false;
			m_BuildAdvancedStructure = false;
		}

		if(CKeyMgr::Get_Instance()->Key_Down('C'))
		{
			CMouseMgr::Get_Instance()->Get_Mouse()->SetBuild_Img(OT_Commend);
			m_bBuildStructure = false;
		}

		if (CKeyMgr::Get_Instance()->Key_Down('S'))
		{
			CMouseMgr::Get_Instance()->Get_Mouse()->SetBuild_Img(OT_Suffly);
			m_bBuildStructure = false;
		}

		if (CKeyMgr::Get_Instance()->Key_Down('R'))
		{
			CMouseMgr::Get_Instance()->Get_Mouse()->SetBuild_Img(OT_Refinery);
			m_bBuildStructure = false;
		}

		if (CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			CMouseMgr::Get_Instance()->Get_Mouse()->SetBuild_Img(OT_Academy);
			m_bBuildStructure = false;
		}

		if (CKeyMgr::Get_Instance()->Key_Down('B'))
		{
			CMouseMgr::Get_Instance()->Get_Mouse()->SetBuild_Img(OT_Barrck);
			m_bBuildStructure = false;
		}
	}

	if (m_BuildAdvancedStructure == true)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_RBUTTON))
		{
			m_bBuildStructure = false;
			m_BuildAdvancedStructure = false;
		}

		if (CKeyMgr::Get_Instance()->Key_Down('F'))
		{
			CMouseMgr::Get_Instance()->Get_Mouse()->SetBuild_Img(OT_Factory);
			m_BuildAdvancedStructure = false;
		}

		if (CKeyMgr::Get_Instance()->Key_Down('S'))
		{
			CMouseMgr::Get_Instance()->Get_Mouse()->SetBuild_Img(OT_Starport);
			m_BuildAdvancedStructure = false;
		}

		if (CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			CMouseMgr::Get_Instance()->Get_Mouse()->SetBuild_Img(OT_Armory);
			m_BuildAdvancedStructure = false;
		}

		if (CKeyMgr::Get_Instance()->Key_Down('I'))
		{
			CMouseMgr::Get_Instance()->Get_Mouse()->SetBuild_Img(OT_ScienceFacility);
			m_BuildAdvancedStructure = false;
		}
	}

}

void CScv::AttackToEnemy(CObj* _Enemey)
{
	if (m_AttackTime + _Enemey->Get_Stat()->Colldown < GetTickCount64() &&
		m_tFrame.iCurCount == m_iAttackFrame)
	{
		DEFENCEID Dfence_id = _Enemey->Get_Stat()->m_eDfenceID;
		ATTACKID Attack_id = m_tStat.m_eAttackID;
		float Damge = fabsf((_Enemey->Get_Stat()->m_iDefence) - ((DamageCalcu[Attack_id][Dfence_id] * m_tStat.m_iAttack)));


		_Enemey->Add_Stat_hp(-Damge);

		m_AttackTime = GetTickCount64();
	}
}

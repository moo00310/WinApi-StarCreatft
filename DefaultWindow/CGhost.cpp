#include "pch.h"
#include "CGhost.h"
#include "CMapMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"
#include "CBulletEffect.h"
#include "CKeyMgr.h"
#include "CGameMgr.h"
#include "CMouseMgr.h"

void CGhost::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	// 리스트 할당
	m_pMonsterList = CObjMgr::Get_Instance()->Get_MonsterList();
	m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);
	m_pBuildList_E = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD_E);

	m_pImgKey = L"Ghost";
	m_eObjID = OT_Ghost;
	m_tStat = { 45.f, 45.f, 10, 0, 224, 1.8f, 625 , DF_SAMLL, AT_CONCUSSIVE };

	m_iAttackFrame = 12;
	m_fCloack = 1.0f;

	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;

	A_GroundPos.x = (int)m_tInfo.fX / 32;
	A_GroundPos.y = (int)m_tInfo.fY / 32;

	m_Ghost = new Gdiplus::Image(L"../StarCraft/Unit/Ghost/Ghost.bmp");

	Gdiplus::Color ExceptColor(255, 0, 255, 0);
	m_imgAttr.SetColorKey(ExceptColor, ExceptColor, Gdiplus::ColorAdjustTypeBitmap);
	
}

int CGhost::Update()
{
	if (m_bDead || m_tStat.m_iHp <= 0)
	{
		// 죽음 이펙트
		CSoundMgr::Get_Instance()->PlaySFX(L"Ghost_Dead .mp3", 0.3f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CGhostDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));
		return OBJ_DEAD;
	}

	KeyInput();
	Update_State();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CGhost::Late_Update()
{
	Change_Motion();
	CUnit::Move_Frame();

	if (AttackCoolTime + 1700 < GetTickCount64())
	{
		m_isAttack = false;
		AttackCoolTime = GetTickCount64();
	}
}

void CGhost::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_0");

	if (m_bSelect)
	{
		GdiTransparentBlt(hDC,			// 복사 받을 DC
			m_tRect.left + iScrollX + 15,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + iScrollY + 25,
			32,			// 복사 받을 이미지의 가로, 세로
			32,
			hFxDC,						// 복사할 이미지 DC	
			0, // 비트맵 출력 시작 좌표(Left, top)
			0,
			32,										// 복사할 이미지의 가로, 세로
			32,
			RGB(255, 0, 255));		// 제거할 색상
	}

	using namespace Gdiplus;
	Graphics graphics(hDC);

	m_ColorMatrix = {
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // R
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // G
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // B
	0.0f, 0.0f, 0.0f, m_fCloack, 0.0f, // A (알파값 0.5로 설정)
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f // 여기 값 바꾸면 프레임 박살남
	};

	m_imgAttr.SetColorMatrix(&m_ColorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	graphics.DrawImage(
		m_Ghost,            // 화면에 랜더링할 이미지 데이터
		Rect(            // 이미지 출력 위치와 크기를 저장하는 클래스
			m_tRect.left + iScrollX,
			m_tRect.top + iScrollY,
			(int)m_tInfo.fCX,        // 렌더링 받을 화면의 가로, 세로 범위
			(int)m_tInfo.fCY
		),
		(int)m_tInfo.fCX * m_tFrame.iCurCount,  // 원본 이미지에서 렌더링 시작할 좌표 X,Y
		(int)m_tInfo.fCY * (int)m_eDir,
		(int)m_tInfo.fCX,            // 원본 이미지에서 렌더링 할 가로, 세로 길이
		(int)m_tInfo.fCY,
		UnitPixel,        // 좌표의 단위를 무엇으로 할지 (픽셀단위 임으로 UnitPixel)
		&m_imgAttr        // 컬러 매트릭스를 포함하고 있는 ImageAttributes 객체의 주소
	);

}

void CGhost::Release()
{
}

void CGhost::Change_Motion()
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
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iCurCount = 1;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_ATTACK:
			m_tFrame.iFrameStart = 10;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iCurCount = 10;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_SHOOT:
			m_tFrame.iFrameStart = 11;
			m_tFrame.iFrameEnd = 12;
			m_tFrame.iCurCount = 11;
			m_tFrame.dwSpeed = 500;
			m_tFrame.dwTime = GetTickCount64();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CGhost::KeyInput()
{
	if (!m_bSelect) return;

	if (CKeyMgr::Get_Instance()->Key_Down('N'))
	{
		if (!CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Cmd_Nuke)) return;

		CMouseMgr::Get_Instance()->Get_Mouse()->SetNukeMod();
	}


	if (CKeyMgr::Get_Instance()->Key_Down('C'))
	{
		if (!CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Ghost_Cloak)) return;

		if (!m_bIsCloack)
		{
			m_fCloack = 0.3f;
			m_bIsCloack = true;
		}
		else
		{
			m_fCloack = 1.0f;
			m_bIsCloack = false;
		}
			
	}
}

void CGhost::AttackToEnemy(CObj* _Enemey)
{
	m_eDir = GetDirection(m_tInfo.fX, m_tInfo.fY, _Enemey->Get_Info().fX, _Enemey->Get_Info().fY);
	if (m_eCurState == STATE_SHOOT)
		m_eCurState = STATE_SHOOT;
	else
		m_eCurState = STATE_ATTACK;

	if (m_AttackTime + _Enemey->Get_Stat()->Colldown < GetTickCount64() &&
		m_tFrame.iCurCount == m_iAttackFrame && !m_isAttack)
	{
		DEFENCEID Dfence_id = _Enemey->Get_Stat()->m_eDfenceID;
		ATTACKID Attack_id = m_tStat.m_eAttackID;
		float Damge = fabsf((_Enemey->Get_Stat()->m_iDefence) - ((DamageCalcu[Attack_id][Dfence_id] * m_tStat.m_iAttack)));

		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CGhostHit>::CreateFX(_Enemey->Get_Info().fX, _Enemey->Get_Info().fY));
		CSoundMgr::Get_Instance()->PlaySFX(L"GhostAttack.mp3", 0.8f);
		_Enemey->Add_Stat_hp(-Damge);
		m_isAttack = true;

		m_AttackTime = GetTickCount64();
	}
}

void CGhost::Update_State()
{
	switch (m_eInput)
	{
	case IP_MOVE:
		Move();
		break;
	case IP_ATTACK:
		Attack();
		break;
	case IP_HOLD:
		Hold();
		break;
	case IP_STOP:
		Stop();
		break;
	case IP_NUKE:
		Nuke();
		break;
	case IP_Chase:
		ChaseUnit();
		break;
	case IP_END:
		break;
	default:
		break;
	}
}

void CGhost::Nuke()
{
	if (m_iPathIndex < _path.size())
	{
		Move_toNext();
	}
	else if (m_iPathIndex == _path.size())
	{
		m_eCurState = STATE_ATTACK;	

		if (CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Cmd_Nuke))
		{
			m_eDir = GetDirection(m_tInfo.fX, m_tInfo.fY, m_Nuketarget.x * 32.f, m_Nuketarget.y * 32.f);
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CNukeMissile>::CreateFX(m_Nuketarget.x * 32.f, m_Nuketarget.y * 32.f));
			CSoundMgr::Get_Instance()->PlaySFX(L"GhostNukeStart.mp3", 0.8f);
			CGameMgr::Get_Instance()->Set_UpGrade_Compelate(UG_Cmd_Nuke, false);
		}
	}
}

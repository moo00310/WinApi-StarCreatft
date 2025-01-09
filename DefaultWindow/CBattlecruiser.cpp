#include "pch.h"
#include "CBattlecruiser.h"
#include "CObjMgr.h"
#include "CMapMgr.h"
#include "CSoundMgr.h"
#include "CBmpMgr.h"
#include "CCollisionMgr.h"
#include "CBulletEffect.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"
#include "CKeyMgr.h"
#include "CGameMgr.h"
#include "CMouseMgr.h"

void CBattlecruiser::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	// 리스트 할당
	m_pMonsterList = CObjMgr::Get_Instance()->Get_MonsterList();
	m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);
	m_pBuildList_E = CObjMgr::Get_Instance()->Get_ObjList(OBJ_BUILD_E);

	m_pImgKey = L"Battlecruiser";
	m_eObjID = OT_Battlecruiser;
	m_tStat = { 500.f, 500.f, 25, 3, 256, 1.1f, 1300 , DF_LAGE, AT_NORMAL };

	m_iAttackFrame = 0;

	m_eRender = RENDER_SKYOBJ;
	m_tInfo.fCX = 120.f;
	m_tInfo.fCY = 120.f;

	A_GroundPos.x = (int)m_tInfo.fX / 32;
	A_GroundPos.y = (int)m_tInfo.fY / 32;


	m_Shade = new Gdiplus::Image(L"../StarCraft/Unit/Battlecruiser/BattleShade.bmp");

	Gdiplus::Color ExceptColor(255, 0, 0, 0);
	Gdiplus::Color endExceptColor(255, 30, 30, 30);
	m_imgAttr.SetColorKey(ExceptColor, endExceptColor, Gdiplus::ColorAdjustTypeBitmap);

	m_ColorMatrix = {
	1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // R
	0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // G
	0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // B
	0.0f, 0.0f, 0.0f, 0.8f, 0.0f, // A (알파값 0.5로 설정)
	0.0f, 0.0f, 0.0f, 0.0f, 1.0f // 여기 값 바꾸면 프레임 박살남
	};
}

int CBattlecruiser::Update()
{
	if (m_bDead || m_tStat.m_iHp <= 0)
	{
		// 죽음 이펙트
		CSoundMgr::Get_Instance()->PlaySFX(L"BatteleDeath.mp3", 0.3f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CBattleDead>::CreateFX(m_tInfo.fX, m_tInfo.fY));
		return OBJ_DEAD;
	}

	KeyInput();
	Update_State();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CBattlecruiser::Late_Update()
{
	Change_Motion();
	CUnit::Move_Frame();

	if (AttackCoolTime + 1700 < GetTickCount64())
	{
		m_isAttack = false;
		AttackCoolTime = GetTickCount64();
	}
}

void CBattlecruiser::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hMemDC = CBmpMgr::Get_Instance()->Find_Image(m_pImgKey);
	HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_5");
	HDC		hShadeDC = CBmpMgr::Get_Instance()->Find_Image(L"BattleShade");

	using namespace Gdiplus;
	Graphics graphics(hDC);
	m_imgAttr.SetColorMatrix(&m_ColorMatrix, ColorMatrixFlagsDefault, ColorAdjustTypeBitmap);

	graphics.DrawImage(
		m_Shade,            // 화면에 랜더링할 이미지 데이터
		Rect(            // 이미지 출력 위치와 크기를 저장하는 클래스
			m_tRect.left + iScrollX,
			m_tRect.top + iScrollY + 30,
			(int)m_tInfo.fCX,        // 렌더링 받을 화면의 가로, 세로 범위
			(int)m_tInfo.fCY
		),
		0,								// 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * (int)m_eDir,
		(int)m_tInfo.fCX,
		(int)m_tInfo.fCY, UnitPixel,        // 좌표의 단위를 무엇으로 할지 (픽셀단위 임으로 UnitPixel)
		&m_imgAttr        // 컬러 매트릭스를 포함하고 있는 ImageAttributes 객체의 주소
	);

	if (m_bSelect)
	{
		GdiTransparentBlt(hDC,			// 복사 받을 DC
			m_tRect.left + iScrollX +10 ,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + iScrollY + 30,
			96,			// 복사 받을 이미지의 가로, 세로
			96,
			hFxDC,						// 복사할 이미지 DC	
			0, // 비트맵 출력 시작 좌표(Left, top)
			0,
			96,										// 복사할 이미지의 가로, 세로
			96,
			RGB(255, 0, 255));		// 제거할 색상
	}

	// 몸체
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
		RGB(0, 0, 0));		// 제거할 색상
}

void CBattlecruiser::Release()
{
}

void CBattlecruiser::Change_Motion()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case STATE_IDLE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 500;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_MOVE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 500;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 500;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_SHOOT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 500;
			m_tFrame.dwTime = GetTickCount64();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CBattlecruiser::KeyInput()
{
	// 야마토
	if (CKeyMgr::Get_Instance()->Key_Down('Y'))
	{
		if (!CGameMgr::Get_Instance()->Get_UpGrade_Compelate(UG_Battle_Yamato)) return;

		CMouseMgr::Get_Instance()->Get_Mouse()->SetYamatoMode();
	}
}

void CBattlecruiser::AttackToEnemy(CObj* _Enemey)
{
	m_eDir = GetDirection(m_tInfo.fX, m_tInfo.fY, _Enemey->Get_Info().fX, _Enemey->Get_Info().fY);
	if (m_eCurState == STATE_SHOOT)
		m_eCurState = STATE_SHOOT;
	else
		m_eCurState = STATE_ATTACK;

	if (m_AttackTime + _Enemey->Get_Stat()->Colldown < GetTickCount64() && !m_isAttack)
	{
		CSoundMgr::Get_Instance()->PlaySFX(L"BattleAttack.mp3", 0.8f);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CBattleAttack>::CreateBattleAtk(this, _Enemey));
		m_isAttack = true;
		m_AttackTime = GetTickCount64();
	}
}

void CBattlecruiser::Update_State()
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
	case IP_Chase:
		ChaseUnit();
		break;
	case IP_YAMTO:
		Yamato();
		break;
	case IP_END:
		break;
	default:
		break;
	}
}

bool CBattlecruiser::CanGo(Pos pos)
{
	if (pos.y >= 128 || pos.x >= 128) return false;
	if (CMapMgr::Get_Instance()->GetTileType(pos) >= 0)
		return true;
	else
		return false;
}

void CBattlecruiser::Move_toNext()
{
	Pos _now = { (int)m_tInfo.fY / TILECY , (int)m_tInfo.fX / TILECY };
	Pos _pos = _path[m_iPathIndex];

	if (m_iPathIndex == 0)
	{
		m_iPathIndex = 1;
		return;
	}

	const float EPSILON = m_tStat.m_fSpeed * 10.0f;
	if (_now == _pos)
	{
		++m_iPathIndex;
	}
	else
	{
		Pos dir = (_pos - _now);
		for (int i = 0; i < DIR_END; i++)
		{
			if (dir == MoveFront[i])
			{
				m_eDir = (DIRECTION)i;
				break;
			}
		}

		// 이동
		if (CCollisionMgr::Collision_RangeChack_bool(this, *m_pUnitList, 50.f))
		{
			m_eCurState = STATE_IDLE;
		}
		else
		{
			// 단위 벡터로 수정?
			fPOINT point = Nomalization(MoveFront[m_eDir]);

			m_eCurState = STATE_MOVE;
			m_tInfo.fX += m_tStat.m_fSpeed * point.x;
			m_tInfo.fY += m_tStat.m_fSpeed * point.y;
		}
	}

}

void CBattlecruiser::Yamato()
{
	if (m_iPathIndex < _path.size())
	{
		Move_toNext();
	}
	else if (m_iPathIndex == _path.size())
	{
		m_eCurState = STATE_IDLE;

		YamatoCount++;
		m_eDir = GetDirection(m_tInfo.fX, m_tInfo.fY, YamatoEnemy->Get_Info().fX , YamatoEnemy->Get_Info().fY);
		if (YamatoCount == 1)
		{
			CSoundMgr::Get_Instance()->PlaySFX(L"BattleAmato.mp3", 0.5f);
		}
		else if (YamatoCount > 100)
		{
			CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CBattleYamato>::CreateBattleAtk(this, YamatoEnemy));
			YamatoEnemy = nullptr;
			YamatoCount = 0;
			m_eInput = IP_Chase;
		}
	}
}

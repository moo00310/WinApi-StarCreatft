#include "pch.h"
#include "CTank.h"
#include "CMapMgr.h"
#include "CObjMgr.h"
#include "CBmpMgr.h"
#include "CSoundMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"
#include "CCollisionMgr.h"

void CTank::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	// 리스트 할당
	m_pMonsterList = CObjMgr::Get_Instance()->Get_MonsterList();
	m_pUnitList = CObjMgr::Get_Instance()->Get_ObjList(OBJ_PLAYER);

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Tank/TankBody.bmp", L"TankBody");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Tank/TankHead.bmp", L"TankHead");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Select/Select_4(80.80).bmp", L"Select_4");

	m_pImgKey = L"TankBody";
	m_eObjID = OT_Tank;
	m_tStat = { 150.f, 150.f, 30, 1, 224, 1.8f, 625 , DF_LAGE, AT_EXPLOSIVE };

	m_iAttackFrame = 14;

	m_eRender = RENDER_GAMEOBJECT;
	m_tInfo.fCX = 128.f;
	m_tInfo.fCY = 128.f;
}

int CTank::Update()
{
	if (m_bDead || m_tStat.m_iHp <= 0)
	{
		// 죽음 이펙트
		//CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, CAbstractFactory<CGhostDead>::Create(m_tInfo.fX, m_tInfo.fY));
		//CSoundMgr::Get_Instance()->StopSound(SOUND_EFFECT);
		//CSoundMgr::Get_Instance()->PlaySound(L"Marine_Dead_1.mp3", SOUND_EFFECT, 0.5f, true);

		return OBJ_DEAD;
	}

	Update_State();

	__super::Update_Rect();
	return OBJ_NOEVENT;
}

void CTank::Late_Update()
{
	Change_Motion();
	CUnit::Move_Frame();
}

void CTank::Render(HDC hDC)
{
	int		iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int		iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC		hBodyDC = CBmpMgr::Get_Instance()->Find_Image(L"TankBody");
	HDC		hHeadDC = CBmpMgr::Get_Instance()->Find_Image(L"TankHead");
	HDC		hFxDC = CBmpMgr::Get_Instance()->Find_Image(L"Select_4");

	if (m_bSelect)
	{
		GdiTransparentBlt(hDC,			// 복사 받을 DC
			m_tRect.left + iScrollX + 25,	// 복사 받을 위치 좌표 X, Y	
			m_tRect.top + iScrollY + 30,
			80,			// 복사 받을 이미지의 가로, 세로
			80,
			hFxDC,						// 복사할 이미지 DC	
			0, // 비트맵 출력 시작 좌표(Left, top)
			0,
			80,										// 복사할 이미지의 가로, 세로
			80,
			RGB(255, 0, 255));		// 제거할 색상
	}

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hBodyDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * m_tFrame.iCurCount, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * (int)m_eDir,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 255, 0));		// 제거할 색상

	GdiTransparentBlt(hDC,			// 복사 받을 DC
		m_tRect.left + iScrollX,	// 복사 받을 위치 좌표 X, Y	
		m_tRect.top + iScrollY -5,
		(int)m_tInfo.fCX,			// 복사 받을 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		hHeadDC,						// 복사할 이미지 DC	
		(int)m_tInfo.fCX * 0, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * (int)m_eAttackDir,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(0, 255, 0));		// 제거할 색상

}

void CTank::Release()
{
}

// BodyChange
void CTank::Change_Motion()
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
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iCurCount = 1;
			m_tFrame.dwSpeed = 100;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iCurCount = 0;
			m_tFrame.dwSpeed = 200;
			m_tFrame.dwTime = GetTickCount64();
			break;

		case STATE_SHOOT:
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CTank::Move()
{
	if (m_iPathIndex < _path.size())
	{
		Pos _now = { (int)m_tInfo.fY / TILECY , (int)m_tInfo.fX / TILECY };
		Pos _pos = _path[m_iPathIndex];
		Pos _pre = _path[max(m_iPathIndex - 1, 0)];

		fPOINT _fNow = { m_tInfo.fX, m_tInfo.fY };
		fPOINT _fPos = { _path[m_iPathIndex].x * TILECY + 16.f ,_path[m_iPathIndex].y * TILECY + 16.f };

		if (m_iPathIndex == 0)
		{
			m_iPathIndex = 1;
			return;
		}

		const float EPSILON = m_tStat.m_fSpeed * 10.0f;
		if (fabsf(_fNow.x - _fPos.x) < EPSILON && fabsf(_fNow.y - _fPos.y) < EPSILON)
		{
			// 맵 타일 옵션 변경
			CMapMgr::Get_Instance()->SetTileType(_pos, 2);
			CMapMgr::Get_Instance()->SetTileType(_pre, 0);
			++m_iPathIndex;
		}
		else
		{
			Pos dir = (_pos - _pre);
			for (int i = 0; i < DIR_END; i++)
			{
				if (dir == MoveFront[i])
				{
					m_eDir = (DIRECTION)i;
					m_eAttackDir = (DIRECTION)i;
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
				fPOINT point = Nomalization(MoveFront[m_eDir]);

				m_eCurState = STATE_MOVE;
				m_tInfo.fX += m_tStat.m_fSpeed * point.x;
				m_tInfo.fY += m_tStat.m_fSpeed * point.y;
			}
		}
	}
	else if (m_iPathIndex == _path.size())
	{
		m_eInput = IP_STOP;
	}
}

void CTank::Attack()
{
	if (m_iPathIndex < _path.size())
	{
		CObj* Enemy = nullptr;
		Pos _now = { (int)(m_tInfo.fY / TILECY) , (int)(m_tInfo.fX / TILECX) };
		Pos _pos = _path[m_iPathIndex];

		if (_now == _pos)
			++m_iPathIndex;
		else
		{

			if ((Enemy = CCollisionMgr::Collision_RangeChack(this, *m_pMonsterList, m_tStat.m_iRange)) == nullptr)
			{
				// 방향 설정
				Pos dir = (_pos - _now);
				for (int i = 0; i < DIR_END; i++)
				{
					if (dir == MoveFront[i])
					{
						m_eDir = (DIRECTION)i;
						//m_eAttackDir = (DIRECTION)i;
						break;
					}
				}

				// 단위 벡터로 수정?
				float x(0.f), y(0.f);
				float length = sqrtf(float(dir.x * dir.x + dir.y * dir.y));
				if (length != 0)
				{
					x = dir.x / length;
					y = dir.y / length;
				}

				// 이동
				m_eCurState = STATE_MOVE;
				m_tInfo.fX += m_tStat.m_fSpeed * x;
				m_tInfo.fY += m_tStat.m_fSpeed * y;
			}
			else
			{
				m_eAttackDir = GetDirection(m_tInfo.fX, m_tInfo.fY, Enemy->Get_Info().fX, Enemy->Get_Info().fY);
				m_eCurState = STATE_ATTACK;
				AttackToEnemy(Enemy);
			}

		}
	}
	else if (m_iPathIndex == _path.size())
	{
		m_eInput = IP_STOP;
	}
}

void CTank::Hold()
{
	CObj* Enemy = nullptr;
	if ((Enemy = CCollisionMgr::Collision_RangeChack(this, *m_pMonsterList, m_tStat.m_iRange)) == nullptr)
	{
		m_eCurState = STATE_IDLE;
	}
	else
	{
		m_eAttackDir = GetDirection(m_tInfo.fX, m_tInfo.fY, Enemy->Get_Info().fX, Enemy->Get_Info().fY);
		m_eCurState = STATE_ATTACK;
	}
}

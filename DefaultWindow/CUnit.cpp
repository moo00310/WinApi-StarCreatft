#include "pch.h"
#include "CUnit.h"
#include "CMapMgr.h"
#include "CCollisionMgr.h"
#include "CAbstractFactory.h"
#include "CObjMgr.h"
#include "CBulletEffect.h"
void CUnit::Move_Frame()
{
	if (m_tFrame.dwTime + m_tFrame.dwSpeed < GetTickCount64())
	{
		++m_tFrame.iCurCount;

		if (m_eCurState == STATE_ATTACK && m_tFrame.iCurCount > m_tFrame.iFrameEnd)
		{
			m_eCurState = STATE_SHOOT;
			m_tFrame.iCurCount = m_tFrame.iFrameStart;
		}
		else if (m_tFrame.iCurCount > m_tFrame.iFrameEnd)
			m_tFrame.iCurCount = m_tFrame.iFrameStart;
		

		m_tFrame.dwTime = GetTickCount64();
	}
}

void CUnit::Update_State()
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
	case IP_END:
		break;
	default:
		break;
	}
}

void CUnit::Astar(Pos _tTarget_Index)
{
	Pos start = { (int)m_tInfo.fY / TILECY , (int)m_tInfo.fX / TILECY };
	if (start.x > 128 || start.y > 128) return;

	// OpenList
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;

	// close[y][x] -> (y, x)에 방문을 했는지 여부
	vector<vector<bool>> closed(128, vector<bool>(128, false));

	// best[y][x] -> 지금까지 (y, x)에 대한 가장 좋은 비용 (작을 수록 좋음)
	vector<vector<int>> best(128, vector<int>(128, INT_MAX));

	// 부모 추적 용도
	map<Pos, Pos> parent;

	// 초기값
	{
		if (!CanGo(_tTarget_Index))
		{
			return;
		}
		int g = 0;
		int h = 10 * (abs(_tTarget_Index.y - _tTarget_Index.y) + abs(_tTarget_Index.x - _tTarget_Index.x));
		pq.push(PQNode{ g + h, g, start });
		best[start.y][start.x] = g + h;
		parent[start] = start;

	}

	while (pq.empty() == false)
	{
		// 제일 좋은 후보를 찾는다
		PQNode node = pq.top();
		pq.pop();

		// 동일한 좌표를 여러 경로로 찾아서\
		// 더 빠른 경로로 인해서 이미 방문(closed)된 경우 스킵
		// [선택]
		if (closed[node.pos.y][node.pos.x])
			continue; 
		if (best[node.pos.y][node.pos.x] < node.f)
			continue;

		// 방문
		closed[node.pos.y][node.pos.x] = true;

		// 목적지에 도착했으면 바로 종료
		if (node.pos == _tTarget_Index)
			break;

		for (int dir = 0; dir < DIR_END; dir++)
		{
			Pos nextPos = node.pos + MoveFront[dir];

			if (nextPos.x < 0 || nextPos.y < 0)
				continue;
			if (nextPos.x >= 128 || nextPos.y >= 128)
				continue;

			// 갈 수 있는 지역은 맞는지 확인
			if (CanGo(nextPos) == false)
				continue;

			// 벽을 낀 대각선 이동 방지
			if (CanMove(node.pos, dir) == false)
				continue;

			// [선택] 이미 방문한 곳이면 스킵
			if (closed[nextPos.y][nextPos.x])
				continue;

			// 비용 계산
			int g = node.g + MoveCost[dir];
			int h = 10 * (abs(_tTarget_Index.y - nextPos.y) + abs(_tTarget_Index.x - nextPos.x));
			// 다른 경로에서 더 빠른 길을 찾았으면 스킵
			if (best[nextPos.y][nextPos.x] <= g + h)
				continue;

			// 예약 진행
			best[nextPos.y][nextPos.x] = g + h;
			pq.push(PQNode{ g + h, g, nextPos });
			parent[nextPos] = node.pos;
		}
	}

	// 거꾸로 거슬러 올라간다
	Pos pos = _tTarget_Index;

	_path.clear();
	m_iPathIndex = 0;


	while (true)
	{
		_path.push_back(pos);

		// 시작점은 자신이 곧 부모이다
		if (pos == parent[pos])
			break;

		pos = parent[pos];
	}
	std::reverse(_path.begin(), _path.end());
}

bool CUnit::CanGo(Pos pos)
{
	if (pos.y > 128 || pos.x > 128) return false;
	if (CMapMgr::Get_Instance()->GetTileType(pos) <= 1)
		return true;
	else
		return false;
}

bool CUnit::CanMove(Pos _pos, int _dir)
{
	if (_dir == 0 || _dir == 4 || _dir == 8 || _dir == 12)
		return true;
	else if (_dir == 2 || _dir == 6 || _dir == 10 || _dir == 14)
	{
		int left = _dir - 2;
		int right = _dir + 2;

		if (right > 15) right = 0;

		if (CanGo(_pos + MoveFront[left]) && CanGo(_pos + MoveFront[right]))
			return true;
		else
			return false;
	}
	else
	{
		int left = _dir - 1;
		int right = _dir + 1;

		if (right > 15) right = 0;
		if (CanGo(_pos + MoveFront[left]) && CanGo(_pos + MoveFront[right]))
			return true;
		else
			return false;
	}

}


void CUnit::HomeAttack(float x, float y)
{
	// 해당 좌표로 공격
	A_GroundPos = { (int)y/32,(int)x/32 };
	Astar(A_GroundPos);
	m_eInput = IP_ATTACK;
}


void CUnit::Move()
{
	if (m_iPathIndex < _path.size())
	{
		Move_toNext();
	}
	else if (m_iPathIndex == _path.size())
	{
		m_eInput = IP_Chase;
	}
		
}

void CUnit::Stop()
{
	m_eCurState = STATE_IDLE;
	return;
}

void CUnit::Attack()
{
	CObj* unit(nullptr);

	if ((unit = CCollisionMgr::Collision_RangeChack_Attack(this, *m_pMonsterList, *m_pBuildList_E, m_tStat.m_iRange + 64.f)) != nullptr)
	{
		if (CCollisionMgr::Collision_Range_Bool(this, unit, m_tStat.m_iRange)) // 충돌 범위 내
		{
			CCollisionMgr::Collision_RectEx(this, *m_pUnitList);
			AttackToEnemy(unit);
		}
		else
		{
			if (preUint != unit)
			{
				Astar(CCollisionMgr::Collision_RangePos(this, unit, m_tStat.m_iRange - 32.f));
				preUint = unit;
			}

			if (m_iPathIndex < _path.size())
			{
				Move_toNext();
			}
			else if (m_iPathIndex == _path.size())
			{
				if (CCollisionMgr::Collision_Range_Bool(this, unit, m_tStat.m_iRange)) // 충돌 범위 내
				{
					CCollisionMgr::Collision_RectEx(this, *m_pUnitList);
					AttackToEnemy(unit);
				}
				else
				{
					Astar(CCollisionMgr::Collision_RangePos(this, unit, m_tStat.m_iRange - 32.f));
					return;
				}

			}
		}

	}
	else
	{
		if (m_iPathIndex < _path.size())
		{
			Move_toNext();
		}
		else if (m_iPathIndex == _path.size())
		{
		
			if (preUint != nullptr)
			{
				Astar(A_GroundPos); // 새로운 목표 위치로 이동
				preUint = nullptr;
			}
			else
			{

				CCollisionMgr::Collision_RectEx(this, *m_pUnitList);
				m_eCurState = STATE_IDLE;
			}
				
		}
	}
		
}

void CUnit::Hold()
{
	CObj* Enemy = nullptr;
	if ((Enemy = CCollisionMgr::Collision_RangeChack(this, *m_pMonsterList, m_tStat.m_iRange)) == nullptr)
	{
		m_eCurState = STATE_IDLE;
	}
	else
	{
		AttackToEnemy(Enemy);
	}
}

void CUnit::Move_toNext()
{
	Pos _now = { (int)m_tInfo.fY / TILECY , (int)m_tInfo.fX / TILECY };
	Pos _pos = _path[m_iPathIndex];

	if (m_iPathIndex == 0)
	{
		m_iPathIndex = 1;
		CMapMgr::Get_Instance()->SetTileType(_now, 1);
		return;
	}

	const float EPSILON = m_tStat.m_fSpeed * 10.0f;
	if (_now == _pos)
	{
		CMapMgr::Get_Instance()->SetTileType(_now, 1);
		CMapMgr::Get_Instance()->SetTileType(_path[m_iPathIndex-1], 0);
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

void CUnit::ChaseUnit()
{
	CObj* unit(nullptr);

	if ((unit = CCollisionMgr::Collision_RangeChack_Attack(this, *m_pMonsterList, *m_pBuildList_E, m_tStat.m_iRange + 96.f)) != nullptr)
	{
		if (preUint != unit)
		{
			Astar(CCollisionMgr::Collision_RangePos(this, unit, m_tStat.m_iRange - 32.f));
			preUint = unit;
		}

		if (m_iPathIndex < _path.size())
		{
			Move_toNext(); 
		}
		else if (m_iPathIndex == _path.size())
		{
			if (CCollisionMgr::Collision_Range_Bool(this, unit, m_tStat.m_iRange))
			{
				// 공격
				AttackToEnemy(unit);
			}
			else
			{
				m_eCurState = STATE_IDLE;
				preUint = nullptr;
			}
		}
	}
	else
	{
		m_eCurState = STATE_IDLE;
		return;
	}
}



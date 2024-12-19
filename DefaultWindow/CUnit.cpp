#include "pch.h"
#include "CUnit.h"
#include "CMapMgr.h"
#include "CCollisionMgr.h"

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
	case IP_END:
		break;
	default:
		break;
	}
}

void CUnit::Astar(Pos _tTarget_Index)
{
	Pos start = { (int) m_tInfo.fY / TILECY , (int)m_tInfo.fX / TILECY };

	// OpenList
	priority_queue<PQNode, vector<PQNode>, greater<PQNode>> pq;

	// close[y][x] -> (y, x)에 방문을 했는지 여부
	vector<vector<bool>> closed(75, vector<bool>(75, false));

	// best[y][x] -> 지금까지 (y, x)에 대한 가장 좋은 비용 (작을 수록 좋음)
	vector<vector<int>> best(75, vector<int>(75, INT_MAX));

	// 부모 추적 용도
	map<Pos, Pos> parent;

	// 초기값
	{
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
			if (nextPos.x > 74 || nextPos.y > 74)
				continue;

			// 갈 수 있는 지역은 맞는지 확인
			if (CanGo(nextPos) == false)
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
	if (CMapMgr::Get_Instance()->GetTileType(pos) <= 1)
		return true;
	else
		return false;
}

DIRECTION CUnit::GetDirection(float player_x, float player_y, float monster_x, float monster_y)
{
	float dx = monster_x - player_x;
	float dy = monster_y - player_y;

	float angle = atan2(dy, dx);  // 라디안
	float angle_deg = angle * (180 / PI);  // 각도 변환

	if (angle_deg < 0) {
		angle_deg += 360;  // 음수를 0-360 범위로
	}

	int sector = static_cast<int>((angle_deg + 11.25) / 22.5) % 16;
	switch (sector) {
	case 0: return DIR_RIGHT;
	case 1: return DIR_RIGHT_DOWN;
	case 2: return DIR_RD;
	case 3: return DIR_DOWN_RIGHT;
	case 4: return DIR_DOWN;
	case 5: return DIR_DOWN_LEFT;
	case 6: return DIR_LD;
	case 7: return DIR_LEFT_DOWN;
	case 8: return DIR_LEFT;
	case 9: return DIR_LEFT_UP;
	case 10: return DIR_LU;
	case 11: return DIR_UP_LEFT;
	case 12: return DIR_UP;
	case 13: return DIR_UP_RIGHT;
	case 14: return DIR_RIGHT_UP;
	case 15: return DIR_RU;	
	default: return DIR_END;  // 예외 처리
	}
}

void CUnit::AttackToEnemy(CObj* _Enemey)
{
	if (m_dwTime + _Enemey->Get_Stat()->Colldown < GetTickCount64()&&
		m_tFrame.iCurCount == m_iAttackFrame)
	{
		DEFENCEID Dfence_id = _Enemey->Get_Stat()->m_eDfenceID;
		ATTACKID Attack_id = m_tStat.m_eAttackID;
		float Damge = fabsf((_Enemey->Get_Stat()->m_iDefence) - ((DamageCalcu[Attack_id][Dfence_id] * m_tStat.m_iAttack)));

		_Enemey->Add_Stat_hp(-Damge);

		m_dwTime = GetTickCount64();
	}
}
// 애니메이션과 싱크 문제

void CUnit::Move()
{
	if (m_iPathIndex < _path.size())
	{
		Pos _now = { (int)m_tInfo.fY / TILECY , (int)m_tInfo.fX / TILECY };
		Pos _pos = _path[m_iPathIndex];

		if (_now == _pos)
			++m_iPathIndex;
		else
		{
			// 방향 설정
			Pos dir = (_pos - _now);
			for (int i = 0; i < DIR_END; i++)
			{
				if (dir == MoveFront[i])
				{
					m_eDir = (DIRECTION)i;
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
	}
	else if (m_iPathIndex == _path.size())
		m_eCurState = STATE_IDLE;
}

void CUnit::Stop()
{
	m_eCurState = STATE_IDLE;
	return;
}

void CUnit::Attack()
{
	if (m_iPathIndex < _path.size())
	{
		CObj* Enemy = nullptr;
		Pos _now = { (int)m_tInfo.fY / TILECY , (int)m_tInfo.fX / TILECY };
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
				m_eDir = GetDirection(m_tInfo.fX, m_tInfo.fY, Enemy->Get_Info().fX, Enemy->Get_Info().fY);
				m_eCurState = STATE_ATTACK;
				AttackToEnemy(Enemy);
			}

		}
	}
	else if (m_iPathIndex == _path.size())
	{
		m_eCurState = STATE_IDLE;
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
		m_eDir = GetDirection(m_tInfo.fX, m_tInfo.fY, Enemy->Get_Info().fX, Enemy->Get_Info().fY);
		m_eCurState = STATE_ATTACK;
	}

}



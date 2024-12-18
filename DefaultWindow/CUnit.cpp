#include "pch.h"
#include "CUnit.h"
#include "CMapMgr.h"



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



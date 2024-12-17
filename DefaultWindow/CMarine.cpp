#include "pch.h"
#include "CMarine.h"
#include "CScrollMgr.h"
#include "CBmpMgr.h"
#include "CKeyMgr.h"
#include "CObjMgr.h"
#include "CAbstractFactory.h"
#include "CBloodEffect.h"
#include "CMapMgr.h"

CMarine::CMarine() : CUnit(UNIT_MARINE), m_iImgId(0), m_eCurState(STATE_END), m_ePreState(STATE_END)
, m_dwTime(GetTickCount64()), m_Map(nullptr), m_iPathIndex(0)
{
    ZeroMemory(&m_tFrame, sizeof(FRAME));
}

CMarine::~CMarine()
{
	Release();
}

void CMarine::Initialize()
{
	// 맵의 주소를 받아옴
	m_Map = CMapMgr::Get_Instance()->GetMap();

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../StarCraft/Unit/Marine/Marine.bmp", L"Marine");

    m_pImgKey = L"Marine";
    m_tStat = { 40, 6, 0, 4, 1.8f, 15 , DF_SAMLL, AT_NORMAL };
	m_eCurState = STATE_IDLE;
	m_ePreState = STATE_IDLE;
	m_eDir = DIR_DOWN_RIGHT;

	m_tInfo.fX = 400.f;
	m_tInfo.fY = 400.f;
    m_tInfo.fCX = 50.f;
    m_tInfo.fCY = 50.f;

	m_eRender = RENDER_GAMEOBJECT;
}

int CMarine::Update()
{
	if (m_bDead)
	{
		// 죽음 이펙트
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT,CAbstractFactory<CMarineDead>::Create(m_tInfo.fX, m_tInfo.fY));
		return OBJ_DEAD;
	}
	
	MoveTo();
	Test_Key_Input();
	Change_Motion();

	__super::Update_Rect();

    return OBJ_NOEVENT;
}

void CMarine::Late_Update()
{
	__super::Move_Frame();

#ifdef  _DEBUG

	if (m_dwTime + 1000 < GetTickCount64())
	{
		cout << "마린 회전 상태 : " << m_eDir << endl;
		m_dwTime = GetTickCount64();
	}

#endif //  _DEBUG
}

void CMarine::Render(HDC hDC)
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
		(int)m_tInfo.fCX * m_tFrame.iCurCount, // 비트맵 출력 시작 좌표(Left, top)
		(int)m_tInfo.fCY * (int)m_eDir,
		(int)m_tInfo.fCX,										// 복사할 이미지의 가로, 세로
		(int)m_tInfo.fCY,
		RGB(255, 255, 0));		// 제거할 색상

}

void CMarine::Release()
{
}

void CMarine::Change_Motion()
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
			m_tFrame.iFrameStart = 13;
			m_tFrame.iFrameEnd = 14;
			m_tFrame.iCurCount = 11;
			m_tFrame.dwSpeed = 150;
			m_tFrame.dwTime = GetTickCount64();
			break;
		}

		m_ePreState = m_eCurState;
	}

}

void CMarine::Test_Key_Input()
{

	if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
	{
		m_eCurState = STATE_ATTACK;
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing('E'))
	{
		if (m_iImgId > 15)
			m_iImgId = 0;
		else
			m_eDir = (DIRECTION)m_iImgId++;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down('R'))
	{
		m_bDead = true;
	}
	else
		//m_eCurState = STATE_IDLE;


	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		m_tInfo.fY -= m_tStat.m_fSpeed;
		
	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		m_tInfo.fY += m_tStat.m_fSpeed;

	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT))
	{
		m_tInfo.fX -= m_tStat.m_fSpeed;

	}
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT))
	{
		m_tInfo.fX += m_tStat.m_fSpeed;

	}

	

	if (CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
	{
		Pos temp = { 600 / TILECY , 600 / TILECY };
		Astar(temp);
	}
}

//A-ster
void CMarine::Astar(Pos _tTarget_Index)
{
	// y,x
	Pos start = { m_tInfo.fY / TILECY , m_tInfo.fX / TILECY };

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
	

	//TODO
	// 방향 설정 해당 좌표로 x y가 이동하는 거
}

bool CMarine::CanGo(Pos pos)
{
	if (CMapMgr::Get_Instance()->GetTileType(pos) <= 1)
		return true;
	else
		return false;
}

void CMarine::MoveTo()
{
	if (m_iPathIndex < _path.size())
	{
		Pos _now = { m_tInfo.fY / TILECY , m_tInfo.fX / TILECY };
		Pos _pos = _path[m_iPathIndex];

		if(_now == _pos)
			m_iPathIndex++;
		else
		{
			// 방향 설정
			Pos dir = (_pos - _now);
			for (int i = 0; i < DIR_END; i++)
			{
				if (dir == MoveFront[i])
				{
					m_eDir = (DIRECTION)i;
				}
			}

			m_eCurState = STATE_MOVE;

			// 이동
			m_tInfo.fX += m_tStat.m_fSpeed * dir.x;
			m_tInfo.fY += m_tStat.m_fSpeed * dir.y;

			// 단위 벡터로 수정?

		}
	}
	else if(m_iPathIndex == _path.size())
		m_eCurState = STATE_IDLE;
}


#pragma once

#define WINCX		800
#define WINCY		600

#define	PURE		= 0
#define PI			3.141592f

#define OBJ_NOEVENT			0
#define OBJ_DEAD			1	

#define VK_MAX				0xff

#define	TILECX				32  // 타일 사이즈
#define	TILECY				32

#define TILEWIDTH			75  //타일 개수
#define TILEHIGHT			75


extern HWND		g_hWnd;

enum DIRECTION {
 // 오른쪽에서 반시계 방향으로 회전
	DIR_RIGHT,		// 오른쪽			//-----------------0도----------------------
	DIR_RIGHT_UP,	
	DIR_RU,			// 오른쪽 대각선		//-----------------45도----------------------
	DIR_UP_RIGHT,	
	DIR_UP,			// 위				//-----------------90도----------------------
	DIR_UP_LEFT,	
	DIR_LU,			// 왼쪽 대각선		//-----------------135도----------------------
	DIR_LEFT_UP,    
	DIR_LEFT,		// 왼쪽				//-----------------180도----------------------
	DIR_LEFT_DOWN,	
	DIR_LD,			// 왼쪽 아래			//-----------------225도----------------------
	DIR_DOWN_LEFT,	
	DIR_DOWN,		// 아래				//-----------------270도----------------------
	DIR_DOWN_RIGHT,
	DIR_RD ,		// 오른쪽 아래대각선	//-----------------315도----------------------
	DIR_RIGHT_DOWN,
	DIR_END, // 방향 없음
};

enum OBJID { OBJ_EFFECT, OBJ_PLAYER, OBJ_BULLET, OBJ_MONSTER, OBJ_MOUSE, OBJ_SHIELD, OBJ_BUTTON, OBJ_END };

enum RENDERID { RENDER_BACKGROUND, RENDER_GAMEOBJECT, RENDER_UI, RENDER_EFFECT, RENDER_END };

enum SCENEID { SC_LOGO, SC_MENU, SC_EDIT, SC_STAGE, SC_END };

enum EditType { ET_TILE = 0, ET_GRASS, ET_WALL, ET_BRIGE, ET_OBJECT, ET_END };

enum DEFENCEID { DF_SAMLL, DF_MEDIUM, DF_LAGE, DF_END};
enum ATTACKID { AT_NORMAL, AT_CONCUSSIVE, AT_EXPLOSIVE, AT_END};

const float DamageCalcu[AT_END][DF_END]
{
	{1.f,1.f,1.f},
	{0.5f,0.75f,1.f},
	{0.25f,0.5f,1.f}
};

enum UNITID{ UNIT_END, UNIT_SCV, UNIT_MARINE,  };
enum BUILDID { BUILD_COMMEND, BUILD_BARRCK, BUILD_END };

enum ATIMESTATE { STATE_IDLE ,STATE_MOVE, STATE_ATTACK, STATE_DEAD, STATE_END };
enum INPUTSTATE { IP_MOVE, IP_ATTACK, IP_HOLD, IP_STOP, IP_PATROL, IP_END };
enum CURSERSTATE { MS_IDLE, MS_OBJ, MS_ATTACK, MS_MOVE, MS_DRAG,
	MS_SCROLL_R, MS_SCROLL_UR, MS_SCROLL_U, MS_SCROLL_UL, MS_SCROLL_L,
	MS_SCROLL_DL, MS_SCROLL_D, MS_SCROLL_DR
};

enum TILE_GROUP { 
	TG_GROUND, 
	TG_HILL, 
 };

const int TILE_ID_RANGES[][2] = {
	{0, 13},    // GROUND
	{14, 27},   // HILL
};


typedef struct tagInfo
{
	float		fX, fY;
	float		fCX, fCY;	// 가로, 세로 길이

}INFO;

typedef struct tagLinePoint
{
	float		fX, fY;

	tagLinePoint()	{ZeroMemory(this, sizeof(tagLinePoint));}
		tagLinePoint(float _fX, float _fY)
		: fX(_fX), fY(_fY)
	{	}

}LINEPOINT;

typedef struct tagLine
{
	LINEPOINT	tLPoint;
	LINEPOINT	tRPoint;

	tagLine() { 	ZeroMemory(this, sizeof(tagLine));	}
	tagLine(LINEPOINT& _tLPoint, LINEPOINT& _tRPoint)
		: tLPoint(_tLPoint), tRPoint(_tRPoint) {	}

}LINE;

typedef struct tagFrame
{
	int		iFrameStart;
	int		iFrameEnd;
	int		iCurCount;
	DWORD	dwSpeed;
	ULONGLONG	dwTime;

}FRAME;


typedef struct tagStat
{
	float m_iMaxHp;		// 최대 체력
	float m_iHp;		// 체력
	int m_iAttack;		// 공격력
	int m_iDefence;     // 방어력
	float m_iRange;		// 사거리
	float m_fSpeed;		// 이동 속도
	int Colldown;		// 공격속도

	DEFENCEID m_eDfenceID;
	ATTACKID m_eAttackID;

}STAT;


template<typename T>
void Safe_Delete(T& Temp)
{
	if (Temp)
	{
		delete Temp;
		Temp = nullptr;
	}
}

struct tagFinder
{
public:
	tagFinder(const TCHAR* pString) : m_pString(pString) {}

public:
	template<typename T>
	bool	operator()(T& rObj)
	{
		return !lstrcmp(m_pString, rObj.first);
	}

private:
	const TCHAR* m_pString;
};

struct DeleteMap
{
public:
	template<typename T>
	void	operator()(T& Pair)
	{
		if (Pair.second)
		{
			delete Pair.second;
			Pair.second = nullptr;
		}
	}
};

/*--------------------------
	A-Star
--------------------------*/
struct Pos
{
	bool operator==(Pos& other)
	{
		return y == other.y && x == other.x;
	}
	bool operator==(const Pos& other)
	{
		return y == other.y && x == other.x;
	}

	bool operator!=(Pos& other)
	{
		return !(*this == other);
	}

	bool operator<(const Pos& other) const
	{
		if (y != other.y)
			return y < other.y;
		return x < other.x;
	}

	Pos operator+(const Pos& other)
	{
		Pos ret;
		ret.y = y + other.y;
		ret.x = x + other.x;
		return ret;
	}

	Pos operator-(const Pos& other)
	{
		Pos ret;
		ret.y = y - other.y;
		ret.x = x - other.x;
		return ret;
	}

	Pos& operator+=(const Pos& other)
	{
		y += other.y;
		x += other.x;
		return *this;
	}


	Pos& operator-=(const Pos& other)
	{
		y -= other.y;
		x -= other.x;
		return *this;
	}

	int y = 0;
	int x = 0;
};

struct PQNode
{
	bool operator<(const PQNode& other) const { return f < other.f; }
	bool operator>(const PQNode& other) const { return f > other.f; }

	int	f; // f = g + h
	int	g;
	Pos		pos;
	DIRECTION dir;
};

const int MoveCost[16] =
{
	10,			// 오른쪽			
	18,
	14,			// 오른쪽 위 대각선	
	18,
	10,			// 위				
	18,
	14,			// 왼쪽 위 대각선		
	18,
	10,			// 왼쪽				
	18,
	14,			// 왼쪽 아래 대각선		
	18,
	10,			// 아래				
	18,
	14,			// 오른쪽 아래 대각선
	18			
};

const Pos MoveFront[] =
{
	// y , x
	Pos {0,1},	// 오른쪽			
	Pos {-1,2},
	Pos {-1,1},	// 오른쪽 위 대각선	
	Pos {-2,1},
	Pos {-1,0},	// 위				
	Pos {-2,-1},
	Pos {-1,-1},	// 왼쪽 위 대각선		
	Pos {-1,-2},
	Pos {0,-1},	// 왼쪽				
	Pos {1,-2},
	Pos {1,-1},	// 왼쪽 아래 대각선		
	Pos {2,-1},
	Pos {1,0},	// 아래				
	Pos {2,1},
	Pos {1,1},	// 오른쪽 아래 대각선
	Pos {1,2} 
};
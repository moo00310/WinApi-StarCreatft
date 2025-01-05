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

#define TILEWIDTH			128  //타일 개수
#define TILEHIGHT			128


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

enum OBJID { OBJ_PLAYER, OBJ_BUILD, OBJ_BUILD_E, OBJ_MONSTER, OBJ_EFFECT, OBJ_BUTTON, OBJ_MOUSE, OBJ_END };

enum RENDERID { RENDER_BACKGROUND, RENDER_EFFECT, RENDER_GAMEOBJECT, RENDER_HIT_EFFECT, RENDER_UI ,RENDER_END };

enum SOUND_ID { SOUND_BGM, SOUND_SFX, SOUN_ALL, SOUND_END };
enum SCENEID { SC_LOGO, SC_MENU, SC_EDIT, SC_STAGE, SC_END };

enum EditType { ET_TILE = 0, ET_Resourece, ET_END };

enum DEFENCEID { DF_SAMLL, DF_MEDIUM, DF_LAGE, DF_END};
enum ATTACKID { AT_NORMAL, AT_CONCUSSIVE, AT_EXPLOSIVE, AT_END};
enum UI_TYPE {UI_MAIN, UI_WIRE, UI_ICON, UI_PORT, UI_MIMAP, UI_END};

const float DamageCalcu[AT_END][DF_END]
{
	{1.f,1.f,1.f},
	{0.5f,0.75f,1.f},
	{0.25f,0.5f,1.f}
};

enum ATIMESTATE { STATE_IDLE ,STATE_MOVE, STATE_ATTACK, STATE_SHOOT, STATE_DEAD, STATE_END };
enum BuildSTATE { BS_TEMP, BS_MAKE, BS_LINK, BS_IDLE, BS_RUN, BS_FLY, BS_END };

enum INPUTSTATE { IP_MOVE, IP_ATTACK, IP_HOLD, IP_STOP, IP_PATROL, IP_BUILD, IP_HEAL, IP_Chase, IP_END };
enum CURSERSTATE { MS_IDLE, MS_OBJ, MS_ATTACK, MS_MOVE, MS_DRAG, MS_SCROLL_R, MS_SCROLL_U, MS_SCROLL_L, MS_SCROLL_D,};

typedef struct tagInfo
{
	float		fX, fY;
	float		fCX, fCY;	// 가로, 세로 길이

}INFO;

struct COLOR
{
	int R, G, B;

	void operator=(COLOR& rhs) {
		R = rhs.R;
		G = rhs.G;
		B = rhs.B;
	}
};

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

typedef struct floatPoint
{
	float		x, y;

	bool operator==(floatPoint& other)
	{
		return y == other.y && x == other.x;
	}

}fPOINT;

inline fPOINT Nomalization(const Pos _dir)
{
	fPOINT point = { 0.f, 0.f };
	float length = sqrtf(float(_dir.x * _dir.x + _dir.y * _dir.y));
	if (length != 0)
	{
		point.x = _dir.x / length;
		point.y = _dir.y / length;
	}
	
	return point;
}
/*-------------------------------------------
*		// 유닛과 건물
----------------------------------------------*/
enum OBJ_TYPE
{
	OT_Scv, OT_Marine, OT_Medic, OT_Ghost, OT_Tank, OT_SiegeTank, OT_Science_Vessel, OT_Unit_End,
	OT_Commend, OT_Suffly, OT_Refinery, OT_Barrck, OT_Academy ,OT_Factory, OT_Addon, OT_Armory, OT_Starport, OT_StarportAddOn,
	OT_ScienceFacility, OT_ScienceSecret, OT_CmdNuke, OT_Build_End,
	OT_Marine_SightUp, OT_Marine_Streampack, OT_Medic_Magic1, OT_Medic_Magic2, OT_Medic_Mana,
	OT_Vulture_Speed, OT_Vulture_mine, OT_Tank_SiegeMod, OT_Gholiat_SightUp,
	OT_Mecha_AtkUp, OT_Sky_AtkUp, OT_Mecha_DefUp, OT_Sky_DefUp,
	OT_Wirse_Cloak, OT_Wires_Mana,
	OT_Vessle_Emp, OT_Vessle_Irradiate, OT_Vessle_Mana,
	OT_Ghost_LockDown, OT_Ghost_Cloak, OT_Ghost_SightUp, OT_Ghost_Mana,
	OT_Cmd_Nuke,
	OT_END
};

enum iTemplateSize { TS_SMALL, TS_NORMAL, TS_LAGE };

const Pos Spawn_index[5] =
{
	{0, 0},
	{1, 0},
	{1, 1},
	{1, 2},
	{1, 3},
};


#define Frame  8

const map<OBJ_TYPE, std::tuple<int, int, int, int>> ObjCost =
{
	// 유닛 이름, {미네랄, 가스, 인구수, 소요시간}
	{OT_Scv,               {50,  0, 1, 20 * Frame}},
	{OT_Marine,            {50,  0, 1, 24 * Frame}},
	{OT_Medic,             {50, 25, 1, 30 * Frame}},
	{OT_Ghost,             {25, 75, 1, 50 * Frame}},
	{OT_Tank,              {150, 100, 2, 50 * Frame}},
	{OT_Science_Vessel,    {100, 225, 2, 80 * Frame}},

	// 건물 이름, {미네랄, 가스, 제공되는 인구수, 소요시간}
	{OT_Commend,           {400,  0, 0, 120 * Frame}},
	{OT_Suffly,            {100,  0,  0, 40 * Frame}},
	{OT_Refinery,          {100,  0,  0, 40 * Frame}},
	{OT_Barrck,            {150,  0,  0, 80 * Frame}},
	{OT_Academy,           {150,  0,  0, 80 * Frame}},
	{OT_Factory,           {200, 100, 0, 80 * Frame}},
	{OT_Addon,             {50,  50, 0, 40 * Frame}},
	{OT_Armory,            {100, 50, 0, 80 * Frame}},
	{OT_Starport,          {150, 100, 0, 70 * Frame}},
	{OT_StarportAddOn,     {50,  50, 0, 40 * Frame}},
	{OT_ScienceFacility,   {100, 150, 0, 60 * Frame}},
	{OT_ScienceSecret,     {50,  50, 0, 40 * Frame}},
	{OT_CmdNuke,           {100, 100, 0, 80 * Frame}},

	// 업그레이드 
	{OT_Marine_SightUp,		{150, 150, 0,100*Frame}},
	{OT_Marine_Streampack,	{100, 100, 0, 80*Frame}},
	{OT_Medic_Magic1,		{100, 100, 0, 80*Frame}},
	{OT_Medic_Magic2,		{100, 100, 0, 120*Frame}},
	{OT_Medic_Mana,			{150, 150, 0, 166*Frame}},

	{OT_Vulture_Speed,		{100, 100, 0, 100 * Frame}},
	{OT_Vulture_mine,		{100, 100, 0, 80 * Frame}},
	{OT_Tank_SiegeMod,		{150, 150, 0, 80 * Frame}},
	{OT_Gholiat_SightUp,	{100, 100, 0, 133 * Frame}},

	{OT_Mecha_AtkUp,		{100, 100, 0, 266 * Frame}},
	{OT_Sky_AtkUp,			{100, 100, 0, 266 * Frame}},
	{OT_Mecha_DefUp,		{100, 100, 0, 266 * Frame}},
	{OT_Sky_DefUp,			{150, 150, 0, 266 * Frame}},

	{OT_Wirse_Cloak,		{150, 150, 0, 100 * Frame}},
	{OT_Wires_Mana,			{200, 200, 0, 166 * Frame}},

	{OT_Vessle_Emp,			{200, 200, 0, 120 * Frame}},
	{OT_Vessle_Irradiate,	{200, 200, 0, 80 * Frame}},
	{OT_Vessle_Mana,		{150, 150, 0, 166 * Frame}},

	{OT_Ghost_LockDown,		{200, 200, 0, 100 * Frame}},
	{OT_Ghost_Cloak,		{100, 100, 0, 80 * Frame}},
	{OT_Ghost_SightUp,		{100, 100, 0, 166 * Frame}},
	{OT_Ghost_Mana,			{150, 150, 0, 166 * Frame}},

	{OT_Cmd_Nuke,			{100, 100, 0, 80 * Frame}},
};

const fPOINT BuildTemplate_Size = { 160, 128 }; 


enum Tech
{
	TECH_Braack,
	TECH_Academy,
	TECH_Factory,
	TECH_Armory,
	TECH_Starport,
	TECH_CovertOps,
	TECH_END
};

enum ICON
{
	IC_Move,
	IC_Stop,
	IC_Attack,
	IC_Patrol,
	IC_Hold,
	IC_Cancel,
	IC_Return,
	IC_Gater,
	IC_Refair,
	IC_Build,
	IC_AdBUild,
	//--------
	IC_Scv,
	//-------
	IC_Marine,
	IC_firebat,
	IC_Ghost,
	//-------
	IC_Vulture,
	IC_Tank,
	IC_Goliath,
	//--------
	IC_Wirse,
	IC_Dropship,
	IC_BattleCulsor,
	IC_Vassle,
	//--------------
	IC_Commend,
	IC_Suffly,
	IC_Refinery,
	IC_Barrack,
	IC_Enginer,
	IC_Terret,
	IC_Archerdemy,
	IC_Bungker,
	//---------
	IC_Factory,
	IC_Starport,
	IC_Siencefacilly,
	IC_Armory,
	//-------------
	IC_Scan_Addon,
	IC_Nuke_Addon,
	IC_CoOvp_Addon,
	IC_BattleAddon,
	IC_StarportAddon,
	IC_FactoryAddon,
	//--------------
	IC_Siegemod,
	IC_Tankmod,
	//-------------
	IC_Medic,
	IC_Vakily,
	//----------------
	IC_MarineRichUp,
	IC_MarineSteamPack,
	IC_MedicMegic_1,
	IC_MedicMegic_2,
	IC_MedicMana,
	//--------------
	IC_Boinic_AtkUp,
	IC_Boinic_DefUp,
	//-------------
	IC_Mecha_AtkUp,
	IC_Sky_AtkUp,
	IC_Mecha_DefUp,
	IC_Sky_DefUp,
	//-------------
	IC_Vasse_Emp,
	IC_Vasse_Eraady,
	IC_Vasse_Mana,
	//--------------
	IC_Wirse_Cloak,
	IC_Wirse_Decloak,
	IC_Wirse_Mana,
	//---------------
	IC_Vulture_SpeedUp,
	IC_Vulture_MineUp,
	IC_Tank_SiegeUp,
	IC_Goliath_SightUp,
	//----------------
	IC_Battle_AmatoUp,
	IC_Battle_ManaUp,
	//---------------
	IC_Cmd_NukeUp,
	IC_Ghost_LockDownUp,
	IC_Ghost_CloakUp,
	IC_Ghost_SightUp,
	IC_Ghost_Mana,
	IC_Cmp_Scan,
	//-------------------
	IC_Ghost_Cloak,
	IC_Ghost_LockDown,
	IC_Ghost_Nuke,
	//------
	IC_Battle_Amato
};

enum Upgrade
{
	UG_Marine_SightUp,
	UG_Marine_Streampack,
	UG_Medic_Magic1,
	UG_Medic_Magic2,
	UG_Medic_Mana,

	UG_Vulture_Speed,
	UG_Vulture_mine,
	UG_Tank_SiegeMod,
	UG_Gholiat_SightUp,

	UG_Mecha_AtkUp,
	UG_Sky_AtkUp,
	UG_Mecha_DefUp,
	UG_Sky_DefUp,

	UG_Wirse_Cloak,
	UG_Wires_Mana,

	UG_Vessle_Emp,
	UG_Vessle_Irradiate,
	UG_Vessle_Mana,

	UG_Ghost_LockDown,
	UG_Ghost_Cloak,
	UG_Ghost_SightUp,
	UG_Ghost_Mana,

	UG_Cmd_Nuke,
	UG_END
};
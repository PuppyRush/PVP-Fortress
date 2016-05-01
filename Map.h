#pragma once
class GameMap
{
public:

	LONG Size, Height[MAPSIZE*WIDTH];

	CRect Coords[30][MAPSIZE*WIDTH], GameRect;
	INT HEIGHT, Wind, Map, MapWay, Kind;
	INT Remain[30][MAPSIZE*WIDTH];		//맵 형식에 따라 한 칸이 몇번만에 파괴 될지를 결정 할 변수
	GameMap();
	~GameMap(void);
	void SetMap();
	void SetClientRect(CRect rect){GameRect=rect;}
};




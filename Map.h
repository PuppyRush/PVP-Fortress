#pragma once
class GameMap
{
public:

	LONG Size, Height[MAPSIZE*WIDTH];

	CRect Coords[30][MAPSIZE*WIDTH], GameRect;
	INT HEIGHT, Wind, Map, MapWay, Kind;
	INT Remain[30][MAPSIZE*WIDTH];		//�� ���Ŀ� ���� �� ĭ�� ������� �ı� ������ ���� �� ����
	GameMap();
	~GameMap(void);
	void SetMap();
	void SetClientRect(CRect rect){GameRect=rect;}
};




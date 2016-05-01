#pragma once
class Tank
{
public:
	
	CString Name;						//탱크 이름
	int Size;							//탱크 크기
	int Kind;							//탱크 종류
	int Num;							//탱크 번호(인덱스)
	int Bomb;							//선택된 탱크의 포탄
	int Bullet;							//선택된 탱크의 총
	int Speed;							//탱크의 속도
	int Health;							//탱크의 체력
	int Defense;						//탱크의 방어력
	int Damage;							//탱크의 공격력
	int JumpDegree;						//탱크의 점프력
	int X,H;							//탱크의 현재 위치한 X번째 열과 높이
	int BulletSleepTime;				//총탄 발사 후 몇초동안 쉴 것인가?
	int BombSleepTime;					//포탄...?
	int JumpLevel;						//점프할 때 y좌표
	int JumpSleepTime;					//점프 후 몇초동안 쉴 것인가?
	int JumpTime;						//점프 후 쿹타임(쉬기)
	int BulletTime;						//총탄 발사 후 쿨타임
	int BombTime;						//포탄 발사 후 쿨타임
	int MaxAngel, MinAngel;				//탱크마다의 최소각도 최대각도			
	LONG PowerCoords;					//*힘을 결정할 변수이자 게이지바의 좌표
	BOOL Sign,Fire;						//움직일 안내각도, 게이지의 방향결정, 방향과 힘 모든순간을 Fire로 정의
	BOOL dx;							//탱크가 향하고 있는 방향
	BOOL Landing;						//바닥을 감지 후 착지
	BOOL Jump, Fall, Angel, Power;		//탱크의 점프, 번지, 발사여부
	BOOL JumpSleep,BulletSleep,BombSleep;		//점프, 총탄, 포탄의 가능여부				
	BOOL EntryLeft, EntryRight,NoEntry;	//진입여부
	CPoint Coords;						//*탱크의 현재 좌표
	CPoint AngelCoords;					//*각도를 결정할 변수이자 각도안내선의 좌표
	CBitmap TankBmp;					//탱크의 비트맵
	BITMAP TankInfo;
	
	
public:
	Tank();
	~Tank(void);
	void SetBitmap(int Tank);
	void SetTank(int x);


};


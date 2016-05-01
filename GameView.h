
// GameView.h : GameView 클래스의 인터페이스
//

#pragma once

#define MAKEINTERSOURCE(i)(LPWSTR)((DWORD)((WORD)(i)))

//갯수 정의
#define MAPSIZE		3		//맵의 전체크기
#define BLOCKNUM	5		//블럭 높이의 평균
#define RANDBLOCK	2		//+-블럭높이
#define WIDTH		30		//맵조각의 가로갯수
#define MENU_NUM	4
#define OPTION_NUM  5
#define TANK_NUM	4
#define WEAPON_NUM	3

//속도 정의
#define FALLSPD		2
#define JUMPSPD		1
#define BULLET1_SPD	3
#define MOVESPD		1

//탱크 정의
#define TANK1		0		//JAVA
#define TANK2		1		//미스터빅
#define TANK3		2
#define TANK4		3

//위치 정의
#define GAUGE_Y		10		//게이지바의 시작높이
#define GAUGE_H		20		//게이지바의 높이
#define GAUGE_W		500		//게이지바의 너비

//크기 정의
#define JUMPLEVEL	20
#define TANKSIZE	3


//무기이름 정의
#define CALIBER45	0
#define CALIBER60	1
#define CALIBER90	2
#define BLACK		0
#define RED			1
#define YELLOW		2


//무기공격력 정의
#define CALIBER45_DMG	3
#define CALIBER60_DMG	5
#define CALIBER90_DMG	8
#define BLACK_DMG		22
#define RED_DMG			17
#define YELLOW_DMG		13


//무기쿨타임 정의
#define CALIBER45_TIME		4
#define CALIBER60_TIME		6
#define CALIBER90_TIME		9
#define BLACK_TIME			19
#define RED_TIME			14
#define YELLOW_TIME			11


//타이머이름 정의
#define GRAVITY			1		//탱크에 중력을 적용
#define JUMP			2		//탱크가 점프하고 다시 떨어지는 경우에 적용
#define SHOOT			3		//파워 게이지의 바에 적용
#define WEAPONBOMB		4		//포탄발사시 적용
#define WEAPONBULLET	5		//총탄 발사시 적용
#define SLEEP			6		//점프 후 당분간 뛰지 못하게 한다.4
#define REALTIME		7		//초 재기
#define CLOUDCLK		8		//구름 생성
#define MOVECLOUD		9		//구름 움직이기
#define MOVEANGEL		10		//각도선 움직이기
#define CHANGEWIND		11		//바람을 바꾸기
#define COMJUMP			12		//컴퓨터탱크의 점프 간격
#define COMBULLET		13		//컴퓨터탱크의 총탄발사 간격
#define COMBOMB			14



//시간정의
#define JUMPTIME		500
#define SLEEPTIME		3		//(0.5초당..)
#define BULLET_TIME		1
#define CLOUDTIME		1000	//구름이 만들어지는 시간단위
#define WINDTIME		15000
#define COMJUMPTIME		3000	
#define COMBULLETTIME	4000
#define COMBOMBTIME		9000


//게임옵션 정의
#define WOOD		2		//2번만에 맵 조각이 없어짐
#define ASP			5
#define METAL		-1
#define PVP			1		//일대일 대전 방식
#define FLAG		0		//점령전(컴퓨터와 유저대결)
#define FLAT		10
#define MOUNTAIN	11
#define TRENCH		12
#define	RANDOM		13
#define LOTO		0
#define MRBIG		1
#define JAVA		2
#define KNU			3

//타이틀,옵션화면
#define TITLE		0
#define MENU		1
#define OPTION		2
#define SelectMap	3
#define SelectChar	4

//기타정의
#define RAD 3.141592/180
#define G	-0.1

class Tank;
class GameMap;
class GameView : public CView
{
protected: // serialization에서만 만들어집니다.
	GameView();
	DECLARE_DYNCREATE(GameView)

// 특성입니다.
public:
	GameDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~GameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
//	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:

	typedef struct BULLET{
		int H,X;
		int item;
		int size;
		CPoint coords;
		BOOL dx;
		struct BULLET *next, *pre;
	}BULLET;
	BULLET **Bullet, **BulletTail; 

	typedef struct BOMB{
		double co,si;
		int t,F;
		int item;
		int size;
		int H,X;
		LONG vx,vy;
		BOOL dx,dy;
		CPoint coords;
		COLORREF col;
	}BOMB;
	BOMB **Bomb;

	typedef struct CLOUD{
		CPoint coords;
		LONG tempy,Levely;
		BOOL dy;

		struct CLOUD *next, *pre;
	}CLOUD;
	CLOUD *Cloud, *CloudTail;

	Tank *tank;
	GameMap *Map;


	INT INT_Order1, INT_Order2 ,INT_TankOrder,INT_TankNum;
	INT  *INT_MoveX, *INT_BulletTime, *INT_BombTime;
	
	INT OP_GameWay, OP_Wind, OP_Map, OP_MapWay, OP_Tank1, OP_Tank2, OP_Bullet1, OP_Bullet2, OP_Bomb1, OP_Bomb2;

	BOOL Title, Start, Option, Main, _Tank, Weapon, Info[3], Sel1,Sel2, SelWeapon,End;
	
	BOOL SetBkSound, BkSound;
	CString STR_BkSound1, STR_BkSound2;

	LONG WindForce;
	CPoint CPT_Arrow, CPT_Select[3], CPT_Option[3]; 
	CRect GameRect,GaugeRect1,GaugeRect2, SetGaugeRect1, SetGaugeRect2, SetHealthRect1, SetHealthRect2, HealthRect1,HealthRect2;
	CRect *ComHpBkRect, *ComHpRect;
	CRect BulletRect1, BulletRect2, BombRect1, BombRect2, BkBombRect1, BkBombRect2, BkBulletRect1, BkBulletRect2;
	CRect TempInterRect;
	CBitmap BMP_Onoff,BMP_Method, BMP_Map;

	CRect T_Main[4]; 
	CRect T_Option[5], T_MapWay[4], T_GameWay[2], T_Map[3], T_Wind[2];
	CRect T_Tank[4];
	CRect T_Weapon1[3],T_Weapon2[3];

	
#ifndef _DEBUG  // GameView.cpp의 디버그 버전
inline GameDoc* GameView::GetDocument() const
   { return reinterpret_cast<GameDoc*>(m_pDocument); }
#endif

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnInitialUpdate();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DrawImage(CDC*);
	void DrawVirtual(void);
	void PhysicalModel_Tank(void);
	void PhysicalModel_Bullet(void);
	void PhysicalModel_Bomb(void);

	bool MakeBullet(int);
	bool DestroyBullet(int,BULLET*);
	CRect PtToRect(CPoint pt, int x){CRect rect; rect.left = pt.x-x; rect.top=pt.y-x ; rect.right=pt.x+x ; rect.bottom=pt.y+x; return rect;}
	CRect BiggerRect(CRect rt, int x){return CRect(rt.left-x, rt.top-x, rt.right+x, rt.bottom+x);}
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void CheckDeadTank(void);
	void GameEnd(int TankNum);
	LONG GetH(int TankNum);
	LONG GetX(int TankNum);
	void CheckEntry(int i);
	void CollisionBullet(int ,int, int);
	void CollisionBomb(int , int);
	void MakeBomb(int);
	void DestroyBomb(int);
	void UpdateFPS();
	void ShatterMap(BULLET*);
	void CalPhy(int i);
	bool SetStart(void);
	void MakeCloud(void);
	void DestroyCloud(void);
	void DrawBar(CDC*);
};


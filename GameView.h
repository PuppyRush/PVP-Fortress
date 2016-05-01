
// GameView.h : GameView Ŭ������ �������̽�
//

#pragma once

#define MAKEINTERSOURCE(i)(LPWSTR)((DWORD)((WORD)(i)))

//���� ����
#define MAPSIZE		3		//���� ��üũ��
#define BLOCKNUM	5		//�� ������ ���
#define RANDBLOCK	2		//+-������
#define WIDTH		30		//�������� ���ΰ���
#define MENU_NUM	4
#define OPTION_NUM  5
#define TANK_NUM	4
#define WEAPON_NUM	3

//�ӵ� ����
#define FALLSPD		2
#define JUMPSPD		1
#define BULLET1_SPD	3
#define MOVESPD		1

//��ũ ����
#define TANK1		0		//JAVA
#define TANK2		1		//�̽��ͺ�
#define TANK3		2
#define TANK4		3

//��ġ ����
#define GAUGE_Y		10		//���������� ���۳���
#define GAUGE_H		20		//���������� ����
#define GAUGE_W		500		//���������� �ʺ�

//ũ�� ����
#define JUMPLEVEL	20
#define TANKSIZE	3


//�����̸� ����
#define CALIBER45	0
#define CALIBER60	1
#define CALIBER90	2
#define BLACK		0
#define RED			1
#define YELLOW		2


//������ݷ� ����
#define CALIBER45_DMG	3
#define CALIBER60_DMG	5
#define CALIBER90_DMG	8
#define BLACK_DMG		22
#define RED_DMG			17
#define YELLOW_DMG		13


//������Ÿ�� ����
#define CALIBER45_TIME		4
#define CALIBER60_TIME		6
#define CALIBER90_TIME		9
#define BLACK_TIME			19
#define RED_TIME			14
#define YELLOW_TIME			11


//Ÿ�̸��̸� ����
#define GRAVITY			1		//��ũ�� �߷��� ����
#define JUMP			2		//��ũ�� �����ϰ� �ٽ� �������� ��쿡 ����
#define SHOOT			3		//�Ŀ� �������� �ٿ� ����
#define WEAPONBOMB		4		//��ź�߻�� ����
#define WEAPONBULLET	5		//��ź �߻�� ����
#define SLEEP			6		//���� �� ��а� ���� ���ϰ� �Ѵ�.4
#define REALTIME		7		//�� ���
#define CLOUDCLK		8		//���� ����
#define MOVECLOUD		9		//���� �����̱�
#define MOVEANGEL		10		//������ �����̱�
#define CHANGEWIND		11		//�ٶ��� �ٲٱ�
#define COMJUMP			12		//��ǻ����ũ�� ���� ����
#define COMBULLET		13		//��ǻ����ũ�� ��ź�߻� ����
#define COMBOMB			14



//�ð�����
#define JUMPTIME		500
#define SLEEPTIME		3		//(0.5�ʴ�..)
#define BULLET_TIME		1
#define CLOUDTIME		1000	//������ ��������� �ð�����
#define WINDTIME		15000
#define COMJUMPTIME		3000	
#define COMBULLETTIME	4000
#define COMBOMBTIME		9000


//���ӿɼ� ����
#define WOOD		2		//2������ �� ������ ������
#define ASP			5
#define METAL		-1
#define PVP			1		//�ϴ��� ���� ���
#define FLAG		0		//������(��ǻ�Ϳ� �������)
#define FLAT		10
#define MOUNTAIN	11
#define TRENCH		12
#define	RANDOM		13
#define LOTO		0
#define MRBIG		1
#define JAVA		2
#define KNU			3

//Ÿ��Ʋ,�ɼ�ȭ��
#define TITLE		0
#define MENU		1
#define OPTION		2
#define SelectMap	3
#define SelectChar	4

//��Ÿ����
#define RAD 3.141592/180
#define G	-0.1

class Tank;
class GameMap;
class GameView : public CView
{
protected: // serialization������ ��������ϴ�.
	GameView();
	DECLARE_DYNCREATE(GameView)

// Ư���Դϴ�.
public:
	GameDoc* GetDocument() const;

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~GameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
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

	
#ifndef _DEBUG  // GameView.cpp�� ����� ����
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


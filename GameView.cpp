
// GameView.cpp : GameView 클래스의 구현
//

//탱크들이 동시에 움직이는 가운데 이러한 탱크들의 움직임을 감지하기 위해
//모든 움지임, 충돌검사에 Loop i<INT_TankNum 같은 반복문으로
//현재의 탱크들을 모두 검사 한다

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFC_ExodusTank.h"
#endif

#include "GameDoc.h"
#include "stdafx.h"
#include "GameView.h"
#include "Tank.h"
#include "Map.h"
#include <cmath>


#pragma comment(lib, "winmm")
#include <MMSystem.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// GameView

IMPLEMENT_DYNCREATE(GameView, CView)

BEGIN_MESSAGE_MAP(GameView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &GameView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// GameView 생성/소멸

GameView::GameView()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	srand((unsigned)time(NULL));

	Title = true;
	Option = Main = Start = Info[1] = Info[0] = Info[2] = End = Start = Sel1 = Sel2 = SelWeapon = false;
	INT_Order1= 0;
	INT_Order2= 0;
	WindForce = 1;
	Cloud = NULL;
	OP_Tank1= OP_Tank2= OP_Bullet1= OP_Bullet2=OP_Bomb1= OP_Bomb2 = 0;
	OP_Wind = OP_Map = OP_MapWay = 0;
	OP_GameWay =1;
	SetBkSound = true;
	BkSound = false;
	End = false;

}

GameView::~GameView()
{
	if(Start){
		delete Map;
		delete [] tank;
		delete [] INT_MoveX;
		delete [] INT_BulletTime;
		delete [] INT_BombTime;
	
		for(int i=0 ; i<INT_TankNum ; i++){
			delete [] Bomb[i];
			delete [] Bullet[i];
		}
	}

}

BOOL GameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// GameView 그리기

void GameView::OnDraw(CDC* /*pDC*/)
{
	GameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void GameView::OnPaint()
{
	
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CView::OnPaint()을(를) 호출하지 마십시오.
	CDC *pDC=GetDC();
	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	CBitmap *pOldBitmap, bitmap;

	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldBitmap = (CBitmap*)memDC.SelectObject(&bitmap);
	memDC.PatBlt(0,0, rect.Width(), rect.Height(), WHITENESS);
	
	DrawImage(&memDC);

	pDC->BitBlt(0,0, rect.Width(), rect.Height(), &memDC, 0,0,SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	ReleaseDC(pDC);
	bitmap.DeleteObject();
}



void GameView::DrawImage(CDC* pDC)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	GetClientRect(GameRect);

	CBrush *pOldBrush=NULL;
	CPen *pOldPen=NULL;

	//타이틀화면
	if(Title){

		CBitmap TitleBmp, *pOldBmp;
		BITMAP MapInfo;
		
		TitleBmp.LoadBitmapW(_TITLE);

		TitleBmp.GetBitmap(&MapInfo);

		pOldBmp = (CBitmap *)memDC.SelectObject(&TitleBmp);
		pDC->BitBlt(0,0,GameRect.Width(), GameRect.Height(), &memDC, 0,0, SRCCOPY);

		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
		TitleBmp.DeleteObject();

	}

	//게임 선택
	else if(Main){

		CBitmap MainBmp, *pOldBmp;
		BITMAP MapInfo;
		
		MainBmp.LoadBitmapW(_MAIN);

		MainBmp.GetBitmap(&MapInfo);

		pOldBmp = (CBitmap *)memDC.SelectObject(&MainBmp);
		pDC->BitBlt(0,0,GameRect.Width(), GameRect.Height(), &memDC, 0,0, SRCCOPY);

		CBrush SelBrush;
		SelBrush.CreateStockObject(NULL_BRUSH);
		CPen SelPen( PS_SOLID, 3, RGB(255,255,255));
		pDC->SelectObject(&SelBrush);
		pDC->SelectObject(&SelPen);

		pDC->Rectangle(T_Main[INT_Order1]);
		
		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
		MainBmp.DeleteObject();
	}


	else if(Option){
		
		CBitmap MainBmp, *pOldBmp;
		BITMAP MapInfo;
		
		MainBmp.LoadBitmapW(_OPTION);

		MainBmp.GetBitmap(&MapInfo);

		pOldBmp = (CBitmap *)memDC.SelectObject(&MainBmp);
		pDC->BitBlt(0,0,GameRect.Width(), GameRect.Height(), &memDC, 0,0, SRCCOPY);

		CBrush SelBrush;
		SelBrush.CreateStockObject(NULL_BRUSH);
		CPen SelPen( PS_SOLID, 3, RGB(255,255,255));
		pDC->SelectObject(&SelBrush);
		pDC->SelectObject(&SelPen);

		pDC->Rectangle(T_Option[INT_Order1]);
		pDC->Rectangle(T_GameWay[OP_GameWay]);
		pDC->Rectangle(T_Map[OP_Map]);
		pDC->Rectangle(T_Wind[OP_Wind]);
		pDC->Rectangle(T_MapWay[OP_MapWay]);
		
		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
		MainBmp.DeleteObject();
	}

	else if(Info[1]){

		if(Info[2]){
			CBitmap InfoBmp, *pOldBmp;
			BITMAP MapInfo;
		
			InfoBmp.LoadBitmapW(_INFO1);

			InfoBmp.GetBitmap(&MapInfo);

			pOldBmp = (CBitmap *)memDC.SelectObject(&InfoBmp);
			pDC->BitBlt(0,0,GameRect.Width(), GameRect.Height(), &memDC, 0,0, SRCCOPY);
		
			memDC.SelectObject(pOldBmp);
			memDC.DeleteDC();
			InfoBmp.DeleteObject();
		}
		else if(Info[3]){
			CBitmap InfoBmp, *pOldBmp;
			BITMAP MapInfo;
		
			InfoBmp.LoadBitmapW(_INFO2);

			InfoBmp.GetBitmap(&MapInfo);

			pOldBmp = (CBitmap *)memDC.SelectObject(&InfoBmp);
			pDC->BitBlt(0,0,GameRect.Width(), GameRect.Height(), &memDC, 0,0, SRCCOPY);
		
			memDC.SelectObject(pOldBmp);
			memDC.DeleteDC();
			InfoBmp.DeleteObject();
		}
	}

	else if(_Tank){
		CBitmap TankBmp, *pOldBmp;
		BITMAP MapInfo;
		
		TankBmp.LoadBitmapW(_TANK);

		TankBmp.GetBitmap(&MapInfo);

		pOldBmp = (CBitmap *)memDC.SelectObject(&TankBmp);
		pDC->BitBlt(0,0,GameRect.Width(), GameRect.Height(), &memDC, 0,0, SRCCOPY);

		CBrush SelBrush;
		SelBrush.CreateStockObject(NULL_BRUSH);
		CPen SelPen( PS_SOLID, 3, RGB(255,255,255));
		CPen SelPen2( PS_SOLID, 3, RGB(128,128,128));
		pOldBrush = pDC->SelectObject(&SelBrush);
		pOldPen = pDC->SelectObject(&SelPen);

		pDC->Rectangle(T_Tank[INT_Order1]);
		SelPen.DeleteObject();
		if(OP_GameWay==PVP){

			pOldPen = pDC->SelectObject(&SelPen2);
			pDC->Rectangle(BiggerRect(T_Tank[INT_Order2],1));
		}

		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
		TankBmp.DeleteObject();
	}


	else if(Weapon){
		CBitmap WeaponBmp, *pOldBmp;
		BITMAP MapInfo;
		
		WeaponBmp.LoadBitmapW(_WEAPON);

		WeaponBmp.GetBitmap(&MapInfo);

		pOldBmp = (CBitmap *)memDC.SelectObject(&WeaponBmp);
		pDC->BitBlt(0,0,GameRect.Width(), GameRect.Height(), &memDC, 0,0, SRCCOPY);

		CBrush SelBrush;
		SelBrush.CreateStockObject(NULL_BRUSH);
		CPen SelPen( PS_SOLID, 3, RGB(255,255,255));
		CPen SelPen2( PS_SOLID, 3, RGB(128,128,128));
		pDC->SelectObject(&SelBrush);
		pDC->SelectObject(&SelPen);

		if(!SelWeapon){
			pDC->Rectangle(T_Weapon1[INT_Order1]);
			if(OP_GameWay==PVP)	{
				pOldPen = pDC->SelectObject(&SelPen2);
				pDC->Rectangle( BiggerRect(T_Weapon1[INT_Order2],1));
			}
		}	
		else{
			pDC->Rectangle(T_Weapon2[INT_Order1]);
			if(OP_GameWay==PVP){
				pOldPen = pDC->SelectObject(&SelPen2);
				pDC->Rectangle( BiggerRect(T_Weapon2[INT_Order2],1));
			}
		}

		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
		WeaponBmp.DeleteObject();
	}
	
	else if(Start){
		Map->GameRect = GameRect;


		//배경 그리기
		CBitmap MapBmp,BkBmp;
		BITMAP MapInfo,BkInfo;
		

		if(OP_Map==0){
			MapBmp.LoadBitmapW(_WOOD);
			BkBmp.LoadBitmapW(_NATURE);
		}
		else if(OP_Map==1){
			MapBmp.LoadBitmapW(_ASP);
			BkBmp.LoadBitmapW(_CONCRTE);
		}
		else if(OP_Map==2){
			MapBmp.LoadBitmapW(_METAL);
			BkBmp.LoadBitmapW(_CITY);
		}
		MapBmp.GetBitmap(&MapInfo);
		BkBmp.GetBitmap(&BkInfo);
		CBitmap *pOldBmp = (CBitmap *)memDC.SelectObject(&BkBmp);
		pDC->SetStretchBltMode(COLORONCOLOR);
		pDC->StretchBlt(0,0,GameRect.Width(), GameRect.Height(), &memDC, 0,0,BkInfo.bmWidth, BkInfo.bmHeight,SRCCOPY);

		//구름 그리기
		CBitmap CloudBmp;
		BITMAP CloudInfo;
		CloudBmp.LoadBitmapW(_CLOUD);
		memDC.SelectObject(&CloudBmp);
		CloudBmp.GetBitmap(&CloudInfo);
		
		CLOUD *temp=Cloud;
		while(temp != NULL){
			pDC->TransparentBlt( temp->coords.x , temp->coords.y ,
							CloudInfo.bmWidth, CloudInfo.bmHeight,
							&memDC, 0,0, CloudInfo.bmWidth,CloudInfo.bmHeight, RGB(0,0,0));			
			temp = temp->next;
		}

		//총알 그리기
		CBrush BulletBrush(RGB(192,192,192));
		CPen BulletPen(PS_SOLID,1,RGB(0,0,0));

		pDC->SelectObject( &BulletBrush);
		pDC->SelectObject( &BulletPen);

		for(int i=0 ; i<INT_TankNum ; i++){
			BULLET *temp = Bullet[i];
			while(temp != NULL){
				pDC->Rectangle( PtToRect(temp->coords, temp->size));
				temp = temp->next;
			}
		}

		
		//포탄 그리기
		CBrush BombBrush;
		CPen BombPen(PS_SOLID,1,RGB(0,0,0));
		pDC->SelectObject(&BombPen);
		for(int i=0 ; i<INT_TankNum ; i++){
			if(Bomb[i] != NULL){
				BombBrush.CreateSolidBrush(Bomb[i]->col);
				pDC->Ellipse( PtToRect(Bomb[i]->coords, Bomb[i]->size));
				BombBrush.DeleteObject();

			}

		


		}

		//지형 그리기
		CBrush MapBrush;
		CPen MapPen(PS_SOLID,2,RGB(0,0,0));
		MapBrush.CreatePatternBrush(&MapBmp);
		CBrush *pOldBrush = pDC->SelectObject(&MapBrush);
		CPen *pOldPen = pDC->SelectObject(&MapPen);


		for(int i=0 ; i <= WIDTH ; i++)
			for(int l=Map->HEIGHT-1 ; Map->HEIGHT-Map->Height[i] <= l ; l--){
	
				pDC->Rectangle(Map->Coords[l][i]);
			}

		//선긋기
		CPen ppen(PS_SOLID,1,RGB(0,0,0));
		pDC->SelectObject(&ppen);
		for(int i=GameRect.Width()/Map->Size ; i>= 0 ; i--){
			pDC->MoveTo(GameRect.right, GameRect.bottom-i*Map->Size );
			pDC->LineTo(0,GameRect.bottom-i*Map->Size);
			pDC->MoveTo(i*Map->Size, 0);
			pDC->LineTo(i*Map->Size, GameRect.bottom);
		}


		//탱크그리기
		CBitmap bmp,bmp_L;
		BITMAP bmpInfo,bmpInfo_L;

		for (int i=0 ; i<INT_TankNum ; i++){
			if(tank[i].Kind==TANK1){				//탱크 종류에 따라 리소스를 결정...
				bmp.LoadBitmapW(_TANK1);
				bmp_L.LoadBitmapW(_TANK1_L);
			}
			else if(tank[i].Kind==TANK2){
				bmp.LoadBitmapW(_Tank2);
				bmp_L.LoadBitmapW(_TANK2_L);
			}
			else if(tank[i].Kind==TANK3){
				bmp.LoadBitmapW(_TANK3);
				bmp_L.LoadBitmapW(_TANK3_L);
			}
			else if(tank[i].Kind==TANK4){
				bmp.LoadBitmapW(_TANK4);
				bmp_L.LoadBitmapW(_TANK4_L);
			}

			

			if(tank[i].dx==true){
				memDC.SelectObject(&bmp);
				bmp.GetBitmap(&bmpInfo);
				pDC->TransparentBlt( tank[i].Coords.x-20 , tank[i].Coords.y-18 , 
									bmpInfo.bmWidth, bmpInfo.bmHeight,
									&memDC, 0,0, bmpInfo.bmWidth, bmpInfo.bmHeight, RGB(255,255,0));
				
			}
			else if(tank[i].dx == false){
				memDC.SelectObject(&bmp_L);
				bmp_L.GetBitmap(&bmpInfo_L);
				pDC->TransparentBlt( tank[i].Coords.x-20 , tank[i].Coords.y-18 , 
									bmpInfo_L.bmWidth, bmpInfo_L.bmHeight,
									&memDC, 0,0, bmpInfo_L.bmWidth, bmpInfo_L.bmHeight, RGB(255,255,0));
				
			}
			bmp.DeleteObject();
			bmp_L.DeleteObject();
	
			
		}


		//각도그리기
		CPen AngelInfoPen;
		CPen AngelPen;

		for(int i=0 ; i<INT_TankNum ; i++){
			if(tank[i].Angel){				
				for(int l=0 ; l<13 ; l++){
					AngelInfoPen.CreatePen(PS_SOLID,1,RGB(153,0,133));
					pDC->SelectObject(&AngelInfoPen);	

					pDC->MoveTo(tank[i].Coords.x , tank[i].Coords.y);
					pDC->LineTo(tank[i].Coords.x + tank[i].Size*4*cos(-(RAD * (double)15*l )),  
					tank[i].Coords.y + tank[i].Size*4*sin(-(RAD* (double)15*l )));

					AngelInfoPen.DeleteObject();
				}
		
				AngelPen.CreatePen(PS_SOLID, 2, RGB(107,153,0));
				pDC->SelectObject(&AngelPen);

				pDC->MoveTo(tank[i].Coords.x , tank[i].Coords.y);
				pDC->LineTo(tank[i].AngelCoords);

				AngelPen.DeleteObject();
			}
		}


		pDC->SelectObject(pOldBrush);
		pDC->SelectObject(pOldPen);
		BulletBrush.DeleteObject();
		BulletPen.DeleteObject();


		DrawBar(pDC);
		CheckDeadTank();
		PhysicalModel_Tank();
		PhysicalModel_Bullet();
		PhysicalModel_Bomb();
		DestroyCloud();
		
		if(End){
			
			CBitmap EndBmp;
			BITMAP EndInfo;
			EndBmp.LoadBitmapW(_GAMEOVER);
			memDC.SelectObject(&EndBmp);
			EndBmp.GetBitmap(&EndInfo);
		

			pDC->TransparentBlt( GameRect.right/2-200 ,GameRect.bottom/2-100,
							EndInfo.bmWidth, EndInfo.bmHeight,
							&memDC, 0,0, EndInfo.bmWidth,EndInfo.bmHeight, RGB(255,0,255));			
	
			EndBmp.DeleteObject();
		}



		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
		BkBmp.DeleteObject();
	}

}

void GameView::DrawBar(CDC *pDC)
{

		CDC memDC;
		CBrush *pOldBrush;
		memDC.CreateCompatibleDC(pDC);

		//체력게이지 계기판(배경)
		CBitmap HealthBkBar;
		BITMAP HealthBkBarInfo;
		HealthBkBar.LoadBitmapW(_HEALTHBAR);
		memDC.SelectObject(&HealthBkBar);
		HealthBkBar.GetBitmap(&HealthBkBarInfo);
				
		if(OP_GameWay==FLAG)
			pDC->TransparentBlt( 100, 60 , 	HealthBkBarInfo.bmWidth, HealthBkBarInfo.bmHeight,
							&memDC, 0,0, HealthBkBarInfo.bmWidth,HealthBkBarInfo.bmHeight, RGB(255,0,255));		

		else if(OP_GameWay==PVP){
			pDC->TransparentBlt( GameRect.right-220 , 60 , 	HealthBkBarInfo.bmWidth, HealthBkBarInfo.bmHeight,
							&memDC, 0,0, HealthBkBarInfo.bmWidth,HealthBkBarInfo.bmHeight, RGB(255,0,255));
			pDC->TransparentBlt( 100, 60 , 	HealthBkBarInfo.bmWidth, HealthBkBarInfo.bmHeight,
							&memDC, 0,0, HealthBkBarInfo.bmWidth,HealthBkBarInfo.bmHeight, RGB(255,0,255));		
		}
		HealthBkBar.DeleteObject();


		//탱크의 체력게이지
		CBrush SetHealthBrush( RGB(200,30,40));
		pOldBrush = pDC->SelectObject(&SetHealthBrush);

		if(OP_GameWay==FLAG)
			pDC->RoundRect(CRect(100, 60 ,100+ 128- (124.0/100.0)*(double)(100.0-tank[0].Health), 87),
							CPoint(30,30));

		else if(OP_GameWay==PVP){
			pDC->RoundRect(CRect(100, 60 ,100+ 128- (124.0/100.0)*(double)(100.0-tank[1].Health), 87),
							CPoint(30,30));
			pDC->RoundRect(CRect(GameRect.right-226 + (124.0/100.0)*(double)(100-tank[0].Health),60,
								GameRect.right-80,86), CPoint(30,30));
		}

		pDC->SelectObject(pOldBrush);
		SetHealthBrush.DeleteObject();


		//파워게이지 배경
		CBitmap PowerBkBar;
		BITMAP PowerBkBarInfo;
		PowerBkBar.LoadBitmapW(_POWERBAR);
		memDC.SelectObject(&PowerBkBar);
		PowerBkBar.GetBitmap(&PowerBkBarInfo);
			
		if(OP_GameWay==FLAG)
			pDC->TransparentBlt( 80, 30 , 
							PowerBkBarInfo.bmWidth, PowerBkBarInfo.bmHeight,
							&memDC, 0,0, PowerBkBarInfo.bmWidth,PowerBkBarInfo.bmHeight, RGB(255,0,255));

		else if(OP_GameWay==PVP){
			pDC->TransparentBlt( 80, 30 , 
							PowerBkBarInfo.bmWidth, PowerBkBarInfo.bmHeight,
							&memDC, 0,0, PowerBkBarInfo.bmWidth,PowerBkBarInfo.bmHeight, RGB(255,0,255));
			pDC->TransparentBlt( GameRect.right-330 , 30 , 
								PowerBkBarInfo.bmWidth, PowerBkBarInfo.bmHeight,
								&memDC, 0,0, PowerBkBarInfo.bmWidth,PowerBkBarInfo.bmHeight, RGB(255,0,255));
		}
		PowerBkBar.DeleteObject();


		//움직일 파워게이지.
		CBrush SetGaugeBrush( RGB(152,254,0));
		pOldBrush = pDC->SelectObject( &SetGaugeBrush);
		if(OP_GameWay==FLAG)
			pDC->RoundRect(SetGaugeRect1,CPoint(30,30));
		else if(OP_GameWay==PVP){
			pDC->RoundRect(SetGaugeRect1,CPoint(30,30));
			pDC->RoundRect(SetGaugeRect2,CPoint(30,30));
		}
		SetGaugeBrush.DeleteObject();


		//쿨타임막대
		CPen NullPen(PS_NULL, 0, RGB(0,0,0));
		CBrush PanelBrush(RGB( 64,64,64));
		pDC->SelectObject(&NullPen);
		pDC->SelectObject(&PanelBrush);
			

		BkBombRect1 = CRect( tank[0].Coords.x - tank[0].Size*2 , tank[0].Coords.y+tank[0].Size+3,
						tank[0].Coords.x + tank[0].Size*2,  tank[0].Coords.y+tank[0].Size+10);
		
		BkBulletRect1 = CRect( tank[0].Coords.x - tank[0].Size*2 , tank[0].Coords.y+tank[0].Size+12,
						tank[0].Coords.x + tank[0].Size*2,  tank[0].Coords.y+tank[0].Size+19);
		if(OP_GameWay==PVP){
			BkBombRect2 = CRect( tank[1].Coords.x - tank[1].Size*2 , tank[1].Coords.y+tank[1].Size+3,
							tank[1].Coords.x + tank[1].Size*2,  tank[1].Coords.y+tank[1].Size+10);
			BkBulletRect2 = CRect( tank[1].Coords.x - tank[1].Size*2 , tank[1].Coords.y+tank[1].Size+12,
							tank[1].Coords.x + tank[1].Size*2,  tank[1].Coords.y+tank[1].Size+19);
		}

		//1P
		pDC->Rectangle( BkBombRect1);
		pDC->Rectangle( BkBulletRect1);
		//2P
		if(OP_GameWay==PVP){
			pDC->Rectangle( BkBombRect2);
			pDC->Rectangle( BkBulletRect2);
		}
		
		CBrush SleepBombBrush( RGB(18,102,255));
		CBrush SleepBulletBrush( RGB(219,151,0));


		//남은 시간 그리기
		//1P
		pOldBrush = pDC->SelectObject(&SleepBombBrush);
		BombRect1 = CRect( tank[0].Coords.x - tank[0].Size*2 , tank[0].Coords.y+tank[0].Size+3,
			tank[0].Coords.x- tank[0].Size*2 + ((tank[0].Size*2.0)/100.0)*2*INT_BombTime[0] ,  tank[0].Coords.y+tank[0].Size+10);
		pOldBrush = pDC->SelectObject(&SleepBombBrush);
		BulletRect1 = CRect( tank[0].Coords.x - tank[0].Size*2 , tank[0].Coords.y+tank[0].Size+12,
			tank[0].Coords.x  - tank[0].Size*2 + ((tank[0].Size*2.0)/100.0)*2*INT_BulletTime[0],  tank[0].Coords.y+tank[0].Size+19);

		//2P
		if(OP_GameWay==PVP){
		pOldBrush = pDC->SelectObject(&SleepBombBrush);
		BombRect2 = CRect( tank[1].Coords.x - tank[1].Size*2 , tank[1].Coords.y+tank[1].Size+3,
				tank[1].Coords.x- tank[0].Size*2+ (tank[0].Size*2.0/100.0)*2*INT_BombTime[1],  tank[1].Coords.y+tank[1].Size+10);
		pOldBrush = pDC->SelectObject(&SleepBombBrush);
		BulletRect2 = CRect( tank[1].Coords.x - tank[1].Size*2 , tank[1].Coords.y+tank[1].Size+12,
			tank[1].Coords.x - tank[1].Size*2+ ((tank[1].Size*2.0)/100.0)*2*INT_BulletTime[1] ,  tank[1].Coords.y+tank[1].Size+19);
		}

		//1P
		pDC->Rectangle( BombRect1);
		pDC->Rectangle( BulletRect1);

		//2P
		if(OP_GameWay==PVP){
			pDC->Rectangle( BombRect2);
			pDC->Rectangle( BulletRect2);
		}

		
		//컴퓨터 체력게이지배경 그리기
		if(OP_GameWay == FLAG){
			CBrush ComBkHp(RGB(64,64,64));
			pOldBrush = pDC->SelectObject(&ComBkHp);
		
			for(int i=1 ; i < INT_TankNum ; i++){
				ComHpBkRect[i] = CRect(tank[i].Coords.x - tank[i].Size*2, tank[i].Coords.y+tank[i].Size+3, tank[i].Coords.x + tank[i].Size*2, tank[i].Coords.y+tank[i].Size+10);
				pDC->Rectangle(ComHpBkRect[i]);
			}
			ComBkHp.DeleteObject();

			//컴퓨터 체력게이지 그리기
			CBrush ComHp(RGB(255,0,0));
			pOldBrush = pDC->SelectObject(&ComHp);

			for(int i=1 ; i < INT_TankNum ; i++){
				ComHpRect[i] = CRect(tank[i].Coords.x - tank[i].Size*2, tank[i].Coords.y+tank[i].Size+3,
										tank[i].Coords.x + tank[i].Size*2 - (tank[i].Size*2.0/100.0)*(100-tank[i].Health), tank[i].Coords.y+tank[i].Size+10);
				pDC->Rectangle(ComHpRect[i]);
			}
		}
}


void GameView::DrawVirtual(void)
{
	CDC *pDC=GetDC();
	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	CBitmap *pOldBitmap, bitmap;

	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldBitmap = (CBitmap*)memDC.SelectObject(&bitmap);
	memDC.PatBlt(0,0, rect.Width(), rect.Height(), WHITENESS);


	DrawImage(&memDC);

	pDC->BitBlt(0,0, rect.Width(), rect.Height(), &memDC, 0,0,SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
	ReleaseDC(pDC);
}


// GameView 인쇄


void GameView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL GameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void GameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void GameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

//void GameView::OnRButtonUp(UINT /* nFlags */, CPoint point)
//{
//	ClientToScreen(&point);
//	OnContextMenu(this, point);
//}

//void GameView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
//{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
//}


// GameView 진단

#ifdef _DEBUG
void GameView::AssertValid() const
{
	CView::AssertValid();
}

void GameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

GameDoc* GameView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(GameDoc)));
	return (GameDoc*)m_pDocument;
}
#endif //_DEBUG


// GameView 메시지 처리기






void GameView::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnClose();
}


void GameView::OnTimer(UINT_PTR nIDEvent)
{

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.


	//탱크의 점프
	if(nIDEvent==JUMP)

		for(int i=0 ; i < INT_TankNum ; i++){
			if( tank[i].JumpSleep && tank[i].Jump){
				if(tank[i].JumpLevel-JUMPLEVEL> tank[i].Coords.y+tank[i].Size+2){
					tank[i].Jump = false;
					tank[i].Fall = true;
				}
			}
		}


	
		
	//쿨타임은 PVP나 FLAG의 1P(i=0)에만 적용
	for(int i=0 ; i<INT_TankNum ; i++)
		if(nIDEvent==SLEEP && (OP_GameWay == PVP || (OP_GameWay == FLAG && i==0))){
		
			if(tank[i].JumpSleep){
				tank[i].JumpTime++;
				if(tank[i].JumpTime == tank[i].JumpSleepTime){
					tank[i].JumpSleep = false;
					tank[i].JumpTime = 0;
				}				
			}
			
			if(tank[i].BulletSleep){
				tank[i].BulletTime++;
				if(tank[i].Bullet == CALIBER45)
					INT_BulletTime[i] = 100/CALIBER45_TIME * tank[i].BulletTime;
				if(tank[i].Bullet == CALIBER60)
					INT_BulletTime[i] = 100/CALIBER60_TIME * tank[i].BulletTime;
				if(tank[i].Bullet == CALIBER90)
					INT_BulletTime[i] = 100/CALIBER90_TIME * tank[i].BulletTime;

				if((tank[i].Bullet == CALIBER45 && tank[i].BulletTime == CALIBER45_TIME+tank[i].BulletSleepTime)||
				   (tank[i].Bullet == CALIBER60 && tank[i].BulletTime == CALIBER60_TIME+tank[i].BulletSleepTime)||
				   (tank[i].Bullet == CALIBER90 && tank[i].BulletTime == CALIBER90_TIME+tank[i].BulletSleepTime)){
					tank[i].BulletSleep = false;
					tank[i].BulletTime = 0;
					INT_BulletTime[i] = 0;
				}
			}

			if(tank[i].BombSleep){
				tank[i].BombTime++;
				if(tank[i].Bomb == BLACK)
					INT_BombTime[i] = 100/BLACK_TIME * tank[i].BombTime;
				if(tank[i].Bomb == RED)
					INT_BombTime[i] = 100/ RED_TIME * tank[i].BombTime;
				if(tank[i].Bomb == YELLOW)
					INT_BombTime[i] = 100/ YELLOW_TIME * tank[i].BombTime;

				if((tank[i].Bomb == BLACK && tank[i].BombTime == BLACK_TIME+tank[i].BombSleepTime)||
				   (tank[i].Bomb == RED && tank[i].BombTime == RED_TIME+tank[i].BombSleepTime)||
				   (tank[i].Bomb == YELLOW && tank[i].BombTime == YELLOW_TIME+tank[i].BombSleepTime)){
					INT_BombTime[i]=0;
					tank[i].BombSleep = false;
					tank[i].BombTime = 0;				
				}
			}
		}
		//포탄 발사 시 안내선을 위한 좌표변경	
		if(nIDEvent == MOVEANGEL){
			for(int i=0 ; i<2 ; i++)
				if(tank[i].Angel){
					if( tank[i].Sign)
						INT_MoveX[i]+=4;
					else
						INT_MoveX[i]-=4;
					//최소각도부터 시작되어 최대각도까지 표현되기 위해 MoveX[i]를 MaxAngel 나눈 나머지로 좌표값 표현
					//탱크의 방향도 고려
					if(tank[i].dx == true)
						tank[i].AngelCoords = CPoint(tank[i].Coords.x + 
							tank[i].Size*5*cos(-(RAD * (double)(tank[i].MinAngel + INT_MoveX[i]%tank[i].MaxAngel )))  ,  
							tank[i].Coords.y + tank[i].Size*5*sin(-(RAD* (double)(tank[i].MinAngel+INT_MoveX[i]%tank[i].MaxAngel ) )));
					else
						tank[i].AngelCoords = CPoint(tank[i].Coords.x + 
							tank[i].Size*5*cos(( RAD * (-180 + (double)(tank[i].MinAngel + INT_MoveX[i]%tank[i].MaxAngel ))))  ,  
													tank[i].Coords.y + 
							tank[i].Size*5*sin(( RAD * (-180 + (double)(tank[i].MinAngel + INT_MoveX[i]%tank[i].MaxAngel ) ))));

					if(INT_MoveX[i]-4 == tank[i].MaxAngel)
						tank[i].Sign = false;
					else if(INT_MoveX[i]+4 == 0)
						tank[i].Sign = true;
				}
			
		}



	//포탄의 발사
		if(nIDEvent==WEAPONBULLET)
			for(int i=0 ; i<INT_TankNum ; i++)		
				if(Bomb[i] != NULL){
				
					Bomb[i]->coords.x += Bomb[i]->vx;
					Bomb[i]->coords.y += Bomb[i]->vy;

				}


	if(nIDEvent==REALTIME)
		for(int i=0 ; i < INT_TankNum ; i++)
			if(Bomb[i] != NULL){
				Bomb[i]->vy+=Bomb[i]->t;
				Bomb[i]->vx+=WindForce;
			}
	if(nIDEvent == CHANGEWIND)
		if(OP_Wind==0)
			WindForce = rand()%3-1;
	
	
	if(nIDEvent == CLOUDCLK)
		if(OP_Wind==0 && WindForce!=0)
			MakeCloud();

	if(OP_GameWay == FLAG){
		if(nIDEvent == COMJUMP){
			for(int i=1 ; ( i< INT_TankNum )&& rand()%3 ; i++){
				
					tank[i].JumpLevel = tank[i].Coords.y;
					tank[i].JumpSleep = true;
					tank[i].Jump=true;
			}


		}


		if(nIDEvent == COMBULLET){
			for(int i=0 ; ( i < INT_TankNum )&& rand()%2; i++)
				MakeBullet(i);
							
		}

		if(nIDEvent == COMBOMB){


		}

	}

	Invalidate(false);
	
	CView::OnTimer(nIDEvent);
}


void GameView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	
	//타이틀 화면에서 작동
	 if(Title){
		if(nChar){
			Title = false;
			Main = true;
		}
	}

	//메인화면에서 작동
	else if(Main){
		if(nChar == VK_RETURN){
			if(INT_Order1 == 0)
				_Tank = true;
			else if(INT_Order1 ==1)
				Option = true;
			else if(INT_Order1 == 2){
				Info[1] = true;
				Info[2] = true;
			}
			else if(INT_Order1 == 3)
				End = true;
			Main = false;
			Title = false;
			INT_Order1 = INT_Order2= 0;
		}



		if(nChar == VK_UP){
			if(INT_Order1==0)	INT_Order1=(MENU_NUM-1);
			else	INT_Order1 -=1;
		
		}
		if(nChar == VK_DOWN){
			if(INT_Order1==MENU_NUM-1)	INT_Order1=0;
			else	INT_Order1 +=1;		}
		
	}

	//옵션화면에서 작동
	else if(Option){
		if(nChar == VK_UP){
			if(INT_Order1==0)	INT_Order1=(OPTION_NUM-1);
			else	INT_Order1 -=1;
		
		}
		if(nChar == VK_DOWN){
			if(INT_Order1==OPTION_NUM-1)	INT_Order1=0;
			else	INT_Order1 +=1;		}


		if(nChar == VK_RETURN){
			//대전방식
			if(INT_Order1==0){
				if(OP_MapWay==0) OP_MapWay=1;
				else if(OP_MapWay==1) OP_MapWay=2;
				else if(OP_MapWay==2) OP_MapWay=3;
				else OP_MapWay=0;
			}
			else if(INT_Order1==1){
				if(OP_GameWay==0)  OP_GameWay=1;
				else OP_GameWay=0;
			}
			//맵 방식
			else if(INT_Order1==2){
				if(OP_Map==0) OP_Map=1;
				else if(OP_Map==1) OP_Map=2; 
				else OP_Map=0;
			}
			//바람여부
			else if(INT_Order1==3){
				if( OP_Wind ==0) OP_Wind=1;
				else OP_Wind=0;
			}
			//뒤로가기
			else if(INT_Order1==4){
				INT_Order1=1;
				Option =false;
				Main = true;
			}
		}
			
	}

	//게임설명화면에서 작동
	else if(Info[1]){

		if(nChar && Info[2]){
			Info[2]=false;
			Info[3]=true;
		}
		else if(nChar && Info[3]){
			Info[1] = Info[2] = Info[3] = false;
			Main = true;
		}

	}

	//탱크선택화면에서 작동
	else if(_Tank){

		//1P
		if(!Sel1){
			if(nChar == VK_UP){
				if(INT_Order1==0)	INT_Order1=(TANK_NUM-1);
				else	INT_Order1 -=1;
		
			}
			else if(nChar == VK_DOWN){
				if(INT_Order1==TANK_NUM-1)	INT_Order1=0;
				else	INT_Order1 +=1;		
			}
		}
		//2P
		if(!Sel2 && OP_GameWay==PVP){
			if(nChar == 'W'){
				if(INT_Order2==0)	INT_Order2=(TANK_NUM-1);
				else	INT_Order2 -=1;
		
			}
			else if(nChar == 'S'){
				if(INT_Order2==TANK_NUM-1)	INT_Order2=0;
				else	INT_Order2 +=1;		
			}
		}
		if(nChar == VK_SPACE && !Sel2){
			Sel2=true;
		}
		if(nChar == VK_RETURN && !Sel1){
			Sel1=true;
		}
		
		if(OP_GameWay==PVP){
			if(Sel1 && Sel2){
				_Tank = false;
				Weapon = true;
				Sel1=false;	
				Sel2=false;	
				OP_Tank1 = INT_Order1;		//탱크지정
				OP_Tank2 = INT_Order2;
				INT_Order1 = INT_Order2 = 0;
			}
		}
		else if(OP_GameWay==FLAG)
			if(Sel1){
				_Tank = false;
				Weapon = true;
				Sel1=false;
				OP_Tank1 = INT_Order1;
				SelWeapon=false;
			}
	}
	else if(Weapon){
		//1P
		if(!Sel1){
			if(nChar == VK_UP){
				if(INT_Order1==0)	INT_Order1=(WEAPON_NUM-1);
				else	INT_Order1 -=1;
		
			}
			if(nChar == VK_DOWN){
				if(INT_Order1==WEAPON_NUM-1)	INT_Order1=0;
				else	INT_Order1 +=1;		
			}
		}
		//2P
		if(!Sel2 && OP_GameWay==PVP){
			if(nChar == 'W'){
				if(INT_Order2==0)	INT_Order2=(WEAPON_NUM-1);
				else	INT_Order2 -=1;
		
			}
			if(nChar == 'S'){
				if(INT_Order2==WEAPON_NUM-1)	INT_Order2=0;
				else	INT_Order2 +=1;		
			}
		}

		if(nChar == VK_SPACE && !Sel2){
			Sel2=true;
		}
		if(nChar == VK_RETURN && !Sel1){
			Sel1=true;
		}

		//총탄고르기
		if(!SelWeapon){
			if(OP_GameWay==PVP){
				if(Sel1 && Sel2){
					SelWeapon = true;
					Sel1=false;	
					Sel2=false;
					OP_Bullet1 = INT_Order1;
					OP_Bullet2 = INT_Order2;
					INT_Order1 = INT_Order2 = 0;
				}
			}
			else if(OP_GameWay==FLAG)
				if(Sel1){
					SelWeapon = true;
					Sel1=false;
					INT_Order1 = INT_Order2 = 0;
				}
		}
		//포탄고르기
		else{
			if(OP_GameWay==PVP){
				if(Sel1 && Sel2){
					SelWeapon = true;
					Start =true;
					Weapon = false;
					SetStart();
					OP_Bomb1 = INT_Order1;
					OP_Bomb2 = INT_Order2;
					Sel1=false;	
					Sel2=false;
				}
			}
			else if(OP_GameWay==FLAG)
				if(Sel1){
					Start=true;
					Weapon = false;
					OP_Bomb1 = INT_Order1;
					SetStart();
				}

		}
	}

	
	else if(Start && !End ){
		
			//1P//
		//점프, 낙하,점프쿨타임, 발사중에는 점프되지 않는다.
		if(nChar==VK_UP && !tank[0].JumpSleep && !tank[0].Jump && !tank[0].Fall && !tank[0].Angel){
			tank[0].JumpLevel = tank[0].Coords.y;
			tank[0].Jump=true;
			tank[0].JumpSleep = true;
			tank[0].JumpTime = 0;
		}
		//각도를 결정하거나 힘을 결정 하는 순간에 작동되지 않는다.
		if((nChar=='o' || nChar=='O') &&!tank[0].BombSleep && !tank[0].Jump && !tank[0].Fall && !tank[0].Angel && !tank[0].Power){
			INT_MoveX[0] = 0;
			tank[0].Fire = true;
			tank[0].Angel = true;
		}

		//각도나 힘을 결정 하는 키로만 사용
		if(nChar==VK_RETURN && (tank[0].Angel || tank[0].Power)){
			if(tank[0].Angel){
				tank[0].Angel = false;
				tank[0].Power = true;
				tank[0].Sign=true;
				tank[0].PowerCoords=0;
			}
			else if(tank[0].Power){
				SetGaugeRect1 = CRect(0,0,0,0);
				tank[0].Power = false;
				tank[0].Angel = false;
				tank[0].BombSleep = true;
				tank[0].Fire = false;
				tank[0].BombTime = 0;
				INT_BombTime[0] = 0;
				MakeBomb(0);
			}
		}

		//총탄 발사
		if((nChar=='p' || nChar=='P')&& !tank[0].BulletSleep &&!tank[0].Angel && !tank[0].Power){
			MakeBullet(0);
			tank[0].BulletTime = 0;
			tank[0].BulletSleep = true;
			INT_BulletTime[0] = 0;
		}


			//2P//
		if(OP_GameWay == PVP){
			//점프
			if((nChar=='w' || nChar=='W') && !tank[1].JumpSleep && !tank[1].Jump && !tank[1].Angel && !tank[1].Power ){
				tank[1].JumpLevel = tank[1].Coords.y;
				tank[1].JumpSleep =  true;
				tank[1].Jump = true;
				tank[1].JumpTime = 0;
			}
			//포탄발사
			
			//총탄발사
			if(((nChar=='b' || nChar=='B' )&& !tank[1].BulletSleep && !tank[1].Angel && !tank[1].Power)){
				MakeBullet(1);
				INT_BulletTime[1]=0;
				tank[1].BulletTime = 0;
				tank[1].BulletSleep = true;
			}

			if((nChar=='v' || nChar=='V')  && !tank[1].BombSleep && !tank[1].Jump && !tank[0].Fall && !tank[1].Angel && !tank[1].Power){
				INT_MoveX[1] = 0;
				tank[1].Fire = true;
				tank[1].Angel = true;

			}
			//결정하기
			if((nChar==VK_SPACE) && (tank[1].Angel || tank[1].Power)){
				if(tank[1].Angel){
					tank[1].Angel = false;
					tank[1].Power = true;
					tank[1].Sign = true;
					tank[1].PowerCoords = 0;
				}
				else if(tank[1].Power){

					SetGaugeRect2 = CRect(0,0,0,0);
					tank[1].Power = false;
					tank[1].Angel = false;
					tank[1].Fire = false;
					tank[1].BombSleep = true;
					tank[1].BombTime = 0;
					INT_BombTime[1] = 0;
					MakeBomb(1);
				}
			}			
		}
				
	

		if(nChar == VK_ESCAPE){
			if(SetBkSound){
				PlaySound(NULL, AfxGetInstanceHandle(),  NULL);
				SetBkSound=false;
			}
			else{
				SetBkSound = true;
				if(BkSound)
					PlaySound( MAKEINTERSOURCE(_BKWAV1), AfxGetInstanceHandle(), SND_ASYNC | SND_LOOP | SND_RESOURCE);
				else
					PlaySound( MAKEINTERSOURCE(_BKWAV2), AfxGetInstanceHandle(), SND_ASYNC | SND_LOOP | SND_RESOURCE);
			}
		}

	}


	if(End){
		if(nChar){
			Main = true;
			End = false;
			Start = false;
			GameEnd(0);
		}
	}
	
	DrawVirtual();

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}



void GameView::OnInitialUpdate()
{
	

	T_Main[0] = CRect(414, 432, 414+230, 442+70);
	T_Main[1] = CRect(414, 510, 414+230, 520+70);
	T_Main[2] = CRect(414, 590, 414+230, 595+70);
	T_Main[3] = CRect(414, 665, 414+230, 670+70);

	T_Option[0] = CRect(210, 328, 222+200, 328+80);
	T_Option[1] = CRect(210, 400, 222+200, 400+80);
	T_Option[2] = CRect(210, 480, 222+180, 480+80);
	T_Option[3] = CRect(210, 550, 222+200, 550+80);
	T_Option[4] = CRect(210, 625, 211+200, 620+85);

	T_Tank[0] = CRect(450,450,455+80,450+50);
	T_Tank[1] = CRect(450,500,450+150,500+65);
	T_Tank[2] = CRect(450,565,455+80,565+50);
	T_Tank[3] = CRect(450,615,450+80,615+60);
	
	T_Weapon1[0] = CRect(265, 505, 265+220, 505+50);
	T_Weapon1[1] = CRect(265, 555, 265+230, 555+50); 
	T_Weapon1[2] = CRect(265, 605, 265+230, 605+50); 

	T_Weapon2[0] = CRect(505, 500, 505+240, 505+50);
	T_Weapon2[1] = CRect(505, 550, 505+270, 555+50);
	T_Weapon2[2] = CRect(505, 600, 505+270, 605+50);

	T_MapWay[0] = CRect(430, 335, 430+90, 335+70);  
	T_MapWay[1] = CRect(555, 335, 550+65, 335+70);
	T_MapWay[2] = CRect(655, 335, 645+95, 335+70);
	T_MapWay[3] = CRect(780, 335, 775+135, 335+70);

	T_GameWay[0] = CRect(435, 415, 435+155, 415+70);
	T_GameWay[1] = CRect(640, 415, 645+190, 415+70);

	T_Map[0] = CRect(440, 495, 440+90, 495+67);
	T_Map[1] = CRect(565, 495, 565+150, 495+67);
	T_Map[2] = CRect(775, 495, 775+60, 495+67);

	T_Wind[0] = CRect(435, 565, 435+90, 565+70);
	T_Wind[1] = CRect(555, 565, 560+90, 565+70);


	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}



// 개별 함수들


bool GameView::SetStart(void)
{
	GetClientRect(&GameRect);

	Map = new GameMap();
	if(Map == NULL)
		return false;
	Map->SetMap();
	
	if(OP_GameWay == PVP){
		INT_TankNum = 2;
		tank = new Tank[INT_TankNum];
		SetTimer(PVP, 1, NULL);
	}
	else if(OP_GameWay == FLAG){
		INT_TankNum = 6;
		tank = new Tank[INT_TankNum];
		ComHpBkRect = new CRect[INT_TankNum];
		ComHpRect = new CRect[INT_TankNum];
		SetTimer(FLAG,1,NULL);
	}

	INT_BulletTime = new INT[2];
	INT_BombTime = new INT[2];
	INT_MoveX = new INT[2];

	Bomb = new BOMB*[INT_TankNum];
	Bullet = new BULLET*[INT_TankNum];
	BulletTail = new BULLET*[INT_TankNum];

	for(int i=0 ; i<INT_TankNum ; i++){
		if(tank == NULL)
			return false;
		tank[i].SetTank(i);

		Bullet[i] = new BULLET;
		BulletTail[i] = new BULLET;
		Bomb[i] = new BOMB;

		Bomb[i] = NULL;
		BulletTail[i] = Bullet[i] =  NULL;
	}


	INT_MoveX[0] = INT_MoveX[1] = INT_BulletTime[0] = INT_BulletTime[1] = INT_BombTime[0] = INT_BombTime[1] = 0;
	 
	tank[0].AngelCoords = CPoint(tank[0].Coords.x + 
							tank[0].Size*5*cos(-(RAD * (double)(tank[0].MinAngel + INT_MoveX[0]%tank[0].MaxAngel )))  ,  
							tank[0].Coords.y + tank[0].Size*5*sin(-(RAD* (double)(tank[0].MinAngel+INT_MoveX[0]%tank[0].MaxAngel ) )));

	tank[1].AngelCoords = CPoint(tank[1].Coords.x + 
							tank[1].Size*5*cos(-(RAD * (double)(tank[1].MinAngel + INT_MoveX[1]%tank[1].MaxAngel )))  ,  
							tank[1].Coords.y + tank[1].Size*5*sin(-(RAD* (double)(tank[1].MinAngel+INT_MoveX[1]%tank[1].MaxAngel ) )));


	SetGaugeRect1 = CRect(0,0,0,0);
	SetHealthRect1 = CRect(0,0,0,0);
	
	SetGaugeRect2 = CRect(0,0,0,0);
	SetHealthRect2 = CRect(0,0,0,0);
	

	SetTimer(GRAVITY,1,NULL);
	SetTimer(SLEEP, JUMPTIME, NULL);
	SetTimer(JUMP, 1, NULL);
	SetTimer(SHOOT, 10, NULL);
	SetTimer(WEAPONBULLET, BULLET_TIME, NULL);
	SetTimer(WEAPONBOMB,  1, NULL);
	SetTimer(REALTIME,100,NULL);
	SetTimer(CLOUDCLK,CLOUDTIME,NULL);
	SetTimer(MOVECLOUD,1,NULL);
	SetTimer(MOVEANGEL,1,NULL);
	SetTimer(CHANGEWIND,WINDTIME,NULL);
	SetTimer(COMJUMP, COMJUMPTIME , NULL);
	SetTimer(COMBULLET, COMBULLETTIME, NULL);
	SetTimer(COMBOMB, COMBOMBTIME, NULL);

	if(rand()%2){
		BkSound=true;
		PlaySound( MAKEINTERSOURCE(_BKWAV1), AfxGetInstanceHandle(), SND_ASYNC | SND_LOOP | SND_RESOURCE);
	}
	else {
		BkSound=false;
		PlaySound(MAKEINTERSOURCE(_BKWAV2), AfxGetInstanceHandle(), SND_ASYNC | SND_LOOP | SND_RESOURCE);
	}
	return true;
}

void GameView::PhysicalModel_Tank(void)
{

	

	for(int i=0 ; i < INT_TankNum ; i++){

		//맵 끝에 도달 시 진행불가
		if( tank[i].Coords.x-tank[i].Size< GameRect.left )
			tank[i].Coords.x = GameRect.left+tank[i].Size;
		else if(tank[i].Coords.x+tank[i].Size > GameRect.right )
			tank[i].Coords.x = GameRect.right-tank[i].Size;

		//맵 끝이 아닌경우에 나머지 검사 시행
		else{
			CRect InterRect;
			LONG H = tank[i].H = GetH(i);
			CheckEntry(i);					//좌표X 검사
			H = GetH(i);
			LONG X = tank[i].X;
			int Size = Map->Size;
			CPoint pt = tank[i].Coords;

					 
			if(0<=H && H<Map->HEIGHT ){
				//바닥감지
 				if( Map->Remain[H+1][X] == 0 && !tank[i].Jump)
					tank[i].Fall = true;


 				if(tank[i].Fall && !tank[i].Jump &&Map->Remain[H][X]!=0&&(Map->Coords[H][X].top < tank[i].Coords.y+tank[i].Size+10))
					//바닥 감지시 추락중지
					if(tank[i].Fall){
						tank[i].Coords.y = Map->Coords[H][X].top -tank[i].Size;
						tank[i].Fall = false;
						tank[i].H-=1;
					}
			}

		}
		
	}
}


LONG GameView::GetH(int TankNum)
{	//탱크의 바닥이 높이의 기준
	return (Map->HEIGHT-1 - (GameRect.bottom-(tank[TankNum].Coords.y+tank[TankNum].Size))/Map->Size);
}


void GameView::CheckEntry(int i)
{
	CRect InterRect; 
	if(!tank[i].EntryRight){
		//왼쪽(X-1)좌표로 진입
		if(!tank[i].EntryLeft && (tank[i].Coords.x-tank[i].Size < (tank[i].X*Map->Size)) ){
			//왼쪽에 벽이 있다면 이동불가
			if(Map->Remain[tank[i].H][tank[i].X-1])
				tank[i].Coords.x = Map->Coords[tank[i].H][tank[i].X-1].right+tank[i].Size;
			//벽이 없다면 진입하는 순간으로 부울대수 셋팅
			else{
				tank[i].EntryRight = false;
				tank[i].EntryLeft = true;
			}
		}

		else if(tank[i].EntryLeft){

			//왼쪽으로 진입이 완전히 끝남
			if(tank[i].Coords.x+tank[i].Size < tank[i].X*Map->Size ){
				tank[i].X-=1;
				tank[i].EntryLeft=false;
			}
			//왼쪽으로 진입하다 말고 다시 돌아옴(X->X-1->X)
			else if(tank[i].Coords.x-tank[i].Size > (tank[i].X-1)*Map->Size )
				tank[i].EntryLeft=false;	
		}
	}
	
	//오른쪽 진입
	if(!tank[i].EntryLeft){
		if( !tank[i].EntryRight && (tank[i].Coords.x+tank[i].Size) > (tank[i].X+1)*Map->Size ){
			if( Map->Remain[tank[i].H][tank[i].X+1])
				tank[i].Coords.x = Map->Coords[tank[i].H][tank[i].X+1].left-tank[i].Size;
			else{

				tank[i].EntryLeft = false;
				tank[i].EntryRight = true;

			}
		}

		else if(tank[i].EntryRight){
			if(tank[i].Coords.x-tank[i].Size > (tank[i].X+1)*Map->Size ){
				tank[i].X+=1;
				tank[i].EntryRight=false;
			}
			if(tank[i].Coords.x+tank[i].Size < (tank[i].X+1)*Map->Size )
				tank[i].EntryRight=false;
		}
		
	}
}



void GameView::PhysicalModel_Bullet(void){
	CRect InterRect;

	for(int i=0 ; i<INT_TankNum ; i++){
		BULLET *temp = Bullet[i];
		while(temp != NULL){
				temp->X = temp->coords.x/Map->Size;
				//총알이 화면 밖으로 나갈 시 없어짐
				if(temp->coords.x < GameRect.left){
					DestroyBullet(i,temp);
					break;
				}
				else if(temp->coords.x > GameRect.right){
					DestroyBullet(i,temp);
					break;
				}
				
				//총탄과 탱크의 충돌
				for(int l=0 ; l<INT_TankNum ; l++){
					if(i!=l && InterRect.IntersectRect( PtToRect(tank[l].Coords, tank[l].Size),
												PtToRect(temp->coords, temp->size+1))){
						CollisionBullet(l,i,temp->item);
						DestroyBullet(i,temp);
						goto exit;
					}
				}

				//총탄과 맵의 충돌
				if(InterRect.IntersectRect( PtToRect( temp->coords, temp->size+1), 
										Map->Coords[temp->H][temp->X]) != NULL){
					ShatterMap(temp);
					DestroyBullet(i,temp);
					break;
				}
			temp = temp->next;
		}
		
	}
	exit:;
}


void GameView::CollisionBomb(int i, int item){

	if(item == BLACK)
		tank[i].Health -= BLACK_DMG+tank[i].Defense+tank[i].Damage;
	if(item == RED)
		tank[i].Health -= RED_DMG+tank[i].Defense+tank[i].Damage;
	if(item == YELLOW)
		tank[i].Health -= YELLOW_DMG+tank[i].Defense+tank[i].Damage;

}


 void GameView::CollisionBullet(int i,int l, int item){
	//i는 충돌 된 탱크 l은 총알을 발사한 탱크

	if(OP_GameWay == PVP){
		if(item == CALIBER45)
			tank[i].Health -= CALIBER45_DMG+tank[i].Defense+tank[i].Damage;
		else if(item == CALIBER60)
			tank[i].Health -= CALIBER60_DMG+tank[i].Defense+tank[i].Damage;
		else if(item == CALIBER90)
			tank[i].Health -= CALIBER90_DMG+tank[i].Defense+tank[i].Damage;
	}
	else if(OP_GameWay == FLAG){
		if(i==0){
			if(item == CALIBER45)
				tank[0].Health -= CALIBER45_DMG+tank[0].Defense+tank[0].Damage;
			else if(item == CALIBER60)
				tank[0].Health -= CALIBER60_DMG+tank[0].Defense+tank[0].Damage;
			else if(item == CALIBER90)
				tank[0].Health -= CALIBER90_DMG+tank[0].Defense+tank[0].Damage;

		}
		else if(i!=0 && l==0){
			if(item == CALIBER45)
				tank[i].Health -= CALIBER45_DMG+tank[i].Defense+tank[i].Damage;
			else if(item == CALIBER60)
				tank[i].Health -= CALIBER60_DMG+tank[i].Defense+tank[i].Damage;
			else if(item == CALIBER90)
				tank[i].Health -= CALIBER90_DMG+tank[i].Defense+tank[i].Damage;
		}

	}
}



void GameView::ShatterMap(BULLET* node){
	CRect *temp = &Map->Coords[node->H][node->X];
	int *remain = &Map->Remain[node->H][node->X];
	if(*remain==0)
		;
	else if(*remain==1){
		*temp = CRect(0,0,0,0);
		*remain=0;
	}
	else{
		if(Map->Kind == WOOD){
		
				*temp = CRect(temp->left, temp->top, temp->right, temp->bottom-Map->Size/2);
		
			*remain-=1;
		}
		else if(Map->Kind == ASP){
		
				*temp = CRect(temp->left, temp->top, temp->right, temp->bottom - Map->Size /( *remain));

			*remain-=1;
	
		}
		
	}

}


void GameView::PhysicalModel_Bomb(void){

	for(int i=0 ; i<INT_TankNum ; i++){
		if(Bomb[i] != NULL){
			CRgn rgn, rgn1, rgn2;
			rgn.CreateRectRgn(0,0,1,1);
			int H = Bomb[i]->H = Map->HEIGHT-1-(GameRect.bottom - Bomb[i]->coords.y)/Map->Size;
			int X =	Bomb[i]->X = (Bomb[i]->coords.x / Map->Size);
			if(H>=0 && H<Map->HEIGHT){
	
				for(int l=0 ; l <INT_TankNum ; l++){
					
					rgn1.CreateRectRgn( tank[l].Coords.x  -tank[l].Size+2 , tank[l].Coords.y  -tank[l].Size+2 ,
										tank[l].Coords.x  +tank[l].Size+2 ,  tank[l].Coords.y  +tank[l].Size+2 );
					rgn2.CreateRectRgn( Bomb[i]->coords.x-Bomb[i]->size+2,Bomb[i]->coords.y-Bomb[i]->size+2,
										Bomb[i]->coords.x+Bomb[i]->size+2,Bomb[i]->coords.y+Bomb[i]->size+2);
					if(rgn.CombineRgn(&rgn1, &rgn2, RGN_DIFF) == COMPLEXREGION){
						CollisionBomb(i,tank[l].Bomb);
						DestroyBomb(i);
						break;
					}
							
					
					rgn1.DeleteObject();
					rgn2.DeleteObject();
				}
			
					rgn1.CreateEllipticRgn( Bomb[i]->coords.x-Bomb[i]->size,Bomb[i]->coords.y-Bomb[i]->size,
											Bomb[i]->coords.x+Bomb[i]->size,Bomb[i]->coords.y+Bomb[i]->size);
	
				//옆면충돌
				if(Map->Remain[H][X+1] != 0){
					rgn2.CreateRectRgn(Map->Coords[H][X+1].left, Map->Coords[H][X+1].top,
									Map->Coords[H][X+1].right, Map->Coords[H][X+1].bottom);
					if(rgn1.CombineRgn(&rgn1, &rgn2, RGN_DIFF) == COMPLEXREGION){
						DestroyBomb(i);
						Map->Coords[H][X+1] = CRect(0,0,0,0);
						Map->Remain[H][X+1] = 0;
						break;
					}
				}

				//아래와 충돌
				else if(Map->Remain[H+1][X] != 0){
					rgn2.CreateRectRgn(Map->Coords[H+1][X].left, Map->Coords[H+1][X].top,
									Map->Coords[H+1][X].right, Map->Coords[H+1][X].bottom);
					if(rgn1.CombineRgn(&rgn1, &rgn2, RGN_DIFF) == COMPLEXREGION){
						DestroyBomb(i);
						Map->Coords[H+1][X] = CRect(0,0,0,0);
						Map->Remain[H+1][X] = 0;
						break;
					}
				}
		

				if(Bomb[i]->coords.x < GameRect.left || Bomb[i]->coords.x > GameRect.right ||
					Bomb[i]->coords.y < GameRect.top || Bomb[i]->coords.y > GameRect.bottom){
					DestroyBomb(i);
						
					break;
				}
			}
			rgn.DeleteObject();
			rgn1.DeleteObject();
			rgn2.DeleteObject();

		}
		
						


	}
}



//i는 현재 탱크의 Index
bool GameView::MakeBullet(int i)
{
	if(Bullet[i] == NULL){
		Bullet[i] = new BULLET;
		Bullet[i]->next = NULL;
		Bullet[i]->pre = NULL;
		BulletTail[i] = Bullet[i];

		Bullet[i]->dx = tank[i].dx;
		Bullet[i]->item = tank[i].Bullet;
		Bullet[i]->coords.x = tank[i].Coords.x;
		Bullet[i]->coords.y = tank[i].Coords.y;
		Bullet[i]->X = tank[i].X;
		Bullet[i]->H = tank[i].H;
		if(Bullet[i]->item == CALIBER45)
			Bullet[i]->size = Map->Size/10;
		else if(Bullet[i]->item == CALIBER60)
			Bullet[i]->size = Map->Size/8;
		else if(Bullet[i]->item == CALIBER90)
			Bullet[i]->size = Map->Size/6;
		return true;
	}

	else{
		BULLET *temp = new BULLET;
		BulletTail[i]->next = temp;
		temp->next = NULL;
		temp->pre = BulletTail[i];
		BulletTail[i] = temp;
	
		temp->dx = tank[i].dx;
		temp->item = tank[i].Bullet;
		temp->X = tank[i].X;
		temp->H = tank[i].H;
		temp->coords.x = tank[i].Coords.x;
		temp->coords.y = tank[i].Coords.y;
		if(temp->item == CALIBER45)
			temp->size = Map->Size/10;
		else if(temp->item == CALIBER60)
			temp->size = Map->Size/8;
		else if(temp->item == CALIBER90)
			temp->size = Map->Size/6;

		return true;
	}
	
	return false;

}


bool GameView::DestroyBullet(int i, BULLET *Node)
{
	if(Node == NULL)
		return false;

	else if(Bullet[i] == Node&& Node->next == NULL)
		Node = BulletTail[i] = Bullet[i]= NULL;
	
	else if(Node == Bullet[i])
		Bullet[i] = Node->next;


	else if(Node == BulletTail[i]){
		BulletTail[i]->pre->next = NULL;
		BulletTail[i] = BulletTail[i]->pre;
	}
	else{
		Node->pre->next = Node->next;
		Node->next->pre = Node->pre;
	}
	delete Node;
	return true;

}


void GameView::MakeBomb(int i){

	if(Bomb[i] == NULL){
		Bomb[i] = new BOMB;
		if( tank[i].dx){
			Bomb[i]->coords.x = tank[i].Coords.x+tank[i].Size;
			Bomb[i]->coords.y = tank[i].Coords.y-tank[i].Size*2;
		}
		else{
			Bomb[i]->coords.x = tank[i].Coords.x-tank[i].Size;
			Bomb[i]->coords.y = tank[i].Coords.y-tank[i].Size*2;
		}
		Bomb[i]->dx = tank[i].dx;
		Bomb[i]->item=tank[i].Bomb;
		Bomb[i]->F = tank[i].PowerCoords/7;
		Bomb[i]->t=1;
		
		CalPhy(i);
	
		if(Bomb[i]->item == BLACK){
			Bomb[i]->size = Map->Size/4;
			Bomb[i]->col = RGB(0,0,0);
		}
		else if(Bomb[i]->item == RED){
			Bomb[i]->size = Map->Size/6;
			Bomb[i]->col = RGB(255,0,0);
		}
		else if(Bomb[i]->item == YELLOW){
			Bomb[i]->size = Map->Size/9;
			Bomb[i]->col = RGB(255,187,0);
		}
	}

}


void GameView::DestroyBomb(int i){
	
	if(Bomb[i] != NULL){
		delete Bomb[i];
		Bomb[i] = NULL;
	}
}


void GameView::CalPhy(int i){
	long F = Bomb[i]->F;
	long t = Bomb[i]->t;
	if(Bomb[i]->dx == true){
		int a =  (double)(tank[i].MinAngel + INT_MoveX[i]%tank[i].MaxAngel);
		Bomb[i]->co = cos(RAD * (double)(tank[i].MinAngel + INT_MoveX[i]%tank[i].MaxAngel ));
		Bomb[i]->si = -sin(RAD* (double)(tank[i].MinAngel+INT_MoveX[i]%tank[i].MaxAngel ));
	}
	else{
		Bomb[i]->co = cos(-( RAD * (-180 + (double)(tank[i].MinAngel + INT_MoveX[i]%tank[i].MaxAngel )))) ;
		Bomb[i]->si = -sin(-( RAD * (-180 + (double)(tank[i].MinAngel + INT_MoveX[i]%tank[i].MaxAngel ) )));
	}

	Bomb[i]->vx = (1.2*F*Bomb[i]->co)/4.0 + t + WindForce;
	Bomb[i]->vy = (1.2*F*Bomb[i]->si)/4.0 + t + WindForce;
}




void GameView::MakeCloud(void)
{
	if(Cloud == NULL){
		Cloud = new CLOUD;
		Cloud->next = Cloud->pre = NULL;
		CloudTail = Cloud;
		
		if(WindForce>0)
			Cloud->coords = CPoint(GameRect.left - 200 , GameRect.top + rand()%(GameRect.bottom-200)+100);
		else if(WindForce<0)
			Cloud->coords = CPoint(GameRect.right + 200 , GameRect.top + rand()%(GameRect.bottom-200)+100);	
		Cloud->tempy = Cloud->coords.y;
		Cloud->dy=false;
		Cloud->Levely = rand()%41+20;
	}

	else{
		CLOUD *temp = new CLOUD;
		CloudTail->next = temp;
		temp->next = NULL;
		temp->pre = CloudTail;
		CloudTail= temp;
		if(WindForce>0)
			temp->coords = CPoint(GameRect.left - 200 , GameRect.top + rand()%(GameRect.bottom-200)+100);
		else if(WindForce<0)
			temp->coords = CPoint(GameRect.right + 200 , GameRect.top + rand()%(GameRect.bottom-200)+100);	

		temp->tempy = temp->coords.y;
		temp->dy=false;
	}


}


void GameView::DestroyCloud(void)
{
	CLOUD *Node = Cloud;
	while(Node != NULL){
		if(GameRect.left-300>Node->coords.x || GameRect.right+300<Node->coords.x){
			if(Node == NULL)
				break;

			else if(Node==Cloud && Node->next == NULL){
				
				Node = CloudTail = Cloud = NULL;
			}
			else if(Node == Cloud)
				Cloud = Node->next;


			else if(Node == CloudTail){
				CloudTail->pre->next = NULL;
				CloudTail =CloudTail->pre;
			}
			else{
				Node->pre->next = Node->next;
				Node->next->pre = Node->pre;
			}
			delete Node;
			break;
		}
		
		Node = Node->next;
	}
}



BOOL GameView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if(Start){
		//1인용만 작동(FLAG)
		if(!tank[0].Fire){
			if( GetKeyState(VK_LEFT)<0){
				tank[0].Coords.x-=MOVESPD;	
				tank[0].dx = false;
			}
			if( GetKeyState(VK_RIGHT)<0){
				tank[0].Coords.x+=MOVESPD;
				tank[0].dx = true;
			}


		}
		//2인용을 위한 작동(PVP)
		if(OP_GameWay == PVP)
			if( !tank[1].Fire){
				if(  GetKeyState('a')<0 ||  GetKeyState('A')<0){
					tank[1].Coords.x-=MOVESPD;	
					tank[1].dx = false;
				}
				if(  GetKeyState('d')<0 || GetKeyState('D')<0){
					tank[1].Coords.x+=MOVESPD;
					tank[1].dx = true;
				} 
			}
		
		for(int i=0 ; i < INT_TankNum ; i++){

			//탱크의 움직임
			if(tank[i].JumpSleep && tank[i].Jump)
				tank[i].Coords.y-=JUMPSPD;
			if(tank[i].Fall)
				tank[i].Coords.y+=FALLSPD;

			//총알의 움직임
			BULLET *BulTemp = Bullet[i];
			while(BulTemp != NULL){
				if(BulTemp->dx==true)
					BulTemp->coords.x += BULLET1_SPD;
				else 
					BulTemp->coords.x -= BULLET1_SPD;

				BulTemp = BulTemp->next;
			}

			//포탄의 움직임
			if(Bomb[i] != NULL){
					Bomb[i]->coords.x += Bomb[i]->vx;
					Bomb[i]->coords.y += Bomb[i]->vy;
				
			}
	

			//각도 결정후 파워게이지 변경을 위한 좌표변경
			if((i==0 || i==1)&&tank[i].Power){
				if(tank[i].Sign)	
					tank[i].PowerCoords+=3;
				else
					tank[i].PowerCoords-=3;
	
				if(i==0){
					SetGaugeRect1 = CRect(GameRect.right - 325,  38 ,GameRect.right - 310 + tank[0].PowerCoords-2 ,40+29);
					
					if(tank[0].PowerCoords-1 >240)
						tank[0].Sign = false;
					else if(tank[0].PowerCoords-2 < 0)
						tank[0].Sign = true;
				}

				else if(i==1){
					SetGaugeRect2 = CRect(85 , 38	 , 90+tank[1].PowerCoords - 2 , 40+29);
					if(tank[1].PowerCoords-1 > 250)
						tank[1].Sign = false;
					else if(tank[1].PowerCoords-2 < 0)
						tank[1].Sign = true;

				}
				if(OP_GameWay == FLAG)
					SetGaugeRect1 = CRect(85 , 38	 , 90+tank[0].PowerCoords - 2 , 40+29);
			}

		CLOUD *temp = Cloud;
		while(temp != NULL){
			temp->coords.x +=WindForce;
			if(temp->dy==true)
				temp->coords.y+=1;
			else
				temp->coords.y-=1;

			if(temp->coords.y < temp->tempy-40)
				temp->dy=true;
			else if(temp->coords.y > temp->tempy+30)
				temp->dy=false;

			temp= temp->next;
		}

		}

	}
	return CView::PreTranslateMessage(pMsg);
}


void GameView::CheckDeadTank(void)
{
	if(OP_GameWay == PVP){
		for(int i=0 ; i < 2; i++)
			//체력이 0이되거나 절벽으로 떨어지면 파괴
			if(tank[i].Coords.y+tank[i].Size > GameRect.bottom || tank[i].Health<0)
				if(i==0 || i==1)
					End=true;
		
	}
	else{
	for(int i=0 ; i < INT_TankNum; i++)
		//체력이 0이되거나 절벽으로 떨어지면 파괴
		if(tank[i].Coords.y+tank[i].Size > GameRect.bottom || tank[i].Health<0){
			if(i==0)
				End = true;

			INT_TankNum--;
		}
	}
}


void GameView::GameEnd(int TankNum=0)
{
	
	Cloud = CloudTail = NULL;
	Option=  _Tank= Weapon= Sel1=Sel2= SelWeapon = false;
	
	if(SetBkSound)
		PlaySound(NULL, AfxGetInstanceHandle(),  NULL);

	for(int i=0 ; i<INT_TankNum ; i++){

		delete [] Bomb[i];
		delete [] Bullet[i];
		delete [] BulletTail[i];

	}

	delete Map;
	delete [] tank;
	delete [] INT_MoveX;
	delete [] INT_BulletTime;
	delete [] INT_BombTime;
	


	INT_TankNum=0;
}




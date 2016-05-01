#include "StdAfx.h"
#include "resource.h"
#include "MainFrm.h"
#include "Map.h"
#include "Tank.h"

Tank::Tank(){

	
}


Tank::~Tank(void)
{
	TankBmp.DeleteObject();

}

void Tank::SetTank(int x){
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	GameView *pView = (GameView *)pFrame->GetActiveView();
	GameMap *Map = pView->Map;

	Num = x;
	
	Size =pView->Map->Size/TANKSIZE;
	


	if(x==0){
		Bullet = pView->OP_Bullet1;
		Bomb = pView->OP_Bomb1;
		Kind = pView->OP_Tank1;
		SetBitmap(Kind);
	}
	else if(x==1 && pView->OP_GameWay==PVP){
		Bullet = pView->OP_Bullet2;
		Bomb = pView->OP_Bomb2;
		Kind = pView->OP_Tank2;
		SetBitmap(Kind);
	}
	else{
	
		Bullet = rand()%WEAPON_NUM;
		Bomb = rand()%WEAPON_NUM;
		Kind = rand()%TANK_NUM;
		SetBitmap(Kind);
	}
	
	EntryLeft = EntryRight = Jump =  Angel = dx = EntryLeft = EntryRight = false;
	Fire = Landing = JumpSleep = Power = BulletSleep = BombSleep =  false;
	if(x==1)
		dx=true;

	Sign = Fall = true;
	PowerCoords = JumpTime= BulletTime = BombTime = 0;
	Health=100;
	Speed=1;


	
	if(Kind==TANK1){
		Name = CString( _T("Loto"));
		MinAngel = 30;
		MaxAngel = 60;
		Damage = 0;
		Defense = 0;
		JumpDegree = 0;
		JumpSleepTime = 3;
		Speed = 0;
		BulletSleepTime = 0;
		BombSleepTime = 0;
	}

	else if(Kind==TANK2){
		Name = CString( _T("MrBig"));
		MinAngel = 30;
		MaxAngel = 75;
		Damage = 5;
		Defense = -4;
		JumpDegree = -5;
		JumpSleepTime = 5;
		Speed = 0;
		BulletSleepTime = -1;
		BombSleepTime = -2;
	}

	if(Kind==TANK3){
		Name = CString( _T("Java"));
		MinAngel = 15;
		MaxAngel = 45;
		Damage = -2;
		Defense = 5;
		JumpDegree= -3;
		JumpSleepTime = 5;
		Speed = 0;
		BulletSleepTime = +3;
		BombSleepTime = +4;
	}

	if(Kind==TANK4){
		Name = CString( _T("Knu"));
		MinAngel = 0;
		MaxAngel = 90;
		Damage= -4;
		Defense = -3;
		JumpDegree = 2;
		JumpSleepTime = 1;
		Speed = 1;
		BulletSleepTime = -2;
		BombSleepTime = -5;
	}


	if(pView->OP_GameWay == PVP){
		if(x==0)
			Coords = CPoint( Map->Size*(WIDTH-3)- Size-5,pView->GameRect.bottom - 300);
		
		else	
				Coords = CPoint( Map->Size*3 - Size-5, 	pView->GameRect.bottom - 300);
	}
	if(pView->OP_GameWay == FLAG){
		if(x==0)
			Coords = CPoint( rand()%100+100, rand()%200+100);
		
		else 
			Coords = CPoint(  Map->Size*(WIDTH/2+x*3)- Size-5, pView->GameRect.bottom -200);
		dx=false;
	}
	AngelCoords = CPoint(Coords.x , Coords.y);
	X = Coords.x/ pView->Map->Size;		//현재 탱크가 위치한 열(배열로 왼쪽으로 부터 x-1째 칸)

}

void Tank::SetBitmap(int Tank){

	if(Tank==TANK1)
		TankBmp.LoadBitmapW(_TANK1);
	else if(Tank==TANK2)
		TankBmp.LoadBitmapW(_Tank2);
	else if(Tank==TANK3)
		TankBmp.LoadBitmapW(_TANK3);
	else if(Tank==TANK4)
		TankBmp.LoadBitmapW(_TANK4);

	TankBmp.GetBitmap(&TankInfo);
}
#include "StdAfx.h"
#include "MainFrm.h"
#include "Map.h"



GameMap::GameMap()
	
{


}


GameMap::~GameMap(void)
{

}

void GameMap::SetMap(){

	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();
	GameView *pView = (GameView *)pFrame->GetActiveView();
	GameRect = pView->GameRect;

	//¸Ê ÁÂÇ¥ »ý¼º
	Size = GameRect.Width()/WIDTH;
	Kind = pView->OP_Map;
	MapWay = pView->OP_MapWay;
	Wind=0;
	

	if(Kind ==0)
		Kind = WOOD;
	else if(Kind==1)
		Kind = ASP;
	else if(Kind==2)
		Kind = METAL;
	
	if(MapWay==0){
		MapWay=FLAT;
		HEIGHT=10;
	}
	else if(MapWay==1){
		MapWay=MOUNTAIN;	
		HEIGHT=18;
	}
	else if(MapWay==2){
		HEIGHT=10;
		MapWay=TRENCH;	
	}
	else if(MapWay==3){
		HEIGHT=10;
		MapWay=RANDOM;
	}


	int i=0;
	if(MapWay == RANDOM){
		
		for(int l=0 ; l <WIDTH*MAPSIZE ; l++){
			Height[l] = HEIGHT/2 - (rand()%BLOCKNUM-RANDBLOCK);
			int j=1;
			for(i=HEIGHT-1 ; i > HEIGHT - Height[l] ; i--){
				Coords[i][l] = CRect( l*Size, GameRect.bottom - Size*j,
										(l+1)*Size, GameRect.bottom - Size*(j-1));

				Remain[i][l] = Kind;	
				j++;
			}
			for(int k=i ; k>=0 ; k--)
				Remain[k][l] = 0;
		}
	}
	else if(MapWay == FLAT){
		for(int l=0 ; l <WIDTH*MAPSIZE ; l++){
			Height[l] = HEIGHT/2;
			int j=1;
			for(i=HEIGHT-1 ; i > HEIGHT - Height[l] ; i--){
				Coords[i][l] = CRect( l*Size, GameRect.bottom - Size*j,
										(l+1)*Size, GameRect.bottom - Size*(j-1));

				Remain[i][l] = Kind;	
				j++;
			}
			for(int k=i ; k>=0 ; k--)
				Remain[k][l] = 0;
		}
	}
	else if(MapWay == TRENCH){
		for(int l=0 ; l <WIDTH*MAPSIZE ; l++){
			Height[l] = HEIGHT/2 - rand()%2;
			int j=1;
			for(i=HEIGHT-1 ; i > HEIGHT - Height[l] ; i--){
				Coords[i][l] = CRect( l*Size, GameRect.bottom - Size*j,
										(l+1)*Size, GameRect.bottom - Size*(j-1));

				Remain[i][l] = Kind;	
				j++;
			}
			for(int k=i ; k>=0 ; k--)
				Remain[k][l] = 0;
		}
	}
	
	else if(MapWay == MOUNTAIN){
		int k=3,p=HEIGHT;
		for(int l=0 ; l <WIDTH*MAPSIZE ; l++){
			if(k<HEIGHT)
				Height[l] = k++;
			else if(k>=HEIGHT)
				Height[l] = p--;
			int j=1;
			for(i=HEIGHT-1 ; i > HEIGHT - Height[l] ; i--){
				Coords[i][l] = CRect( l*Size, GameRect.bottom - Size*j,
										(l+1)*Size, GameRect.bottom - Size*(j-1));

				Remain[i][l] = Kind;	
				j++;
			}
			for(int k=i ; k>=0 ; k--)
				Remain[k][l] = 0;
		}
	}

	

}
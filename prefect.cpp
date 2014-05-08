//prefect.cpp

/*******
*
*  
compile in windows (mingw):
   g++ -mwindows -lmingw32 prefect.cpp -o bin\prefect.exe sdlmain.gnu.lib Xinput.lib CRM32Pro.gnu.lib -DWINDOWS

compile in linux:
   g++ -w -O3 -s -pthread prefect.cpp -o bin/prefect /usr/CRM32Pro/CRM32Pro.so -I/usr/CRM32Pro -L/usr/X11/lib -lX11 -lXext -DLINUX
   (or just type 'make')
   

big TODOS:

We are at inMnu==2 - handle response for attack
need windows
need linux


cleanup 2 tiles, 2 fonts, 1 sprite sofar
*
*
********/
#include <set>
#ifdef LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>

#define MAX_AXIS 16
#define MAX_BUTTON 16


//struct JS_DATA_TYPE {
// int buttons;    /* immediate button state */
// int x;          /* immediate x axis value */
// int y;          /* immediate y axis value */
//};
// #define JS_RETURN       sizeof(struct JS_DATA_TYPE)


struct padData {
  unsigned char axisCount;
  unsigned char buttonCount;
  int fd;
  int version;
  char devName[80];
  int aPos[MAX_AXIS];
  int bPos[MAX_BUTTON];
  bool changed;
  js_event ev;
};
struct JS_DATA_TYPE js;

padData pad;
#endif
#ifdef WINDOWS
void  UpdateControllerState();
#endif


#include "CRM32Pro.h"
#include "prefect.h"
#include <time.h>


int inGrid=1;
int redrawBase=0;
int isSnap=-1;
int actTile=0;
int gridSwitch=1;
int lastinfo=0;
int iTmp;
int has_joystick=0;
int result;
int btnAprs=0;
int btnBprs=0;
int update=1;
int mnIdx=1;
int mxIdx=3;
int truicox, truicoy, tester, icox, icoy;
  set<int> draws[NUM_PLANES];
  set<int> alives;	

//SDL_Surface *bg;
//CRM32Pro_CFont;
//CRM32Pro_CSprite *sprBat;
//SDL_Rect inf;
void RenderInCombat(int bLogicUpdate);
void DrawMap(void);
void updateInfo(void);
void setEnvironment(void);
void KillCreature(int cIDX);
void KillAllCreatures(void);
void PopulateCreatures(void);
//int areUsure(char *question);
//int doMenu(int mnVar, int mnTyp);

void CreateCreature(int crType, int pOwner, int crPosZ, int crPosY, int crPosX);
void SetTileType(int wPln, int wRow, int wCol);

int PosToGridX(int tmpX){  
  int  retval=(tmpX-MAP_OFFSET_X)/64;
  return retval;
}
int GridToPosX(int tmpX){
  int retval=(tmpX*64)+MAP_OFFSET_X;
  return retval;
}

int PosToGridY(int tmpY){  
  int  retval=(tmpY-MAP_OFFSET_Y)/64;
  return retval;
}
int GridToPosY(int tmpY){
  int retval=(tmpY*64)+ MAP_OFFSET_Y;
  return retval;
}


void RenderInGame(int bLogicUpdate)
{
    // Check if we need to render everything.
if(!CRM32Pro.IsRenderNeeded() && (!bLogicUpdate)) return; 
 CRM32Pro.Blit(bg,NULL,CRM32Pro.screen,NULL);	
 CRM32Pro.Blit(info,NULL,infoB,NULL);
 if ((inGrid >0) && (gridSwitch>0)){
   grid=IImage->Load(GFX_RESOURCE,"reggrid");
   gridSwitch=0;
 }
 if (gridSwitch>0){
   CRM32Pro.FreeSurface(grid);
   CRM32Pro.Blit(bg,NULL,CRM32Pro.screen,NULL);
   sCursor.spr->Draw();
   gridSwitch=0;
 }
 if (inGrid >0 ){	
   CRM32Pro.Blit(grid,NULL,CRM32Pro.screen,NULL);
 }
 
 // draw sprites
 
 
 for (iTmp=0; iTmp < MAX_ICONS; iTmp++){
   if ((creatures[iTmp].alive==1) && (creatures[iTmp].draw==1)){creatures[iTmp].spr->Draw();
   }}
 
 sCursor.spr->Draw();	
 actTile=Earth_map0[prefect.mpPln][sCursor.row][sCursor.col];
 int crtindex=Creat_map0[prefect.mpPln][sCursor.row][sCursor.col];
 //		char strTmp[100]= {'\0'};
 //		char strRgt[255]={'\0'};
 
 if (update==1){
   SetTileType(0, sCursor.col, sCursor.row);
   sprintf(infopanel.line1,"Environment Type = %s (%d)",sCursor.underTile,actTile);
   if (crtindex>1){
     sprintf(infopanel.line2,"Occupied by %s, Health %d of %d",creatures[crtindex].creatureName, creatures[crtindex].NRG, creatures[crtindex].maxhealth);
     
     infopanel.icon=creatures[crtindex].cKind;
     
   }else{
     sprintf(infopanel.line2,"Unoccupied Area");
     infopanel.icon=0;
   }
   if (prefect.crsMd>0){
     sprintf(infopanel.line3,"Currently Moving idx %d, a %s", prefect.mpIdx, creatures[prefect.mpIdx].creatureName);
   }
 }
   updateInfo();
 
 int pstx, psty;
 
 /*
   if ((lastinfo + 250) < ITimeSystem->GetTime()){
   redrawBase++;
   lastinfo = ITimeSystem->GetTime();
   if (redrawBase>5){
   
   //		  			CRM32Pro.Blit(bg, NULL, CRM32Pro.screen,NULL);
   redrawBase=0;
   }
   //	  CRM32Pro.Blit(bg,NULL,CRM32Pro.screen,NULL);
   //		CRM32Pro.Blit(info,NULL,infoB,NULL);
   
   /*
   actTile=Earth_map0[0][sCursor.col][sCursor.row];
   char strTmp[100];
   SetTileType(sCursor.col, sCursor.row);
   sprintf(strTmp,"Environment Type = %s (%d) POS = x:%d y:%d",sCursor.underTile,actTile,sCursor.col,sCursor.row);
   fFont->PutString(infoB,15,5,strTmp);
   CRM32Pro.Blit(infoB,NULL,CRM32Pro.screen,&inf);
   }
 */
 
 
}

void LoadGraphics(){
  info = IImage->Load(GFX_RESOURCE,"infopanel");
  infoB = IImage->Load(GFX_RESOURCE,"infopanel");
  inf.y= 600 - info->h;
  inf.x= 0;
  inf.h= info->h;
  inf.w= info->w;
  bg = IImage->Load(GFX_RESOURCE,"map");
  grid=IImage->Load(GFX_RESOURCE,"reggrid");
  sCursor.spr = new CRM32Pro_CSprite();
  sCursor.spr->Load(GFX_RESOURCE,"cursorA");
  sCursor.spr->AutoRestore(1);
  Earth.tile = new CRM32Pro_CTile();
  Earth.tile->Load(GFX_RESOURCE,"tileset");
  Earth.status = new CRM32Pro_CTile();
  Earth.status->Load(GFX_RESOURCE,"creatures");
  fFont = new CRM32Pro_CFont();
  fFont->Load(GFX_RESOURCE,"fontA");
}

void FreeGraphics(){
  delete sCursor.spr;
  CRM32Pro.FreeSurface(bg);
  delete Earth.tile;
  delete Earth.status;
  KillAllCreatures();
  CRM32Pro.FreeSurface(grid);
  //ICursor->Delete(cCursor);
}

void GameLoop(){
  int pstx, psty;
  IScreenFX->FadeToImage(bg,1000);
  Uint8       done = 0;
  LoadGraphics();
  SDL_Event event;
  DrawMap();
  sCursor.spr->SetPosition(300,400);
  CRM32Pro.SetRenderCallback(RenderInGame);
  CRM32Pro.CleanUp();
  // As we are using smooth sprite movement, we set the render engine to draw always
  CRM32Pro.RenderNeeded(UPDATEFRAME_ALWAYS);
  CRM32Pro.Blit(bg,NULL,CRM32Pro.screen,NULL);
  while(!done){

  
    //do any cpu stuff here?

    int tmpx=sCursor.spr->GetPosX();
    int tmpy=sCursor.spr->GetPosY();
    tmpx=tmpx+32;
    tmpy=tmpy+32;
    sCursor.row=PosToGridY(tmpy);
    sCursor.col=PosToGridX(tmpx);


   
if (prefect.inCbt==1){
//combat time

}


    if ((prefect.mnCmd==1) && (prefect.inMnu==0)){
		prefect.mnCmd=0;

		int tmpMover=Creat_map0[prefect.mpPln][sCursor.row][sCursor.col];
		Creat_map0[prefect.mpPln][sCursor.row][sCursor.col]=0;
		int xpos=sCursor.col; int ypos=sCursor.row;
		delete sCursor.spr;
		creatures[tmpMover].draw=0;
		delete creatures[tmpMover].spr;		

sCursor.spr=new CRM32Pro_CSprite();
		sCursor.spr->Load(GFX_RESOURCE,creatures[tmpMover].imageName); 
		sCursor.spr->SetPosition(GridToPosX(xpos),GridToPosY(ypos));
      		sCursor.spr->SetAlpha(200);						     
		

		prefect.crsMd=1;
		prefect.mpIdx=tmpMover;
	      }
#ifdef WINDOWS
//check game state, react when necessary.


//player input
UpdateControllerState();
if (prefect.joys[prefect.playerturn -1] == 1){
XINPUT_STATE tmpSt=g_Controllers[prefect.playerturn -1].state;
XINPUT_STATE oldSt=g_Controllers[prefect.playerturn -1].lastState;


// we've got input
float LX = tmpSt.Gamepad.sThumbLX;
float LY = tmpSt.Gamepad.sThumbLY;
LY=LY * -1.0f;


float magnitude = sqrt(LX*LX + LY*LY);

//determine the direction the controller is pushed
float normalizedLX = LX / magnitude;
float normalizedLY = LY / magnitude;

float normalizedMagnitude = 0;

//check if the controller is outside a circular dead zone
if (magnitude > INPUT_DEADZONE)
{
  //clip the magnitude at its expected maximum value
  if (magnitude > 32767) magnitude = 32767;
  
  //adjust magnitude relative to the end of the dead zone
  magnitude -= INPUT_DEADZONE;

  //optionally normalize the magnitude with respect to its expected range
  //giving a magnitude value of 0.0 to 1.0
  normalizedMagnitude = magnitude / (32767 - INPUT_DEADZONE);
}
else //if the controller is in the deadzone zero out the magnitude
{
	magnitude = 0.0;
	normalizedMagnitude = 0.0;
}

//ILogSystem.Msg(LOG_NORMAL,"lx:%f ly:%f nlx:%f nly%f m:%f nm:%f",LX,LY,normalizedLX,normalizedLY,magnitude,normalizedMagnitude);
if ((prefect.inMnu==0) && (prefect.inCbt==0) && (magnitude>0)){
int icox, icoy;
	int truicox,truicoy;
	//	int speedmod=1000;	
	icox=sCursor.spr->GetPosX();
	icoy=sCursor.spr->GetPosY();

	icox=icox + (int)(10*normalizedLX);
	icoy=icoy + (int)(10*normalizedLY);

	
	if (icoy < 45) icoy=45;
	if (icoy > 429) icoy=429;
	if (icox < 45) icox=45;
	if (icox > 686) icox=686;
	
	truicox=PosToGridX(icox+32);
	truicoy=PosToGridY(icoy+32);
	
	
	sCursor.row=truicoy;sCursor.col=truicox;
	//use truicoy for snap-to-grid, icoy otherwise
	//DONE : toggle
	if (isSnap >0){
	  sCursor.spr->SetPosition(truicox, truicoy,1); 
					  
	}else{
	  sCursor.spr->SetPosition(icox, icoy,1);
	}

} else if ((prefect.inMnu==1) && (prefect.inCbt==0)){
	  // menu type 
	  int mnVar=Creat_map0[prefect.mpPln][sCursor.row][sCursor.col];
	  
	  sprintf(infopanel.line1,"Your orders, %s? :",prefect.player1);
	  sprintf(infopanel.line2,"Move %s",creatures[mnVar].creatureName);
	  sprintf(infopanel.line3,"Button 1) Confirm  : Button 2) Cancel");
	  infopanel.icon=creatures[mnVar].cKind;
	  
  if (LX >128){
	      if (mnIdx==mxIdx){
		mnIdx=1;
	      }else{
		mnIdx++;
	      }
	    }
	    
	    //up
	    if (LX <0){
	      if (mnIdx==1){
		mnIdx=mxIdx;
	      }else{
		mnIdx--;
	      }
	    }
	    
	      SDL_Delay(125);
	    switch(mnIdx){
	    case OPT_MOVE:
	      sprintf(infopanel.line2,"Move, Special::Infinity");
	      break;
	    case OPT_CAST:
	      sprintf(infopanel.line2,"Cast a spell (nothing yet!)");
	      break;
	    case OPT_CANCEL:
	sprintf(infopanel.line2,"Nevermind...");
	break;
	    default:
	      sprintf(infopanel.line2,"Oops! Menu Error!");
	      break;
	    }
	    //  SDL_Delay(50);
	    	    updateInfo();
	    if ((tmpSt.Gamepad.wButtons & XINPUT_GAMEPAD_A) && (btnAprs==0)){
	      //	return mnIdx;
	      btnAprs=1;
	      prefect.mnCmd=mnIdx;
	      update=1;
	      prefect.inMnu=0;
	      sprintf(infopanel.line3,"Turn 0 : %s is plotting...",prefect.player1);

	    }
	    if (tmpSt.Gamepad.wButtons & XINPUT_GAMEPAD_B){
	      prefect.mnCmd=0;
	      update=1;
	      prefect.inMnu=0;
	      sprintf(infopanel.line3,"Turn 0 : %s is plotting...",prefect.player1);
	    }
				
				
}

//if ((tmpSt.Gamepad.wButtons & XINPUT_GAMEPAD_A) && (btnAprs==0) && (prefect.inCbt==0)){
//button press A





//btnAprs=1;
//}
if (!(tmpSt.Gamepad.wButtons & XINPUT_GAMEPAD_A)) btnAprs=0;

	if ((tmpSt.Gamepad.wButtons & XINPUT_GAMEPAD_A) && (btnAprs==0) && (prefect.inMnu==0)){
	      btnAprs=1;
	  int tester=prefect.crsMd;
	  // SDL_Delay(50);
	  if ((tester==0) && (prefect.inCbt==0)){
	    if (Creat_map0[prefect.mpPln][sCursor.row][sCursor.col]>0){
	      //      int tmpRslt=doMenu(Creat_map0[prefect.mpPln][sCursor.row][sCursor.col],1);
	      update=0;
	      mnIdx=1;
	      prefect.inMnu=1;

	      //	      SDL_Delay(150);
	    }
	  
	  }      
		else if ((tester==1) && (prefect.inCbt==0)){
	  btnAprs=1;	    
	    //test to see if nigger is there, execute combat... 
	  int offx=sCursor.col; int offy=sCursor.row;	    
	  if (Creat_map0[prefect.mpPln][offy][offx] >0){
	    int otherIdx=Creat_map0[prefect.mpPln][offy][offx];
	    update=0;
	    prefect.inMnu=2;
	    if (creatures[prefect.mpIdx].owner == creatures[otherIdx].owner){
	      sprintf(infopanel.line1,"You cannot challenge yourself!");
	      prefect.inMnu=0;
	      SDL_Delay(250);
	      update=0;
	    }else{
	    sprintf(infopanel.line1,"Do you wish to challenge this foe? ");
	    sprintf(infopanel.line2,"idx: %d : a %s",otherIdx,creatures[otherIdx].creatureName);
	    sprintf(infopanel.line3,"1 - Attack    2 - Retreat");
	    
	    //	    updateInfo();
	    }
	  }else if (Creat_map0[prefect.mpPln][offy][offx]==0){

	    int tmpMover=prefect.mpIdx;
	    creatures[tmpMover].draw=1;
	    delete sCursor.spr;
	    sCursor.spr =new CRM32Pro_CSprite();
	    sCursor.spr->Load(GFX_RESOURCE,"cursorA");
	    sCursor.spr->SetPosition(GridToPosX(offx), GridToPosY(offy));
	    sCursor.spr->SetAlpha(255);
	    creatures[tmpMover].spr=new CRM32Pro_CSprite();
	    creatures[tmpMover].spr->Load(GFX_RESOURCE,creatures[tmpMover].imageName);
	    Creat_map0[prefect.mpPln][sCursor.row][sCursor.col]=tmpMover;
	    prefect.crsMd=0;
	    creatures[tmpMover].spr->SetPosition(GridToPosX(sCursor.col),GridToPosY(sCursor.row)) ;
	    creatures[tmpMover].spr->SelectAnim(SPRSTATE_NORMAL);
	    //   sCursor.spr->Draw();
	    //creatures[tmpMover].spr->Draw();
	    sprintf(infopanel.line3,"%s is thinking..",prefect.player1);
	  }}

	}	  

}


#endif //WINDOWS
	
	
	


#ifdef LINUX
    result = read(pad.fd, &js, JS_RETURN);
    if (result > 0)
      {
	
	int icox, icoy;
	int truicox,truicoy;
	//	int speedmod=1000;
	
	icox=sCursor.spr->GetPosX();
	icoy=sCursor.spr->GetPosY();
	if ((js.x > 200) && (prefect.inMnu==0)) {
	  icox= icox +10;
	 	  if (sCursor.state != SPRSTATE_RIGHT){
		    sCursor.spr->SelectAnim(SPRSTATE_RIGHT);
		    sCursor.state=SPRSTATE_RIGHT;
		  }
	} else if ((js.x < 100) && (prefect.inMnu==0)) {
	  icox= icox -10;
	  if (sCursor.state != SPRSTATE_LEFT){
	    sCursor.spr->SelectAnim(SPRSTATE_LEFT);
	    sCursor.state = SPRSTATE_LEFT;
	  }
	}

	if ((js.y >200 ) && (prefect.inMnu==0))        icoy=icoy +10;
	if ((js.y <100 ) && (prefect.inMnu==0))        icoy=icoy -10;

	if ((js.y==128)&&(js.x==128)&&(prefect.inMnu==0)){
 sCursor.spr->SelectAnim(SPRSTATE_NORMAL);
 sCursor.state=SPRSTATE_NORMAL;
 sCursor.spr->SelectFrame(1);
	}
	if (icoy < 45) icoy=45;
	if (icoy > 429) icoy=429;
	if (icox < 45) icox=45;
	if (icox > 686) icox=686;
	truicox=PosToGridX(icox+32);
	truicoy=PosToGridY(icoy+32);
	
	
	sCursor.row=truicoy;sCursor.col=truicox;
	//use truicoy for snap-to-grid, icoy otherwise
	//DONE : toggle
	if (isSnap >0){
	  sCursor.spr->SetPosition(truicox, truicoy,1); 
					  
	}else{
	  sCursor.spr->SetPosition(icox, icoy,1);
	}

	if ((js.buttons & 1) && (prefect.inMnu==2) &&(btnAprs==0)){
	  //setup combat
	  tester=Creat_map0[prefect.mpPln][sCursor.row][sCursor.col];
	  combat.in=1;
	  combat.fght1=prefect.mpIdx;
	  combat.fght2=tester;
	  combat.own1=creatures[prefect.mpIdx].owner;
	  combat.own2=creatures[tester].owner;
	  prefect.cbtIdxA=prefect.mpIdx;
	  prefect.cbtIdxB=tester;
	  prefect.inCbt=1;
	  btnAprs=1;
	}
	
	  // handle button 1
	if (js.buttons & 4){
	    //HERE
	  CreateCreature(CREATURE_BAT,0,prefect.mpPln,sCursor.row,sCursor.col);
	//  SDL_Delay(200);
	}
	if (js.buttons & 8){
	  //THERE
	  ILogSystem.Msg(LOG_NORMAL,"z: %d  - col: %d - row: %d",prefect.mpPln,sCursor.col,sCursor.row);
	}

	
	if ((js.buttons & 1) && (btnAprs==0) && (prefect.inMnu==0)){
	      btnAprs=1;
	  int tester=prefect.crsMd;
	  // SDL_Delay(50);
	  if ((tester==0) && (prefect.inCbt==0)){
	    if (Creat_map0[prefect.mpPln][sCursor.row][sCursor.col]>0){
	      //      int tmpRslt=doMenu(Creat_map0[prefect.mpPln][sCursor.row][sCursor.col],1);
	      update=0;
	      mnIdx=1;
	      prefect.inMnu=1;

	      //	      SDL_Delay(150);
	    }
	  
	  }      
		else if ((tester==1) && (prefect.inCbt==0)){
	  btnAprs=1;	    
	    //test to see if nigger is there, execute combat... 
	  int offx=sCursor.col; int offy=sCursor.row;	    
	  if (Creat_map0[prefect.mpPln][offy][offx] >0){
	    int otherIdx=Creat_map0[prefect.mpPln][offy][offx];
	    update=0;
	    prefect.inMnu=2;
	    if (creatures[prefect.mpIdx].owner == creatures[otherIdx].owner){
	      sprintf(infopanel.line1,"You cannot challenge yourself!");
	      prefect.inMnu=0;
	      SDL_Delay(250);
	      update=0;
	    }else {

	    sprintf(infopanel.line1,"Do you wish to challenge this foe? ");
	    sprintf(infopanel.line2,"idx: %d : a %s",otherIdx,creatures[otherIdx].creatureName);
	    sprintf(infopanel.line3,"1 - Attack    2 - Retreat");
	
	    //	    updateInfo();
	    }
	  }else if (Creat_map0[prefect.mpPln][offy][offx]==0){

	    int tmpMover=prefect.mpIdx;
	    creatures[tmpMover].draw=1;
	    delete sCursor.spr;
	    sCursor.spr =new CRM32Pro_CSprite();
	    sCursor.spr->Load(GFX_RESOURCE,"cursorA");
	    sCursor.spr->SetPosition(GridToPosX(offx), GridToPosY(offy));
	    sCursor.spr->SetAlpha(255);
	    creatures[tmpMover].spr=new CRM32Pro_CSprite();
	    creatures[tmpMover].spr->Load(GFX_RESOURCE,creatures[tmpMover].imageName);
	    Creat_map0[prefect.mpPln][sCursor.row][sCursor.col]=tmpMover;
	    prefect.crsMd=0;
	    creatures[tmpMover].spr->SetPosition(GridToPosX(sCursor.col),GridToPosY(sCursor.row)) ;
	    creatures[tmpMover].spr->SelectAnim(SPRSTATE_NORMAL);
	    //   sCursor.spr->Draw();
	    //creatures[tmpMover].spr->Draw();
	    sprintf(infopanel.line3,"%s is thinking..",prefect.player1);
	  }}

	}	  
       
  
	if (!(js.buttons & 1)){
	  btnAprs=0;
	}				}
	//ILogSystem.Msg(LOG_NORMAL,"js.x :%d ; js.y :%d",js.x,js.y);


	if (prefect.inMnu==1){ 
	  // menu type 
	  int mnVar=Creat_map0[prefect.mpPln][sCursor.row][sCursor.col];
	  
	  sprintf(infopanel.line1,"Your orders, %s? :",prefect.player1);
	  sprintf(infopanel.line2,"Move %s",creatures[mnVar].creatureName);
	  sprintf(infopanel.line3,"Button 1) Confirm  : Button 2) Cancel");
	  infopanel.icon=creatures[mnVar].cKind;
	  result = read(pad.fd, &js, JS_RETURN);
	  if (result > 0){
	    
	    //	    if (!js.buttons & 1){
	    //btnAprs=0;
	    //}
	    
      //down
	    if (js.x >200){
	      if (mnIdx==mxIdx){
		mnIdx=1;
	      }else{
		mnIdx++;
	      }
	    }
	    
	    //up
	    if (js.x <100){
	      if (mnIdx==1){
		mnIdx=mxIdx;
	      }else{
		mnIdx--;
	      }
	    }
	    
	      SDL_Delay(125);
	    switch(mnIdx){
	    case OPT_MOVE:
	      sprintf(infopanel.line2,"Move, Special::Infinity");
	      break;
	    case OPT_CAST:
	      sprintf(infopanel.line2,"Cast a spell (nothing yet!)");
	      break;
	    case OPT_CANCEL:
	sprintf(infopanel.line2,"Nevermind...");
	break;
	    default:
	      sprintf(infopanel.line2,"Oops! Menu Error!");
	      break;
	    }
	    //  SDL_Delay(50);
	    	    updateInfo();
	    
	    if ((js.buttons & 1) && (btnAprs==0)){
	      //	return mnIdx;
	      btnAprs=1;
	      prefect.mnCmd=mnIdx;
	      update=1;
	      prefect.inMnu=0;
	      sprintf(infopanel.line3,"Turn 0 : %s is plotting...",prefect.player1);

	    }
	    if (js.buttons & 2){
	      prefect.mnCmd=0;
	      update=1;
	      prefect.inMnu=0;
	      sprintf(infopanel.line3,"Turn 0 : %s is plotting...",prefect.player1);
	    }
	    if (!(js.buttons & 1)){
	      btnAprs=0;
	    }
	    
	  }
	  updateInfo();
	} //end of menu type 1
	
	if ((js.buttons & 1) && (btnAprs==0) && (prefect.inMnu==2)){
	  combat.start=1;
	  btnAprs=1;
	  prefect.inMnu=0;
	  
 	}
	if ((js.buttons & 2) && (prefect.inMnu==2)){
	  combat.start=0;
	  prefect.inMnu=0;
	  update=1;
 	}
	

	CRM32Pro.RenderNeeded();
    
#endif  //LINUX
				  
    
    while(CRM32Pro.Update(&event)){
      switch(event.type){
      case SDL_KEYDOWN:
	if(event.key.keysym.sym == SDLK_ESCAPE) done = 1;
	if(event.key.keysym.sym == SDLK_F1){ inGrid = inGrid * (-1); gridSwitch=1;};
//	if(event.key.keysym.sym == SDLK_F2) isSnap = isSnap * (-1);
	if(event.key.keysym.sym == SDLK_F3){
	  CreateCreature(CREATURE_SHADOWLORD,1,prefect.mpPln,sCursor.row,sCursor.col);
	}
	if(event.key.keysym.sym == SDLK_F4){
	  CreateCreature(CREATURE_KAIN,1,prefect.mpPln,sCursor.row,sCursor.col);
	}
	
	if (event.key.keysym.sym == SDLK_F6){
		CreateCreature(CREATURE_MAMMOTH,1,prefect.mpPln,sCursor.row,sCursor.col);
		}
	
	if(event.key.keysym.sym == SDLK_F5){
	  CreateCreature(CREATURE_BAT,1,prefect.mpPln,sCursor.row,sCursor.col);
	}
	
	
	break;
#ifdef MOUSECTRL	
      case SDL_MOUSEMOTION:
		int icox, icoy;
		int truicox,truicoy;
	sCursor.spr->GetOffset(&icox, &icoy);
	icox=event.motion.x - icox / 2;
	icoy=event.motion.y - icoy / 2;
	
	truicox=icox / 64;
	truicoy = icoy /64;
	pstx=(icox+32)/64;
	psty=(icoy+32)/64;
	sCursor.row=psty;sCursor.col=pstx;
	truicox=truicox *64 +MAP_OFFSET_X;
	truicoy=truicoy *64 +MAP_OFFSET_Y;
	//use truicoy for snap-to-grid, icoy otherwise
	//DONE : toggle
	if (isSnap >0){
	  sCursor.spr->SetPosition(truicox, truicoy,1); 
	  
	}else{
	  sCursor.spr->SetPosition(icox + MAP_OFFSET_X, icoy + MAP_OFFSET_Y,1);
	}
	//rCursor.y = event.motion.y - rCursor.h / 2;
	// As we have moved the sprite that follows the cursor, we order a render update
	
	
	CRM32Pro.RenderNeeded();
	break;
#endif //MOUSECTRL


      case SDL_QUIT:
	done = 1;
	break;
      case EVENT_LOGICWAIT:
	
	
	
	break;
      default:
	
	break;
      }
    }
      }
  }  



int main(int argc,char *argv[])
{
  
  Uint8 done;
  int   iRet;
  int   tmpX, tmpY, tmpZ;
  for (iTmp=0; iTmp < MAX_ICONS; iTmp++) creatures[iTmp].alive=0;
  
  for (tmpX=0; tmpX != NUM_COLS; tmpX++){
    for (tmpY=0; tmpY != NUM_ROWS; tmpY++){
      for (tmpZ=0; tmpZ != NUM_PLANES; tmpZ++){
	Earth_map0[tmpZ][tmpY][tmpX]=3;
	Creat_map0[tmpZ][tmpY][tmpX]=0;
      }}}


  prefect.playerturn=1;
  prefect.inCbt=0;
 prefect.cbtIdxA=0;
 prefect.cbtIdxB=0;
 prefect.crsMd=0;
 prefect.mpPln=0;
 prefect.mpIdx=0;
 prefect.inMnu=0;
 sprintf(prefect.player1,"k0rd");
 sprintf(prefect.player2,"player 2");
 sprintf(prefect.player3,"player 3");
 sprintf(prefect.player4,"player 4");
 PopulateCreatures();
 
 // -Init log system-
 ILogSystem.Init("prefect.log",LOG_FILE,LOG_NORMAL,PREFECT_VERSION);
 sprintf(infopanel.line1,"Welcome To Prefect !");
 sprintf(infopanel.line2,"Welcome To Prefect !");
 sprintf(infopanel.line3,"Welcome To prefect !");
 infopanel.icon=0;
   
   if(CRM32Pro.Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_AUDIO ) < 0)

     {
       ILogSystem.Msg(LOG_NORMAL," ·[PREFECT Init Error] - Couldn´t initialize CRM32Pro: %s\n",SDL_GetError());
       exit(1);
     }
 
   // -Desired config-
 if(!CRM32Pro.LoadConfig("setup.dpf","setup.xml"))
   {
     ILogSystem.Msg(LOG_NORMAL," ·[PREFECT Init Error] - Couldn´t load 'setup.xml'. Please, run 'Setup.exe'\n");
     exit(1);
   }
 CRM32Pro.Config.Title = PREFECT_VERSION;
 //  CRM32Pro.Config.Icon = IImage->Load(GFX_RESOURCE,"icono");
 
 // -Graphics system initialize-  
 if(!CRM32Pro.SetVideoMode())
   {
     CRM32Pro.Quit();
     return 1;
   }
 ICursor->Init();
 
 // -Time system initialize-
 ITimeSystem->Init();
 ITimeSystem->SetRate(0,0);  // Optional: desired Rendering and Logic Frame Rate
 // -Init sound system-
 iRet = ISoundFX->Init(); // It uses the config of CRM32Pro.Config
 if(iRet == -1) ILogSystem.Msg(LOG_NORMAL," ·[PREFECT Sound System] - Couldn´t init sound system: %s\n",SDL_GetError());	
 
#ifdef LINUX
 pad.fd= open("/dev/input/js0", O_RDONLY);
 if (pad.fd >0)
   {
     ILogSystem.Msg (LOG_NORMAL," ?[PREFECT joystick system] - JOYSTICK 0 detected!\n");
     has_joystick++;
   }
#endif //LINUX
 
#ifdef WINDOWS
ZeroMemory( g_Controllers, sizeof(CONTROLER_STATE)*MAX_CONTROLLERS );   

XInputEnable(true);
DWORD dwResult;  
int connected=1;  
char tmpMsg[255];
for (DWORD i=0; i< MAX_CONTROLLERS; i++ )
{
  XINPUT_STATE state;
  ZeroMemory( &state, sizeof(XINPUT_STATE) );

        // Simply get the state of the controller from XInput.
        dwResult = XInputGetState( i, &state );

        if( dwResult == ERROR_SUCCESS )
  { 
        sprintf(tmpMsg,"Joystick connected for player %d\n",connected);
		prefect.joys[connected -1]=1;
  }
        else
  {
            sprintf(tmpMsg,"Joystick not found for player %d\n",connected); 
			prefect.joys[connected -1]=0;
  }
  ILogSystem.Msg(LOG_NORMAL,"%s",tmpMsg);
  
	connected++;
}

#endif //WINDOWS

 
 
 // else if(iRet == 1) LoadAudio();
 
 
 /* ISoundFX->SoundPlay(1,thunder,0);
    sGameBg = IImage->Load(GFX_RESOURCE,"logotipoMS");   
    SDL_FillRect(CRM32Pro.screen,NULL,SDL_MapRGB(CRM32Pro.screen->format,0,0,0));
    IScreenFX->FadeToImage(sGameBg,500);    
    while(ISoundFX->IsPlaying(1)) SDL_Delay(50); 
    IScreenFX->FadeToColor(0,0,0,500);
    SDL_FreeSurface(sGameBg);*/
 
 // -Loading gfx-

 LoadGraphics();
 
   GameLoop();
   
   
   //ISoundFX->MusicFadeOut(500);
   SDL_Delay(250);
   // ISoundFX->MusicStop();
   IScreenFX->FadeToColor(0,0,0,500);
   //FreeAudio();
   FreeGraphics();   
   // CRM32Pro.AudioInfo();
   CRM32Pro.VideoInfo();
   CRM32Pro.Quit();
   ILogSystem.Msg(LOG_NORMAL,"\n · End of execution\n");
   
   
   
   return 1;
   
   
   
   }
void SetTileType(int wPln, int wRow, int wCol){
  char TileName[50];
  char strTmpz[50];
  sprintf(strTmpz,"Unknown");
   switch (Earth_map0[wPln][wRow][wCol]){
   case 1:
     sprintf(strTmpz,"Water");
     break;
   case 2:
     sprintf(strTmpz,"Island");
     break;  
   case 3:
     sprintf(strTmpz,"Desert");
     break;
   case 4:
     sprintf(strTmpz,"Grassland");
     break;
   case 5:
     sprintf(strTmpz,"Quicksand");
     break;
   }
   
   sprintf(sCursor.underTile,"%s",strTmpz);
   
   
   
}
   
void DrawMap(void) {
  //water tilemap test
  srand (time(NULL));
  //lets make 2 random islands for now
  int islandX , islandY, rawr;
  
  for(int posX = 0; posX < NUM_COLS; posX++) {
    for(int posY = 0; posY < NUM_ROWS; posY++) {
      Earth.tile->SetPosition(GridToPosX(posX), GridToPosY(posY));

      Earth.tile->Draw(bg, Earth_map0[prefect.mpPln][posY][posX]);
      
      
      // Earth.tile->Draw(bg, 1);
      
      
      
    }
  }
  //  for(rawr = 0; rawr < NUM_ISLANDS; rawr++){
  // islandX=rand() %NUM_ROWS +1;
  // islandY=rand() %NUM_COLS +1;
  // Earth.tile->SetPosition(PosToGridX(islandX), PosToGridY(islandY));
  //		 Earth.tile->Draw(bg, 1);
  //		 Earth_map0[prefect.mpPln][islandY][islandX]=1;
  // }
}

void CreateCreature(int crType, int pOwner, int crPosZ, int crPosY, int crPosX){
  int found=0;
// ignoring everything except plotting the creature for now
  if (Creat_map0[crPosZ][crPosY][crPosX] > 0){
      return;

    }  

  
  for (iTmp=0; (iTmp < MAX_ICONS); iTmp++){
    
    if (creatures[iTmp].alive==0){
      found=iTmp;
      
    }
  }
  if (found==0){
  }else{
    delete sCursor.spr;
    creatures[found].col=crPosX;
    creatures[found].row=crPosY;
    creatures[found].alive=1;
    
    switch (crType){
    case CREATURE_SHADOWLORD:
      creatures[found].spr=new CRM32Pro_CSprite();
      creatures[found].spr->Load(GFX_RESOURCE, "shadowlord");
      sprintf(creatures[found].imageName,"shadowlord");
      Creat_map0[crPosZ][crPosY][crPosX]=found;
      creatures[found].cKind=CREATURE_SHADOWLORD;
      creatures[found].draw=1;
      sprintf(creatures[found].creatureName,"Shadowlord");
      draws[crPosZ].insert(found);
      alives.insert(found);
      break;
    case CREATURE_KAIN:
      creatures[found].spr=new CRM32Pro_CSprite();
      creatures[found].spr->Load(GFX_RESOURCE, "kain");
      sprintf(creatures[found].imageName,"kain"); 
      creatures[found].cKind=CREATURE_KAIN;
      creatures[found].draw=1;
      Creat_map0[crPosZ][crPosY][crPosX]=found;
      sprintf(creatures[found].creatureName,"Kain");
      draws[crPosZ].insert(found);
      alives.insert(found);

    break;
    case CREATURE_BAT:
      creatures[found].spr=new CRM32Pro_CSprite();
      creatures[found].spr->Load(GFX_RESOURCE, "bat");
      creatures[found].cKind=CREATURE_BAT;
      sprintf(creatures[found].imageName,"bat");
      creatures[found].draw=1;
      Creat_map0[crPosZ][crPosY][crPosX]=found;
      sprintf(creatures[found].creatureName,"Bat");
      draws[crPosZ].insert(found);
      alives.insert(found);

	//  creatures[found].spr->SelectAnim(SPRSTATE_LEFT);
      break;
	  
	  case CREATURE_MAMMOTH:
	  creatures[found].spr=new CRM32Pro_CSprite();
	  creatures[found].spr->Load(GFX_RESOURCE, MAMMOTH_IMAGE);
	  sprintf(creatures[found].imageName,MAMMOTH_IMAGE);
	  creatures[found].draw=1;
	        Creat_map0[crPosZ][crPosY][crPosX]=found;
      sprintf(creatures[found].creatureName,"Wooly Mammoth");
      draws[crPosZ].insert(found);
      alives.insert(found);
      
    }
    creatures[found].owner=pOwner;
    creatures[found].keeper=pOwner;
    creatures[found].spr->SetPosition((crPosX * 64) + MAP_OFFSET_X ,(crPosY * 64)+ MAP_OFFSET_Y);
    creatures[found].spr->AutoRestore(0);
    creatures[found].spr->Draw();
    sCursor.spr= new CRM32Pro_CSprite();
    sCursor.spr->Load(GFX_RESOURCE,"cursorA");
    sCursor.spr->SetPosition((crPosX * 64) + MAP_OFFSET_X ,(crPosY * 64)+ MAP_OFFSET_Y);
  }
  
  
}

void updateInfo(void){
  
  //updateInfo();
  if (infopanel.icon>0){  

    
    Earth.status->SetPosition(725,5);
    Earth.status->Draw(infoB,infopanel.icon);
  }
  fFont->PutString(infoB,17,5,infopanel.line1);
  fFont->PutString(infoB,17,20,infopanel.line2);
  fFont->PutString(infoB,17,35,infopanel.line3);
  CRM32Pro.Blit(infoB,NULL,CRM32Pro.screen,&inf);

  
}
#ifdef WINDOWS
void  UpdateControllerState()   
 {   
     for (DWORD i=0; i< MAX_CONTROLLERS; i++ )   
     {   
         g_Controllers[i].lastState = g_Controllers[i].state;   
         g_Controllers[i].dwResult = XInputGetState( i, &g_Controllers[i].state );
//		 if (g_Controllers[i].dwResult==ERROR_SUCCESS){
//			g_Controllers[i].isActive=1;
//			}else{
//			g_Controllers[i].isActive=0;
//			}
     }   
}   
#endif //WINDOWS

void KillCreature(int cIDX){
delete creatures[cIDX].spr;
creatures[cIDX].alive=0;
creatures[cIDX].draw=0;
creatures[cIDX].NRG=0;
alives.erase(cIDX);
draws[prefect.mpPln].erase(cIDX);
}

void KillAllCreatures(void){
set<int>::iterator it;
it=alives.begin();
while (it != alives.end()){
KillCreature(*it);
it++;
}}

void PopulateCreatures(void){

bat.health=BAT_HEALTH;
sprintf(bat.image,BAT_IMAGE);
bat.speed=BAT_SPEED;

}

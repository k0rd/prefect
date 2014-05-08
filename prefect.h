//prefect.h
#include "attacks.h"
#include "maps.h"
#include "creatures.h"
#include "menu.h"
#include <iostream>
#include <cmath>
#ifdef WINDOWS
#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30
#define INPUT_DEADZONE 7849
#include "xinput.h"
#define MAX_CONTROLLERS 4
#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "XInput.lib")
struct _wincontrollers{
	XINPUT_STATE state;
	
	} control[3];
 struct CONTROLER_STATE   
 {   
     XINPUT_STATE     lastState;          
     XINPUT_STATE     state;          
     DWORD            dwResult;       
     bool             bLockVibration;   
     XINPUT_VIBRATION vibration;   
	 int isActive;
 };   
    
 CONTROLER_STATE g_Controllers[MAX_CONTROLLERS];   
#endif
#define GFX_RESOURCE "prefect.dpf"
// even - devel - odd - stable; major.minor.revision
#define PREFECT_VERSION "Prefect ALPHA 0.2.2"
SDL_Rect rRect;
SDL_Rect rCursor, inf;
SDL_Surface *bg, *grid, *info, *infoB;
//SDL_Surface *bg, *sRestoreBg[2];
// SDL_Surface *sMenuBg, *sMenuTitle, *sMenuInfo;
CRM32Pro_CFont *fFont;
//Uint32 iAux = 0;
//Uint32 cCursor;

//char sCad[10];
struct _prefect {
	int playerturn, cbtIdxA, cbtIdxB, inCbt, crsMd, mpPln, mpIdx, inMnu, mnCmd, joys[4];
	char player1[50];	
	char player2[50];
	char player3[50];
	char player4[50];
} prefect;

struct _infopanel{
  char line1[255];
  char line2[255];
  char line3[255];
  int icon;
}infopanel;

struct _combat{
  int in;
  int fght1;
  int fght2;
  int start;
  int winner;
  int own1;
  int own2;
}combat;

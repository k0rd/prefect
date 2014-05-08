//creatures.h
//#include <vector>



//BAT
#define CREATURE_BAT          1
#define BAT_HEALTH            60
#define BAT_IMAGE             "bat"
#define BAT_SPEED             1.2f

struct _bat{
  int health;
  char image[10];
  float speed;
} bat;


//SHADOWLORD
#define CREATURE_SHADOWLORD   2
#define SHADOWLORD_HEALTH     200
#define SHADOWLORD_IMAGE      "shadowlord"
#define SHADOWLORD_SPEED      1.2f


//KAIN
#define CREATURE_KAIN         3
#define KAIN_HEALTH           1000
#define KAIN_IMAGE            "kain"
#define KAIN_SPEED            1.0f

//MAMMOTH
#define CREATURE_MAMMOTH      4
#define MAMMOTH_HEALTH 		  150
#define MAMMOTH_IMAGE		  "mammoth"
#define MAMMOTH_SPEED         0.8f


struct _cursor {
  int col,row;
  int tX, tY;
  int tX2, tY2;
  float speed;
  char underTile[50];
  CRM32Pro_CSprite *spr;
  int state;
} sCursor;



struct _creature {
  int col,row,plane;
  int tX, tY;
  int tX2, tY2;
  char creatureName[100];
  Uint8 TypesA;
  Uint8 TypesB;
  Uint8 TypesC;
  int cKind;
  float speed;
  int alive;
  int draw;
  int endurance, starthealth, maxhealth;
  int NRG;
  int owner, keeper;
  char imageName[100];
  CRM32Pro_CSprite *spr;
} creatures[MAX_ICONS];

//vector <_creature> creatures;



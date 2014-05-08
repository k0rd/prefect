//maps.h

#define MAP_OFFSET_Y 45     //x offset of our grid
#define MAP_OFFSET_X 45		//y offset of our grid
#define NUM_ISLANDS 10		//number of islands (water plane)
#define NUM_COLS 11			//x
#define NUM_ROWS 7			//y
#define NUM_PLANES 8		//z
#define MAX_ICONS 480       // the number of characters that are allowed.



#define IS_PASSIBLE 0x00000001
#define IS_WATER    0x00000002
#define IS_AIR      0x00000004
#define IS_FIRE     0x00000008
#define IS_OCCUPIED 0x00000016
#define IS_PPOINT   0x00000032
#define IS_PORT_E   0x00000064
#define IS_PORT_W   0x00000128
#define IS_PORT_A   0x00000256
#define IS_PORT_F   0x00000512
#define IS_PORT_V   0x00001024


struct MapA {
//we need to combine to one big tileset
    CRM32Pro_CTile* tile;    //terrain
	CRM32Pro_CTile* status;  //status
    int posX, posY;
    int tX, tY;
} Earth; // The whole world

int Earth_map0[NUM_PLANES][NUM_ROWS][NUM_COLS];
int Creat_map0[NUM_PLANES][NUM_ROWS][NUM_COLS];

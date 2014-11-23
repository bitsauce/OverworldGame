#ifndef CONSTANTS_H
#define CONSTANTS_H

#define CANVAS_WIDTH 800
#define CANVAS_HEIGHT 600

#define UI_MENU_BUTTON_FONT "Arial Bold 27"
#define UI_MENU_BUTTON_TEXTURE ":/sprites/gui/menu_button.png"

#define MAX_LOADED_CHUNKS 128

// TERRAIN CONSTANTS
#define BLOCK_PX 16
#define BLOCK_PXF float(BLOCK_PX)

#define BORDER_PX 4
#define BORDER_PXF float(BORDER_PX)

#define FULL_TILE_PX (BLOCK_PX + BORDER_PX*2)
#define FULL_TILE_PXF float(FULL_TILE_PX)

#define CHUNK_BLOCKS 16
#define CHUNK_BLOCKSF float(CHUNK_BLOCKS)
#define CHUNK_PX (CHUNK_BLOCKS*BLOCK_PX)
#define CHUNK_PXF float(CHUNK_PX)

#define SUPER_CHUNK_CHUNKS 512
#define SUPER_CHUNK_CHUNKSF float(SUPER_CHUNK_CHUNKS)
#define SUPER_CHUNK_TILES (SUPER_CHUNK_CHUNKS*CHUNK_BLOCKS)
#define SUPER_CHUNK_TILESF float(SUPER_CHUNK_TILES)
#define SUPER_CHUNK_PX (SUPER_CHUNK_CHUNKS*CHUNK_BLOCKS*BLOCK_PX)
#define SUPER_CHUNK_PXF float(SUPER_CHUNK_PX)

// Tile texture coordinates
/*const float TILE_U0 = 0.000f;
const float TILE_V0 = 0.000f;
const float TILE_U1 = BORDER_PXF/FULL_TILE_PXF;
const float TILE_V1 = BORDER_PXF/FULL_TILE_PXF;
const float TILE_U2 = 0.500f;
const float TILE_V2 = 0.500f;
const float TILE_U3 = (TILE_PXF + BORDER_PXF)/FULL_TILE_PXF;
const float TILE_V3 = (TILE_PXF + BORDER_PXF)/FULL_TILE_PXF;
const float TILE_U4 = 1.000f;
const float TILE_V4 = 1.000f;
grid<float> TILE_TEXTURE_COORDS =
{
	{ TILE_U0, TILE_V0, TILE_U1, TILE_V1 }, // 0
	{ TILE_U1, TILE_V0, TILE_U3, TILE_V1 }, // 1
	{ TILE_U3, TILE_V0, TILE_U4, TILE_V1 }, // 2
	{ TILE_U3, TILE_V1, TILE_U4, TILE_V3 }, // 3
	{ TILE_U3, TILE_V3, TILE_U4, TILE_V4 }, // 4
	{ TILE_U1, TILE_V3, TILE_U3, TILE_V4 }, // 5
	{ TILE_U0, TILE_V3, TILE_U1, TILE_V4 }, // 6
	{ TILE_U0, TILE_V1, TILE_U1, TILE_V3 }, // 7
	{ TILE_U1, TILE_V1, TILE_U3, TILE_V3 }  // 8
};*/

enum BlockID
{
	NULL_TILE = 0,
	EMPTY_TILE,
	RESERVED_TILE,
	
	// BACKGROUND_TILES BEGIN
	WOOD_TILE,
	STONE_WALL,
	// BACKGROUND_TILES END
	
	BACKGROUND_TILES,
	
	// SCENE_TILES BEGIN
	GRASS_TILE,
	STONE_TILE,
	// SCENE_TILES END
	
	SCENE_TILES,
	
	// FOREGROUND_TILES BEGIN
	LEAF_TILE,
	// FOREGROUND_TILES END
	
	FOREGROUND_TILES,
	
	MAX_TILES
};

// INVENTORY CONSTANTS
const int INV_WIDTH = 9;
const int INV_HEIGHT = 3;

// Drawing layers
enum Layer
{
	LAYER_BACKGROUND,
	LAYER_SCENE,
	LAYER_FOREGROUND,
	LAYER_GUI,
	LAYER_UITEXT,
	LAYER_COUNT
};

// Terrain layers
enum TerrainLayer
{
	TERRAIN_LAYER_BACKGROUND,
	TERRAIN_LAYER_SCENE,
	TERRAIN_LAYER_FOREGROUND,
	TERRAIN_LAYER_COUNT
};

// Tile directions
enum Direction
{
	NORTH		= 1,
	NORTH_EAST 	= 2,
	EAST		= 4,
	SOUTH_EAST 	= 8,
	SOUTH		= 16,
	SOUTH_WEST 	= 32,
	WEST		= 64,
	NORTH_WEST 	= 128,
	NESW = NORTH | EAST | SOUTH | WEST
};

extern class UiObject *canvas;
class MainMenuScene;
class WorldSelectScene;
class WorldCreateScene;
class GameScene;
namespace scene
{
	extern MainMenuScene *mainMenu;
	extern WorldSelectScene *worldSelect;
	extern WorldCreateScene *worldCreate;
	extern GameScene *game;
}

#endif // CONSTANTS_H
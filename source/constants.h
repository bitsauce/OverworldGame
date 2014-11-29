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

#define FULL_BLOCK_PX (BLOCK_PX + BORDER_PX*2)
#define FULL_BLOCK_PXF float(FULL_BLOCK_PX)

#define CHUNK_BLOCKS 16
#define CHUNK_BLOCKSF float(CHUNK_BLOCKS)
#define CHUNK_PX (CHUNK_BLOCKS*BLOCK_PX)
#define CHUNK_PXF float(CHUNK_PX)

#define SUPER_CHUNK_CHUNKS 512
#define SUPER_CHUNK_CHUNKSF float(SUPER_CHUNK_CHUNKS)
#define SUPER_CHUNK_BLOCKS (SUPER_CHUNK_CHUNKS*CHUNK_BLOCKS)
#define SUPER_CHUNK_BLOCKSF float(SUPER_CHUNK_BLOCKS)
#define SUPER_CHUNK_PX (SUPER_CHUNK_CHUNKS*CHUNK_BLOCKS*BLOCK_PX)
#define SUPER_CHUNK_PXF float(SUPER_CHUNK_PX)

enum DrawOrder
{
	DRAW_ORDER_CAMERA,
	DRAW_ORDER_BACKGROUND,
	DRAW_ORDER_TIME_OF_DAY,
DRAW_ORDER_SCENE_START,
	DRAW_ORDER_BLOCK_ENTITY,
	DRAW_ORDER_TERRAIN,
DRAW_ORDER_SCENE_END,
	DRAW_ORDER_UI
};

enum BlockID
{
	BLOCK_EMPTY,
	BLOCK_OCCUPIED,
	
	BLOCK_BACKGROUND_WOOD,
	BLOCK_BACKGROUND_STONE,
	
	BLOCK_SCENE_GRASS,
	BLOCK_SCENE_STONE,
	
	BLOCK_FOREGROUND_LEAF,
	
	BLOCK_COUNT
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

// Directions
enum Direction
{
	NORTH		= 1 << 0,
	NORTH_EAST 	= 1 << 1,
	EAST		= 1 << 2,
	SOUTH_EAST 	= 1 << 3,
	SOUTH		= 1 << 4,
	SOUTH_WEST 	= 1 << 5,
	WEST		= 1 << 6,
	NORTH_WEST 	= 1 << 7,
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
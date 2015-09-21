#ifndef DEBUG_H
#define DEBUG_H

#include "Config.h"

class Terrain;
class Lighting;
class ChunkLoader;
class Game;
class World;
enum BlockID;

class Debug
{
public:
	Debug(Game *game);

	void update();
	void draw(SpriteBatch *spriteBatch);
	void toggle();

	void nextBlock(int action);
	void prevBlock(int action);

	void setVariable(const string &name, const string &value);

	void debugFunction(int action, const int i);

	bool isEnabled() { return m_enabled; }
	FontPtr m_font;

private:
	// Enabled flag
	bool m_enabled;
	bool m_debugChunkLoader;
	bool m_blockPainterEnabled;

	// Debug variables
	map<string, string> m_variables;

	// Game pointer
	Game *m_game;
	World *m_world;

	// Debug sprites
	Sprite m_bulbSprite;

	// Debug font

	//Texture2DPtr m_loadedChunks;

	// Paint block
	BlockID m_block;

	Random m_random;
};

#endif // DEBUG_H
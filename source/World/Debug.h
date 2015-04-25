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
	Debug(Game &game);

	void update();
	void draw(SpriteBatch *spriteBatch);
	void toggle();

	void nextBlock();
	void prevBlock();

	void setVariable(const string &name, const string &value);

	void debugFunction(const int i);

	bool isEnabled() { return m_enabled; }

private:
	// Enabled flag
	bool m_enabled;
	bool m_debugChunkLoader;

	// Debug variables
	map<string, string> m_variables;

	// Game pointer
	Game &m_game;
	World *m_world;

	// Debug sprites
	Sprite m_bulbSprite;

	// Debug font
	FontPtr m_font;

	//Texture2DPtr m_loadedChunks;

	// Paint block
	BlockID m_block;

	Random m_random;
};

#endif // DEBUG_H
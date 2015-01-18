#ifndef DEBUG_H
#define DEBUG_H

#include <x2d/x2d.h>
#include "game/gameObject.h"

class Terrain;
class Lighting;
class ChunkLoader;
enum BlockID;

class Debug : public GameObject
{
public:
	Debug(Terrain *terrain, Lighting *lighting);

	void update();
	void draw(xd::SpriteBatch *spriteBatch);
	void toggle();

	void nextBlock();
	void prevBlock();

	void setVariable(const string &name, const string &value);

	void debugF2();
	void debugF3();
	void debugF4();

	bool isEnabled() { return m_enabled; }

private:
	// Enabled flag
	bool m_enabled;
	bool m_debugChunkLoader;

	// Debug variables
	map<string, string> m_variables;

	// Terrain pointer
	Terrain *m_terrain;

	// Lighting potiner
	Lighting *m_lighting;

	// Debug sprites
	xd::Sprite m_bulbSprite;

	// Debug font
	xd::FontPtr m_font;

	//xd::Texture2DPtr m_loadedChunks;

	// Paint block
	BlockID m_block;
};

#endif // DEBUG_H
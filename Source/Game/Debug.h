#ifndef DEBUG_H
#define DEBUG_H

#include "Config.h"

class Lighting;
class OverworldGame;
class World;
class CommandLine;
enum BlockID;

class Debug : public GameObject
{
public:
	Debug(OverworldGame *game);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	void toggle();

	void nextBlock(int action);
	void prevBlock(int action);

	void setVariable(const string &name, const string &value);

	void debugFunction(KeyEvent *e);

	bool isEnabled() { return m_enabled; }
	FontPtr m_font;

private:
	// Enabled flag
	bool m_enabled;
	bool m_debugChunkLoader;
	bool m_debugLighting;
	bool m_blockPainterEnabled;

	Texture2DPtr m_blockPainterTexture;

	// Debug variables
	map<string, string> m_variables;

	// Game pointer
	OverworldGame *m_game;
	World *m_world;

	// Debug sprites
	Sprite m_bulbSprite;

	// Paint block
	BlockID m_block;

	// Random
	Random m_random;
};

#endif // DEBUG_H
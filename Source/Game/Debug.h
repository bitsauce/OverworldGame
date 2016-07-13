#ifndef DEBUG_H
#define DEBUG_H

#include "Config.h"

class Lighting;
class OverworldGame;
class World;
class CommandLine;
class Pointlight;
class BlockData;

class Debug : public SceneObject
{
public:
	Debug(OverworldGame *game);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	void toggle();

	void nextBlock(KeyEvent *e);
	void prevBlock(KeyEvent *e);

	void randomizeLight(KeyEvent *e);
	void placeLight(KeyEvent *e);

	void setVariable(const string &name, const string &value);

	void debugFunction(KeyEvent *e);

	bool isEnabled() { return m_enabled; }
	Resource<Font> m_font;

private:
	// Enabled flag
	bool m_enabled;
	bool m_debugChunkLoader;
	bool m_debugLighting;
	bool m_blockPainterEnabled;

	Resource<Texture2D> m_blockPainterTexture;

	// Debug variables
	map<string, string> m_variables;

	// Game pointer
	OverworldGame *m_game;
	World *m_world;

	// Paint block
	map<BlockID, BlockData*>::const_iterator m_block;

	// Random
	Random m_random;

	// Pointlight
	Pointlight *m_activePointlight;
};

#endif // DEBUG_H
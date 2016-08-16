#ifndef DEBUG_H
#define DEBUG_H

#include "Config.h"

class Lighting;
class OverworldGame;
class World;
class CommandLine;
class Pointlight;
class LightSource;
class BlockData;
class ColorPicker;

class Debug : public SceneObject
{
public:
	Debug(OverworldGame *game);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	void onMouseEvent(MouseEvent *e);

	void toggle();

	void nextBlock(KeyEvent *e);
	void prevBlock(KeyEvent *e);

	void randomizeLight(KeyEvent *e);
	void placeLight(KeyEvent *e);

	void addVariable(const string &name, const string &value);

	void debugFunction(KeyEvent *e);

	bool isEnabled() { return m_enabled; }

private:
	// Enabled flag
	bool m_enabled;
	bool m_debugChunkLoader;
	bool m_debugLighting;
	
	enum DebugMode
	{
		DEBUG_MODE_DEFAULT,
		BLOCK_PAINTER,
		LIGHT_PAINTER
	};
	DebugMode m_debugMode;

	Resource<Texture2D> m_blockPainterTexture;
	ColorPicker *m_colorPicker;

	// Debug variables
	Resource<Font> m_font;
	list<pair<string, string>> m_variables;

	// Game pointer
	OverworldGame *m_game;
	World *m_world;

	// Paint block
	map<BlockID, BlockData*>::const_iterator m_block;

	// Random
	Random m_random;

	// Light painter
	Resource<Shader> m_drawCircleShader;
	Pointlight *m_newPointLight;
	LightSource *m_selectedLight;
	bool m_lmbState;
	uint m_moveCount;
	Vector2F m_lightDragOffset;
};

#endif // DEBUG_H
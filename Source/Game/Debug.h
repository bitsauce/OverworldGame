#ifndef DEBUG_H
#define DEBUG_H

#include "Config.h"

class Lighting;
class OverworldGame;
class World;
class CommandLine;
class Pointlight;
class DebugPointlight;
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
	void onKeyEvent(KeyEvent *e);

	void toggle();

	void nextBlock(KeyEvent *e);
	void prevBlock(KeyEvent *e);

	void addVariable(const string &name, const string &value);

	void debugFunction(KeyEvent *e);

	bool isEnabled() { return m_enabled; }

private:
	// Enabled flag
	bool m_enabled;
	bool m_debugChunkLoader;
	
	enum DebugMode
	{
		DEBUG_MODE_DEFAULT,
		DEBUG_MODE_BLOCK_PAINTER,
		DEBUG_MODE_LIGHT,
		DEBUG_MODE_LIGHT_PAINTER
	};
	DebugMode m_debugMode;

	shared_ptr<Texture2D> m_blockPainterTexture;
	ColorPicker *m_colorPicker;

	// Debug variables
	Resource<Font> m_font;
	list<pair<string, string>> m_variables;

	// Game pointer
	OverworldGame *m_game;
	World *m_world;

	// Paint block
	map<BlockID, BlockData*>::const_iterator m_block;
	char m_blockSubId;

	// Random
	Random m_random;

	// Light painter
	Resource<Shader> m_drawCircleShader;
	DebugPointlight *m_newPointlight;
	DebugPointlight *m_selectedLight;
	bool m_lmbState;
	uint m_moveCount;
	Vector2F m_lightDragOffset;
};

#endif // DEBUG_H
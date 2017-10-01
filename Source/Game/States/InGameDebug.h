#pragma once

#include "Config.h"

class Lighting;
class Overworld;
class World;
class CommandLine;
class Pointlight;
class DebugPointlight;
class LightSource;
class BlockData;
class ColorPicker;
class GameState;

class InGameDebug : public SceneObject
{
public:
	InGameDebug(GameState *gameState);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	void onMouseEvent(MouseEvent *e);
	void onKeyEvent(KeyEvent *e);

	void toggle();

	void nextBlock(InputEvent *e);
	void prevBlock(InputEvent *e);

	void addVariable(const string &name, const string &value);

	void debugFunction(InputEvent *e);

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
	Overworld *m_game;
	World *m_world;

	// Paint block
	vector<BlockData*>::const_iterator m_block;

	// Random
	Random m_random;

	// Light painter
	Resource<Shader> m_drawCircleShader;
	DebugPointlight *m_newPointlight;
	DebugPointlight *m_selectedLight;
	bool m_lmbState;
	uint m_moveCount;
	Vector2F m_lightDragOffset;

	Resource<Texture2D> m_inputIconsTexture;
};
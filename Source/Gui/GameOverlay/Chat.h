#pragma once

#include "Gui/LineEdit.h"

class OverworldGame;
class GameOverlay;
class Commander;

class Chat : public UiObject
{
public:
	Chat(OverworldGame *game, GraphicsContext *context, GameOverlay *gameOverlay);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

	void toggle(InputEvent *e);
	void sendMessage(InputEvent *e);
	void nextMessage(InputEvent *e);
	void prevMessage(InputEvent *e);
	void insertMessage(const string &message);

private:
	OverworldGame *m_game;

	Resource<Font> m_font;

	LineEdit m_chatInput;
	
	bool m_active;
	float m_fadeTime;

	int m_chatLinePos;

	vector<string> m_messages;

	// Message history
	vector<string> m_inputHistory;
	int m_inputHistoryIndex;

	bool m_redrawText;
	RenderTarget2D * m_chatRenderTarget;
	SpriteBatch m_textSpriteBatch;
};
#pragma once

#include "Gui/Gui.h"

class Overworld;
class GameOverlay;
class Commander;

class Chat : public UiObject
{
public:
	Chat(Overworld *game, GameOverlay *gameOverlay);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

	void toggle(InputEvent *e);
	void sendMessage(InputEvent *e);
	void nextMessage(InputEvent *e);
	void prevMessage(InputEvent *e);
	void insertMessage(const string &message);



private:
	Overworld *m_game;

	Resource<Font> m_font;

	LineEdit *m_chatInput;
	
	bool m_active;
	float m_fadeTime;

	list<string> m_messages;

	// Message history
	vector<string> m_inputHistory;
	int m_inputHistoryIndex;

	bool m_redrawText;
	RenderTarget2D * m_chatRenderTarget;
	SpriteBatch m_textSpriteBatch;
};
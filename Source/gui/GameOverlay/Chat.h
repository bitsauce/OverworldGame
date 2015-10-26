#pragma once

#include "Gui/LineEdit.h"

class GameOverlay;

class Chat : public UiObject
{
public:
	Chat(Scene *scene, GameOverlay *gameOverlay);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	void toggle(int action);
	void sendMessage(int action);

private:
	GameOverlay *m_gameOverlay;
	LineEdit m_chatInput;
	bool m_active;
	float m_fadeTime;
	string m_chatText;
	FontPtr m_font;
};
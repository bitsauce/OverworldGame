#ifndef HOTBAR_SLOT_H
#define HOTBAR_SLOT_H

#include "Config.h"
#include "Gui/UiObject.h"

class GameOverlay;

class Hotbar : public UiObject, public MouseListener, public KeyListener
{
public:
	Hotbar(Scene *scene, GameOverlay *gameOverlay);
	~Hotbar();

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	void setSelectedSlot(int action, const uint slot);
	
	void keyEvent(const KeyEvent & event);
	void mouseWheelEvent(const int delta);

private:
	GameOverlay *m_gameOverlay;
	Sprite m_backgroundSprite;
	Sprite m_slotSprite;
	Sprite m_slotSelectedSprite;
	FontPtr m_font;
};

#endif // HOTBAR_SLOT_H
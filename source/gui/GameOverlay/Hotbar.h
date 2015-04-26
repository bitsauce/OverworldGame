#ifndef HOTBAR_SLOT_H
#define HOTBAR_SLOT_H

#include "Config.h"
#include "Gui/UiObject.h"

class GameOverlay;

class Hotbar : public UiObject, public KeyListener
{
public:
	Hotbar(Scene *scene, GameOverlay *gameOverlay);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	void keyPressEvent(const VirtualKey key);

private:
	GameOverlay *m_gameOverlay;
	Sprite m_spriteItemSlot;
	Sprite m_spriteItemSlotSelected;
	Sprite m_spriteBackground;
	FontPtr m_font;
};

#endif // HOTBAR_SLOT_H
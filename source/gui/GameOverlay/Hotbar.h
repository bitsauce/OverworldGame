#ifndef HOTBAR_SLOT_H
#define HOTBAR_SLOT_H

#include "Config.h"
#include "Gui/UiObject.h"

class GameOverlay;

class Hotbar : public UiObject, public KeyListener, public MouseListener
{
public:
	Hotbar(Scene *scene, GameOverlay *gameOverlay);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);
	
	void setSelectedSlot(const uint slot) { m_selectedSlot = slot; }
	uint getSelectedSlot() const { return m_selectedSlot; }

	void keyPressEvent(const VirtualKey key);
	void mouseWheelEvent(const int delta);

private:
	GameOverlay *m_gameOverlay;
	Sprite m_slotSprite;
	Sprite m_slotSelectedSprite;
	Sprite m_backgroundSprite;
	FontPtr m_font;
	uint m_selectedSlot;
};

#endif // HOTBAR_SLOT_H
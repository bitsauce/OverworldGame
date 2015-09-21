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

	void setSelectedSlot(int action, const uint slot)
	{
		if(action != GLFW_PRESS) return;
		m_selectedSlot = slot;
	}

	uint getSelectedSlot() const
	{
		return m_selectedSlot;
	}
	
	void keyPressEvent(const VirtualKey key);
	void mouseWheelEvent(const int delta);

private:
	GameOverlay *m_gameOverlay;
	Sprite m_backgroundSprite;
	Sprite m_slotSprite;
	Sprite m_slotSelectedSprite;
	FontPtr m_font;
	uint m_selectedSlot;
};

#endif // HOTBAR_SLOT_H
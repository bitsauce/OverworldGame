#ifndef HOTBAR_SLOT_H
#define HOTBAR_SLOT_H

#include "Config.h"
#include "Gui/UiObject.h"

class GameOverlay;

class Hotbar : public UiObject
{
public:
	Hotbar(GameOverlay *gameOverlay);
	~Hotbar();

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

	void setSelectedSlot(KeyEvent* e, const uint slot);
	
	void onMouseWheel(MouseEvent *e);
	void onMouseMove(MouseEvent *e);
	void onMouseDown(MouseEvent *e);

private:
	GameOverlay *m_gameOverlay;
	Sprite m_backgroundSprite;
	Sprite m_slotSprite;
	Sprite m_slotSelectedSprite;
	Resource<Font> m_itemAmountFont, m_itemDescFont;
	Vector2F m_mousePosition;
	int m_mouseWheel;
};

#endif // HOTBAR_SLOT_H
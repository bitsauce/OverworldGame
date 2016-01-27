#ifndef HOTBAR_SLOT_H
#define HOTBAR_SLOT_H

#include "Config.h"
#include "Gui/UiObject.h"

class GameOverlay;

class Hotbar : public UiObject
{
public:
	Hotbar(Scene *scene, GameOverlay *gameOverlay);
	~Hotbar();

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

	void setSelectedSlot(int action, const uint slot);
	
	void onKeyEvent(const KeyEvent & event);
	void onMouseWheelEvent(const int delta);

private:
	GameOverlay *m_gameOverlay;
	Sprite m_backgroundSprite;
	Sprite m_slotSprite;
	Sprite m_slotSelectedSprite;
	Resource<Font> m_font;
};

#endif // HOTBAR_SLOT_H
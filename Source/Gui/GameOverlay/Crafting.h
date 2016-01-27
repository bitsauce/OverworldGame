#ifndef CRAFTING_H
#define CRAFTING_H

#include "Gui/UiObject.h"
#include "Game/Storage.h"

class GameOverlay;

class Crafting : public UiObject
{
public:
	Crafting(Scene *scene, GameOverlay *gameOverlay);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	
	void keyEvent(const KeyEvent & event);

private:
	GameOverlay *m_gameOverlay;
	Storage m_storage;
	Sprite m_itemSlotSprite;
	Sprite m_backgroundSprite;
	Resource<Font> m_font;
};

#endif // CRAFTING_H
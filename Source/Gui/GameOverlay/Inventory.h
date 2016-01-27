#ifndef INVENTORY_H
#define INVENTORY_H

#include "Gui/UiObject.h"
#include "Game/Storage.h"

class GameOverlay;

class Inventory : public UiObject
{
public:
	Inventory(Scene *scene, GameOverlay *gameOverlay);

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);

	void toggle(int action);
	void show();
	void hide();
	
	void keyEvent(const KeyEvent & event);

private:
	GameOverlay *m_gameOverlay;
	Sprite m_itemSlotSprite;
	Sprite m_backgroundSprite;
	Resource<Font> m_font;

	float m_fadeInAlpha;
	float m_animationDuration;
	float m_animationTime;
	float m_interpolateStartY, m_interpolateEndY;
	float m_interpolateStartAlpha, m_interpolateEndAlpha;
	bool m_showing;
};

#endif // INVENTORY_H
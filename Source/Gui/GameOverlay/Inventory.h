#ifndef INVENTORY_H
#define INVENTORY_H

#include "Gui/UiObject.h"
#include "Game/ItemContainer.h"

class Scene;
class GameOverlay;

class Inventory : public UiObject, public KeyListener
{
public:
	Inventory(Scene *scene, GameOverlay *gameOverlay);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	void toggle();
	void show();
	void hide();
	
	void keyPressEvent(const VirtualKey key);

private:
	GameOverlay *m_gameOverlay;
	ItemContainer m_itemContainer;
	Sprite m_itemSlotSprite;
	Sprite m_backgroundSprite;
	FontPtr m_font;

	float m_fadeInAlpha;
	float m_animationDuration;
	float m_animationTime;
	float m_interpolateStartY, m_interpolateEndY;
	float m_interpolateStartAlpha, m_interpolateEndAlpha;
	bool m_showing;
};

#endif // INVENTORY_H
#ifndef CRAFTING_H
#define CRAFTING_H

#include "Gui/UiObject.h"
#include "Game/Storage.h"

class Scene;
class GameOverlay;

class Crafting : public UiObject, public KeyListener
{
public:
	Crafting(Scene *scene, GameOverlay *gameOverlay);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);
	
	void keyPressEvent(const VirtualKey key);

private:
	GameOverlay *m_gameOverlay;
	Storage m_storage;
	Sprite m_itemSlotSprite;
	Sprite m_backgroundSprite;
	FontPtr m_font;
};

#endif // CRAFTING_H
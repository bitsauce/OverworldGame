#ifndef CRAFTING_H
#define CRAFTING_H

#include "Gui/UiObject.h"
#include "Game/ItemStorage.h"

class Scene;
class GameOverlay;

class Crafting : public UiObject
{
public:
	Crafting(Scene *scene, GameOverlay *gameOverlay);

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

private:
	GameOverlay *m_gameOverlay;
	ItemStorage m_ItemStorage;
	Sprite m_itemSlotSprite;
	Sprite m_backgroundSprite;
	FontPtr m_font;
};

#endif // CRAFTING_H
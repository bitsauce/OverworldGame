#ifndef INVENTORY_H
#define INVENTORY_H

#include "Config.h"
#include "UiObject.h"

class ItemContainer;
class Player;
class Hotbar;
class HealthManaStatus;
class Omnicon;

class GameOverlay : public UiObject, public MouseListener
{
public:
	GameOverlay(Player *player, UiObject *object);
	~GameOverlay();

	void update(const float dt);
	void draw(SpriteBatch *spriteBatch, const float alpha);

private:
	Player *m_player;
	Hotbar *m_hotbar;
	HealthManaStatus *m_healthManaStatus;
	Omnicon *m_omnicon;
};

#endif // INVENTORY_H
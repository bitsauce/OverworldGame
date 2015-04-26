#ifndef GAME_OVERLAY_H
#define GAME_OVERLAY_H

#include "Config.h"
#include "UiObject.h"
#include "Game/ItemContainer.h"

class ItemContainer;
class Player;
class Hotbar;
class HealthManaStatus;
class Omnicon;
class Inventory;
class Game;

class GameOverlay : public UiObject, public MouseListener
{
public:
	GameOverlay(Game *game, Scene *scene);
	~GameOverlay();

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

	void toggleCrafting();
	bool isCrafting() const { return m_craftingEnabled; }

	void setHoldItem(const uint idx);
	ItemContainer::ItemSlot getHoldItem() const { return m_holdItem; }

	bool isHovered() const;

	void setPlayer(Player *player) { m_player = player; }
	Player *getPlayer() const { return m_player; }

private:
	Game *m_game;
	Player *m_player;
	Hotbar *m_hotbar;
	HealthManaStatus *m_healthManaStatus;
	Omnicon *m_omnicon;
	Inventory *m_inventory;

	bool m_craftingEnabled;

	ItemContainer::ItemSlot m_holdItem;
	FontPtr m_font;
};

#endif // GAME_OVERLAY_H
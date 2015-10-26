#ifndef GAME_OVERLAY_H
#define GAME_OVERLAY_H

#include "Config.h"
#include "UiObject.h"
#include "Game/Storage.h"
#include "GameOverlay/Hotbar.h"
#include "GameOverlay/Inventory.h"
#include "GameOverlay/HealthManaStatus.h"
#include "GameOverlay/Omnicon.h"
#include "GameOverlay/Crafting.h"
#include "GameOverlay/Chat.h"

class OverworldGame;
class Player;

class GameOverlay : public UiObject
{
public:
	GameOverlay(OverworldGame *game, Scene *scene);
	~GameOverlay();

	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);
	
	// Getters/setters
	void setPlayer(Player *player) { m_player = player; }
	Player *getPlayer() const { return m_player; }
	Hotbar *getHotbar() const { return m_hotbar; }
	HealthManaStatus *getHealthManaStatus() const { return m_healthManaStatus; }
	Omnicon *getOmnicon() const { return m_omnicon; }
	Inventory *getInventory() const { return m_inventory; }
	Chat * getChat() const { return m_chat; }

	// Crafting overlay
	void toggleCrafting();
	bool isCrafting() const { return m_craftingEnabled; }

	void performSlotAction(Storage::Slot *slot, const VirtualKey type);

	// Ui
	bool isHovered() const;

	bool m_hidden;

private:
	OverworldGame *m_game;
	Player *m_player;
	Hotbar *m_hotbar;
	HealthManaStatus *m_healthManaStatus;
	Omnicon *m_omnicon;
	Inventory *m_inventory;
	Crafting *m_crafting;
	Chat * m_chat;

	bool m_craftingEnabled;

	FontPtr m_font;
};

#endif // GAME_OVERLAY_H
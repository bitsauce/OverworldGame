#ifndef GAME_OVERLAY_H
#define GAME_OVERLAY_H

#include "Config.h"
#include "Gui/UiObjects/UiObject.h"
#include "Game/Storage.h"
#include "Hotbar.h"
#include "Inventory.h"
#include "HealthManaStatus.h"
#include "Omnicon.h"
#include "Crafting.h"
#include "Chat.h"

class Overworld;
class Player;

class GameOverlay : public Canvas
{
public:
	GameOverlay(Overworld *game, Window *window);
	~GameOverlay();

	void onTick(TickEvent *e);
	void onDraw(DrawEvent *e);
	
	// Getters/setters
	void setPlayer(Player *player) { m_player = player; }
	Player *getPlayer() const { return m_player; }
	Hotbar *getHotbar() const { return m_hotbar; }
	HealthManaStatus *getHealthManaStatus() const { return m_healthManaStatus; }
	Omnicon *getOmnicon() const { return m_omnicon; }
	Inventory *getInventory() const { return m_inventory; }
	Chat *getChat() const { return m_chat; }

	// Crafting overlay
	void toggleCrafting();
	bool isCrafting() const { return m_craftingEnabled; }

	void performSlotAction(Storage::Slot *slot, const Keycode type);

	// Ui
	bool isHovered() const;

	bool m_active;

private:
	Overworld *m_game;
	Player *m_player;
	Hotbar *m_hotbar;
	HealthManaStatus *m_healthManaStatus;
	Omnicon *m_omnicon;
	Inventory *m_inventory;
	Crafting *m_crafting;
	Chat * m_chat;

	bool m_craftingEnabled;

	Resource<Font> m_font;
};

#endif // GAME_OVERLAY_H
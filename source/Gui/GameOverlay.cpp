#include "GameOverlay.h"
#include "Game/ItemContainer.h"
#include "Entities/Player.h"
#include "GameOverlay/Hotbar.h"
#include "GameOverlay/HealthManaStatus.h"
#include "GameOverlay/Omnicon.h"
#include "GameOverlay/Inventory.h"
#include "Items/ItemData.h"
#include "Game/Game.h"
#include "Networking/Connection.h"
#include "Entities/ItemDrop.h"

GameOverlay::GameOverlay(Game *game, Scene *scene) :
	UiObject(scene, scene->getCanvas()),
	m_game(game),
	m_player(nullptr),
	m_craftingEnabled(false),
	m_font(ResourceManager::get<Font>(UI_INVENTORY_FONT))
{
	setPosition(Vector2(0.0f, 0.0f));
	setSize(Vector2(1.0f, 1.0f));

	m_hotbar = new Hotbar(scene, this);
	m_healthManaStatus = new HealthManaStatus(scene, this);
	m_omnicon = new Omnicon(scene, this);
	m_inventory = new Inventory(scene, this);
	m_crafting = new Crafting(scene, this);
}

GameOverlay::~GameOverlay()
{
	delete m_hotbar;
	delete m_healthManaStatus;
	delete m_omnicon;
	delete m_inventory;
	delete m_crafting;
}

void GameOverlay::update(const float delta)
{
	ItemSlot &heldItem = m_player->getHeldItem();
	if(!heldItem.isEmpty() && !isHovered() && Input::getKeyState(XD_RMB))
	{
		ItemDrop *itemDrop = new ItemDrop(m_game->getWorld(), m_player->getCenter() - Vector2(0.0f, 20.0f), heldItem.getItem(), heldItem.getAmount());
		itemDrop->applyImpulse(Vector2(m_game->getWorld()->getCamera()->getInputPosition() - m_player->getCenter()).normalized() * 4.0f);
		heldItem.set(ITEM_NONE, 0);
	}
}

void GameOverlay::draw(SpriteBatch *spriteBatch, const float delta)
{
	m_player->getHeldItem().drawItem(Input::getPosition(), spriteBatch, m_font);
}

void GameOverlay::toggleCrafting()
{
	if(!m_craftingEnabled)
	{
		m_inventory->show();
	}
	else
	{
		m_inventory->hide();
	}

	m_craftingEnabled = !m_craftingEnabled;
}

void GameOverlay::takeItem(ItemContainer *itemContainer, const uint idx)
{
	ItemSlot &heldItem = m_player->getHeldItem();
	ItemSlot &slot = itemContainer->getSlotAt(idx);

	ItemSlot tmp = heldItem;
	heldItem.set(slot.getItem(), slot.getAmount());
	itemContainer->getSlotAt(idx).set(tmp.getItem(), tmp.getAmount());
}

void GameOverlay::placeSingleItem(ItemContainer *itemContainer, const uint idx)
{
	ItemSlot &heldItem = m_player->getHeldItem();
	ItemSlot &slot = itemContainer->getSlotAt(idx);
	if(heldItem.isEmpty())
	{
		if(slot.getItem() == ITEM_NONE)
		{
			slot.set(heldItem.getItem(), 1);
		}
		else if(slot.getItem() == heldItem.getItem())
		{
			slot.inc();
		}
		heldItem.dec();
	}
}

bool GameOverlay::isHovered() const
{
	return m_hotbar->isHovered() || m_inventory->isHovered();
}
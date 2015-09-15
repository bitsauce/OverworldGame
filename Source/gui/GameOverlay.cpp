#include "GameOverlay.h"
#include "Game/Storage.h"
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
	if (!m_player) return;
	Storage::Slot *heldItem = m_player->getHeldItem();
	if(!heldItem->isEmpty() && !isHovered() && Input::getKeyState(XD_RMB))
	{
		ItemDrop *itemDrop = new ItemDrop(m_game->getWorld(), m_player->getCenter() - Vector2(0.0f, 20.0f), heldItem->getItem(), heldItem->getAmount());
		itemDrop->applyImpulse(Vector2(m_game->getWorld()->getCamera()->getInputPosition() - m_player->getCenter()).normalized() * 4.0f);
		heldItem->set(ITEM_NONE, 0);
	}
}

void GameOverlay::draw(SpriteBatch *spriteBatch, const float delta)
{
	if (!m_player) return;
	m_player->getHeldItem()->drawItem(Input::getPosition() + Vector2(5.0f, 0.0f), spriteBatch, m_font);
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

bool GameOverlay::isHovered() const
{
	return m_hotbar->isHovered() || m_inventory->isHovered() || m_crafting->isHovered();
}

void GameOverlay::performSlotAction(Storage::Slot *slot, const VirtualKey type)
{
	if (!m_player) return;
	Storage::Slot *heldSlot = m_player->getHeldItem();
	if(type == XD_LMB)
	{
		if(!heldSlot->isEmpty())
		{
			// Left click with a held item...
			if(heldSlot->getItem() == slot->getItem())
			{
				// ... and the items are of the same type -> Place items from the held slot into the clicked slot
				int rest = slot->inc(heldSlot->getAmount());
				if(rest == 0)
				{
					heldSlot->set(ITEM_NONE, 0);
				}
				else
				{
					heldSlot->set(heldSlot->getItem(), rest);
				}
			}
			else
			{
				// ... and the items are of different types -> Swap held and clicked slot
				Storage::Slot tmp = *slot;
				*slot = *heldSlot;
				*heldSlot = tmp;
			}
		}
	}
	else if(type == XD_RMB)
	{
		if(heldSlot->isEmpty())
		{
			// Right click with no held item...
			if(Input::getKeyState(XD_KEY_LCONTROL))
			{
				// ... and with CTRL -> Take half of the clicked slot
				int halfAmount = ceil(slot->getAmount() / 2.f);
				heldSlot->set(slot->getItem(), halfAmount);
				slot->dec(halfAmount);
			}
			else
			{
				// ... and without CTRL -> Take the clicked slot
				Storage::Slot tmp = *slot;
				*slot = *heldSlot;
				*heldSlot = tmp;
			}
		}
		else
		{
			// Right click with a held item -> Place one of the held items into the clicked slot
			if(heldSlot->getItem() == slot->getItem())
			{
				if(Input::getKeyState(XD_KEY_LCONTROL))
				{
					int halfAmount = ceil(heldSlot->getAmount() / 2.f);
					heldSlot->dec(halfAmount - slot->inc(halfAmount));
				}
				else
				{
					if(slot->inc() == 0)
					{
						heldSlot->dec();
					}
				}
			}
			else if(slot->isEmpty())
			{
				if(Input::getKeyState(XD_KEY_LCONTROL))
				{
					int halfAmount = ceil(heldSlot->getAmount() / 2.f);
					slot->set(heldSlot->getItem(), halfAmount);
					heldSlot->dec(halfAmount);
				}
				else
				{
					slot->set(heldSlot->getItem(), 1);
					heldSlot->dec();
				}
			}
		}
	}
}
#include "GameOverlay.h"
#include "Game/Game.h"
#include "Game/Storage.h"
#include "Items/ItemData.h"
#include "Networking/Connection.h"
#include "Entities/Player.h"
#include "Entities/ItemDrop.h"

GameOverlay::GameOverlay(Overworld *game, UiObject *parent, GraphicsContext *context) :
	UiObject(parent),
	m_game(game),
	m_player(nullptr),
	m_craftingEnabled(false),
	m_font(Resource<Font>("Fonts/Inventory")),
	m_active(true)
{
	setPosition(Vector2F(0.0f, 0.0f));
	setSize(Vector2F(1.0f, 1.0f));

	m_hotbar = new Hotbar(this);
	/*m_healthManaStatus = new HealthManaStatus(scene, this);
	m_omnicon = new Omnicon(scene, this);
	m_inventory = new Inventory(scene, this);
	m_crafting = new Crafting(scene, this);*/
	//m_chat = new Chat(game, context, this);
}

GameOverlay::~GameOverlay()
{
	delete m_hotbar;
	/*delete m_healthManaStatus;
	delete m_omnicon;
	delete m_inventory;
	delete m_crafting;*/
	delete m_chat;
}

void GameOverlay::onTick(TickEvent *e)
{
	UiObject::onTick(e);
	if(!m_player) return;
	/*Storage::Slot *heldItem = m_player->getHeldItem();
	if(!heldItem->isEmpty() && !isHovered() && m_game->getInputManager()->getKeyState(MOUSE_BUTTON_RIGHT))
	{
		ItemDrop *itemDrop = new ItemDrop(m_game->getClient()->getWorld(), heldItem->getItem(), heldItem->getAmount());
		itemDrop->setPosition(m_player->getCenter() - Vector2F(0.0f, 20.0f));
		itemDrop->applyImpulse(Vector2F(m_game->getClient()->getWorld()->getCamera()->getInputPosition() - m_player->getCenter()).normalized() * 4.0f);
		heldItem->set(ITEM_NONE, 0);
	}*/
}

void GameOverlay::onDraw(DrawEvent *e)
{
	e->getGraphicsContext()->setTransformationMatrix(Matrix4());
	
	SpriteBatch *spriteBatch = (SpriteBatch*)e->getUserData();
	spriteBatch->end();
	spriteBatch->begin(e->getGraphicsContext(), SpriteBatch::State());

	UiObject::onDraw(e);

	if(!m_player) return;

	m_player->getHeldItem()->drawItem(m_game->getInputManager()->getPosition() + Vector2F(5.0f, 0.0f), spriteBatch, m_font);
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
	return m_hotbar->isHovered();// || m_inventory->isHovered() || m_crafting->isHovered();
}

void GameOverlay::performSlotAction(Storage::Slot *slot, const Keycode type)
{
	if(!m_player) return;
	Storage::Slot *heldSlot = m_player->getHeldItem();
	if(type == SAUCE_MOUSE_BUTTON_LEFT)
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
	else if(type == SAUCE_MOUSE_BUTTON_RIGHT)
	{
		if(heldSlot->isEmpty())
		{
			// Right click with no held item...
			if(KeyEvent::CTRL)
			{
				// ... and with CTRL -> Take half of the clicked slot
				int halfAmount = (int) ceil(slot->getAmount() / 2.f);
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
				if(KeyEvent::CTRL)
				{
					int halfAmount = (int) ceil(heldSlot->getAmount() / 2.f);
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
				if(KeyEvent::CTRL)
				{
					int halfAmount = (int) ceil(heldSlot->getAmount() / 2.f);
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
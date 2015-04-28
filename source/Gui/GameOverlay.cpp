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
}

GameOverlay::~GameOverlay()
{
	delete m_hotbar;
	delete m_healthManaStatus;
	delete m_omnicon;
}

void GameOverlay::update(const float delta)
{
	if(!m_holdItem.isEmpty() && !isHovered() && Input::getKeyState(XD_RMB))
	{
		ItemDrop *itemDrop = new ItemDrop(m_game->getWorld(), m_player->getCenter() - Vector2(0.0f, 20.0f), m_holdItem.item, m_holdItem.amount);
		itemDrop->applyImpulse(Vector2(m_game->getWorld()->getCamera()->getInputPosition() - m_player->getCenter()).normalized() * 4.0f);
		m_holdItem.set(ITEM_NONE, 0);
	}
}

void GameOverlay::draw(SpriteBatch *spriteBatch, const float delta)
{
	if(!m_holdItem.isEmpty())
	{
		Vector2 position = Input::getPosition();
		if(m_holdItem.item != ITEM_NONE)
		{
			spriteBatch->drawSprite(Sprite(ItemData::get(m_holdItem.item)->getIconTexture(), Rect(position.x + 13.f, position.y + 12.f, 32.f, 32.f)));
			if(m_holdItem.amount > 1)
			{
				spriteBatch->drawText(Vector2(position.x + 11.f, position.y + 10.f), util::intToStr(m_holdItem.amount), m_font);
			}
		}
	}
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
	ItemContainer::Slot tmp = m_holdItem;
	m_holdItem = itemContainer->getSlotAt(idx);
	itemContainer->setSlotAt(idx, tmp);
}

bool GameOverlay::isHovered() const
{
	return m_hotbar->isHovered() || m_inventory->isHovered();
}
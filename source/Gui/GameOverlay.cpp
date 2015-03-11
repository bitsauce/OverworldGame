#include "GameOverlay.h"
#include "Game/ItemContainer.h"
#include "Entities/Player.h"
#include "Gui/GameOverlay/Hotbar.h"
#include "Gui/GameOverlay/HealthManaStatus.h"

#include "World/World.h"
#include "Networking/Connection.h"

GameOverlay::GameOverlay(Player *player, UiObject *parent) :
	UiObject(parent),
	m_player(player)
{
	m_hotbar = new Hotbar(player, parent);
	m_healthManaStatus = new HealthManaStatus(player, parent);
}

GameOverlay::~GameOverlay()
{
	delete m_hotbar;
	delete m_healthManaStatus;
}

void GameOverlay::update(const float dt)
{
}

void GameOverlay::draw(SpriteBatch *spriteBatch, const float dt)
{
}
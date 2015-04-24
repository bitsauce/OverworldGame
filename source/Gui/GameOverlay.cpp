#include "GameOverlay.h"
#include "Game/ItemContainer.h"
#include "Entities/Player.h"
#include "GameOverlay/Hotbar.h"
#include "GameOverlay/HealthManaStatus.h"
#include "GameOverlay/Omnicon.h"

#include "World/World.h"
#include "Networking/Connection.h"

GameOverlay::GameOverlay(Player *player, UiObject *parent) :
	UiObject(parent),
	m_player(player)
{
	m_hotbar = new Hotbar(player, parent);
	m_healthManaStatus = new HealthManaStatus(player, parent);
	m_omnicon = new Omnicon(player, parent);
}

GameOverlay::~GameOverlay()
{
	delete m_hotbar;
	delete m_healthManaStatus;
	delete m_omnicon;
}

void GameOverlay::update(const float dt)
{
}

void GameOverlay::draw(SpriteBatch *spriteBatch, const float dt)
{
}
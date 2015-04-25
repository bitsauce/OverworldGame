#include "GameOverlay.h"
#include "Game/ItemContainer.h"
#include "Entities/Player.h"
#include "GameOverlay/Hotbar.h"
#include "GameOverlay/HealthManaStatus.h"
#include "GameOverlay/Omnicon.h"

#include "World/World.h"
#include "Networking/Connection.h"

GameOverlay::GameOverlay(Scene *scene, Player *player, UiObject *parent) :
	UiObject(scene, parent),
	m_player(player)
{
	m_hotbar = new Hotbar(scene, player, parent);
	m_healthManaStatus = new HealthManaStatus(scene, player, parent);
	m_omnicon = new Omnicon(scene, player, parent);
}

GameOverlay::~GameOverlay()
{
	delete m_hotbar;
	delete m_healthManaStatus;
	delete m_omnicon;
}

void GameOverlay::update(const float delta)
{
}

void GameOverlay::draw(SpriteBatch *spriteBatch, const float delta)
{
}
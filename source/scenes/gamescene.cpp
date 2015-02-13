#include "Game.h"
#include "Game/World.h"

#include "Gui/UiObject.h"
#include "Gui/Canvas.h"
#include "Gui/GameOverlay.h"

#include "Constants.h"

void GameScene::showEvent()
{
	LOG("Scene: Game Scene");

	// Setup canvas
	canvas = new Canvas();
	canvas->update();
	
	Player *player = new Player();
	player->getBody()->setPosition(0, 0);
	player->getItemContainer().addItem(ITEM_PICKAXE_IRON, 1);
	player->getItemContainer().addItem(ITEM_GRASS_BLOCK, 255);

	m_inventory = new GameOverlay(player, canvas);
}

void GameScene::hideEvent()
{
	// Save and clear
	World::save();
	World::clear();
}
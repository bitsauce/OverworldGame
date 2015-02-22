#include "Game.h"
#include "World/World.h"

#include "Gui/UiObject.h"
#include "Gui/Canvas.h"
#include "Gui/GameOverlay.h"

#include "Constants.h"

void GameScene::showEvent()
{
	LOG("Scene: Game Scene");

	// Setup canvas
	canvas = new Canvas(numeric_limits<int>::max(), numeric_limits<int>::max());
	canvas->update();
	
	m_inventory = new GameOverlay(World::getPlayers()[0], canvas);
}

void GameScene::hideEvent()
{
	// Save and clear
	World::save();
	World::clear();
}
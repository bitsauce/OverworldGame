#include "Game.h"
#include "Game/World.h"

#include "Gui/UiObject.h"
#include "Gui/Canvas.h"
#include "Gui/Inventory.h"

#include "Constants.h"

void GameScene::showEvent()
{
	LOG("Scene: Game Scene");

	// Setup canvas
	canvas = new Canvas();
	canvas->update();

	m_inventory = new Inventory(canvas, new ItemContainer(9));
}

void GameScene::hideEvent()
{
	// Save and clear
	World::save();
	World::clear();
}
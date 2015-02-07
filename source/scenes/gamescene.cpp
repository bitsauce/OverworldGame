#include "gamescene.h"
#include "game.h"

#include "game/world.h"

#include "gui/uiobject.h"
#include "gui/canvas.h"

#include "gui/Inventory.h"

#include "constants.h"

void GameScene::showEvent()
{
	xd::LOG("Scene: Game Scene");

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
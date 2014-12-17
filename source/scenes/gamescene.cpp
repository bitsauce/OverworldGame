#include "gamescene.h"
#include "game.h"

#include "game/world.h"

#include "gui/uiobject.h"
#include "gui/canvas.h"

#include "constants.h"

void GameScene::showEvent()
{
	LOG("Scene: Game Scene");

	// Setup canvas
	canvas = new Canvas();
	canvas->update();
}

void GameScene::hideEvent()
{
	// Save and clear
	World::save();
	World::clear();
}
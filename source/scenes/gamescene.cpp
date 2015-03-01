#include "GameScene.h"
#include "World/World.h"

#include "Gui/UiObject.h"
#include "Gui/Canvas.h"
#include "Gui/GameOverlay.h"

#include "Constants.h"

GameScene::GameScene()
{
	LOG("Scene: Game Scene");

	// Setup canvas
	canvas = new Canvas();
	canvas->update();
}

GameScene::~GameScene()
{
	// Save and clear
	World::save();
	World::clear();
}
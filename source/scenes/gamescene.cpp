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
	canvas = new Canvas(numeric_limits<int>::max(), numeric_limits<int>::max());
	canvas->update();
	
	if(World::getPlayers().size() > 0) m_inventory = new GameOverlay(World::getPlayers()[0], canvas);
}

GameScene::~GameScene()
{
	// Save and clear
	World::save();
	World::clear();
}
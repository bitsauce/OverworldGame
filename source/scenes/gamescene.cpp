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
	
	for(uint i = 0; i < m_uiObjects.size(); ++i)
	{
		delete m_uiObjects[i];
	}
	m_uiObjects.clear();
}

void GameScene::drawEvent()
{
	Game::draw();
}

void GameScene::updateEvent()
{
	Game::update();
}

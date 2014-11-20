#include "gamescene.h"
#include "game.h"

#include "gui/uiobject.h"

void GameScene::showEvent()
{
}

void GameScene::hideEvent()
{
	for(uint i = 0; i < m_uiObjects.size(); ++i)
	{
		delete m_uiObjects[i];
	}
}

void GameScene::drawEvent()
{
	gameDraw();
}

void GameScene::updateEvent()
{
	gameUpdate();
}

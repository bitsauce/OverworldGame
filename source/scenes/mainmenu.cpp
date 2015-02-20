#include "mainmenu.h"
#include "worldselect.h"
#include "scenemanager.h"

#include "gui/button.h"
#include "gui/canvas.h"

#include "game.h"
#include "constants.h"

int TITLE_BLOCKS[4][37] = {
	{ 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0 },
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1 },
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1 },
	{ 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0 }
};

void MainMenuScene::showEvent()
{
	xd::LOG("Scene: Main menu");
	
	// Setup canvas
	canvas = new Canvas(800, 600);
	canvas->setSize(Vector2(1.0f, 1.0f));

	// Create and setup menu buttons
	Button *spBtn = new Button("Singleplayer", function<void()>(bind(&MainMenuScene::showWorldSelect, this)), canvas);
	Button *mpBtn = new Button("Multiplayer", function<void()>(), canvas);
	
	spBtn->setPosition(Vector2(0.0f, -0.1f));
	spBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	spBtn->setAnchor(Vector2(0.5f, 0.5f));

	mpBtn->setPosition(Vector2(0.0f, 0.0f));
	mpBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	mpBtn->setAnchor(Vector2(0.5f, 0.5f));

	m_sceneObjects.push_back(spBtn);
	m_sceneObjects.push_back(mpBtn);
}

void MainMenuScene::hideEvent()
{
	for(uint i = 0; i < m_sceneObjects.size(); ++i)
		delete m_sceneObjects[i];
	m_sceneObjects.clear();
}

void MainMenuScene::showWorldSelect()
{
	SceneManager::gotoScene(SCENE_WORLD_SELECT);
}
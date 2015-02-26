#include "WorldSelect.h"
#include "WorldCreate.h"
#include "SceneManager.h"

#include "Gui/UiObject.h"
#include "Gui/Button.h"
#include "Gui/Canvas.h"

#include "Constants.h"
#include "Physics/PhysicsBody.h"
#include "Entities/Player.h"

void WorldSelectScene::showEvent()
{
	LOG("Scene: Select World");
	
	// Setup canvas
	canvas = new Canvas(800, 600);
	canvas->update();

	FileSystemIterator itr("saves:/Overworld/", "*", FileSystemIterator::DIRECTORIES);
	int i = 0;
	while(itr)
	{
		string worldDir = itr.next();
		Button *button = new Button(IniFile(worldDir + "/world.ini").getValue("world", "name"), function<void()>(bind(&WorldSelectScene::worldClicked, this)), canvas);
		button->setAnchor(Vector2(0.5f, 0.5f));
		button->setPosition(Vector2(0.0f, -0.3f + i++*0.1f));
		button->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
		button->setUserData(new string(worldDir));
		m_sceneObjects.push_back(button);
	}

	Button *cwBtn = new Button("Create World", function<void()>(bind(&WorldSelectScene::showCreateWorld, this)), canvas);
	cwBtn->setAnchor(Vector2(0.5f, 1.0f));
	cwBtn->setPosition(Vector2(0.0f, -0.1f));
	cwBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	m_sceneObjects.push_back(cwBtn);
}

void WorldSelectScene::hideEvent()
{
	for(uint i = 0; i < m_sceneObjects.size(); ++i)
		delete m_sceneObjects[i];
	m_sceneObjects.clear();
}

void WorldSelectScene::worldClicked()
{
	// Create a player
	Player *p = new Player();
	p->getBody()->setPosition(0, 0);

	// Go to game
	SceneManager::gotoScene(SCENE_GAME);
}

void WorldSelectScene::showCreateWorld()
{
	SceneManager::gotoScene(SCENE_WORLD_CREATE);
}
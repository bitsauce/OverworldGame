#include "WorldSelect.h"
#include "WorldCreate.h"
#include "SceneManager.h"
#include "GameScene.h"

#include "Gui/UiObject.h"
#include "Gui/Button.h"
#include "Gui/Canvas.h"

#include "Constants.h"
#include "Physics/PhysicsBody.h"
#include "Entities/Player.h"

WorldSelectScene::WorldSelectScene(World &world) :
	m_world(world)
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
		addSceneObject(button);
	}

	Button *cwBtn = new Button("Create World", function<void()>(bind(&WorldSelectScene::showCreateWorld, this)), canvas);
	cwBtn->setAnchor(Vector2(0.5f, 1.0f));
	cwBtn->setPosition(Vector2(0.0f, -0.1f));
	cwBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	addSceneObject(cwBtn);
}

void WorldSelectScene::worldClicked()
{
	// Create a player
	//Player *p = new Player();
	//p->getBody()->setPosition(0, 0);

	// Go to game
	SceneManager::setScene(new GameScene(m_world));
}

void WorldSelectScene::showCreateWorld()
{
	SceneManager::setScene(new WorldCreateScene(m_world));
}
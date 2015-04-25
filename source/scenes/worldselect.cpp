#include "WorldSelect.h"
#include "WorldCreate.h"
#include "GameScene.h"

#include "Gui/UiObject.h"
#include "Gui/Button.h"
#include "Gui/Canvas.h"

#include "Constants.h"
#include "Entities/Physics/DynamicEntity.h"
#include "Entities/Player.h"

WorldSelectScene::WorldSelectScene(Scene *scene, World *world) :
	m_world(world)
{
	LOG("Scene: Select World");
	
	// Setup canvas
	Canvas *canvas = new Canvas(scene, 800, 600);
	canvas->updateSize();

	FileSystemIterator itr("saves:/Overworld/", "*", FileSystemIterator::DIRECTORIES);
	int i = 0;
	while(itr)
	{
		string worldDir = itr.next();
		Button *button = new Button(scene, IniFile(worldDir + "/world.ini").getValue("world", "name"), function<void()>(bind(&WorldSelectScene::worldClicked, this)), canvas);
		button->setAnchor(Vector2(0.5f, 0.5f));
		button->setPosition(Vector2(0.0f, -0.3f + i++*0.1f));
		button->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
		button->setUserData(new string(worldDir));
	}

	Button *cwBtn = new Button(scene, "Create World", function<void()>(bind(&WorldSelectScene::showCreateWorld, this)), canvas);
	cwBtn->setAnchor(Vector2(0.5f, 1.0f));
	cwBtn->setPosition(Vector2(0.0f, -0.1f));
	cwBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
}

void WorldSelectScene::worldClicked()
{
	// Create a player
	//Player *p = new Player();
	//p->getBody()->setPosition(0, 0);

	// Go to game
	//m_game->setState(new InGameScene());
	//SceneManager::setScene(new GameScene(m_world));
}

void WorldSelectScene::showCreateWorld()
{
	//SceneManager::setScene(new WorldCreateScene(m_world));
}
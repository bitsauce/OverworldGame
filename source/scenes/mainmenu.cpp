#include "MainMenu.h"
#include "WorldSelect.h"

#include "Gui/Button.h"
#include "Gui/Canvas.h"

#include "Constants.h"

int TITLE_BLOCKS[4][37] = {
	{ 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0 },
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1 },
	{ 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1 },
	{ 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0 }
};

MainMenuScene::MainMenuScene(Scene *scene, World *world) :
	m_world(world)
{
	LOG("Scene: Main menu");
	
	// Setup canvas
	Canvas *canvas = new Canvas(scene, 800, 600);
	canvas->setSize(Vector2(1.0f, 1.0f));

	// Create and setup menu buttons
	Button *spBtn = new Button(scene, "Singleplayer", function<void()>(bind(&MainMenuScene::showWorldSelectScene, this)), canvas);
	Button *mpBtn = new Button(scene, "Multiplayer", function<void()>(), canvas);
	
	spBtn->setPosition(Vector2(0.0f, -0.1f));
	spBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	spBtn->setAnchor(Vector2(0.5f, 0.5f));

	mpBtn->setPosition(Vector2(0.0f, 0.0f));
	mpBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	mpBtn->setAnchor(Vector2(0.5f, 0.5f));
}

void MainMenuScene::showWorldSelectScene()
{
	//SceneManager::setScene(new WorldSelectScene(m_world));
}
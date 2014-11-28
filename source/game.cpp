#include "game.h"
#include "constants.h"

#include "game/world.h"
#include "game/blockdata.h"

#include "scenes/mainmenu.h"
#include "scenes/worldselect.h"
#include "scenes/worldcreate.h"
#include "scenes/gamescene.h"

#include "gui/canvas.h"
#include "gui/uiobject.h"

#include "game/camera.h"

UiObject *canvas = 0;
MainMenuScene *scene::mainMenu = 0;
WorldSelectScene *scene::worldSelect = 0;
WorldCreateScene *scene::worldCreate = 0;
GameScene *scene::game = 0;

void gameMain()
{
	scene::mainMenu = new MainMenuScene;
	scene::worldSelect = new WorldSelectScene;
	scene::worldCreate = new WorldCreateScene;
	scene::game = new GameScene;

	// Set some key bindings
	XInput::bind(XD_KEY_ESCAPE, function<void()>(XEngine::exit));
	//XInput::bind(XD_KEY_I, &delegate<void()>(Camera::zoomIn));
	//XInput::bind(XD_KEY_O, &delegate<void()>(Camera::zoomOut));
	//XInput::bind(XD_KEY_C, &delegate<void()>(Debug::debugFunc));
	//XInput::bind(XD_BACKSPACE, &delegate<void()>());

	BlockData::init();
	World::init();

	//Camera::lookAt(Vector2(0.0f, 0.0f));

	// Set b2d world scale
	//Box2D::setGravity(0, 40);
	//Box2D::setWorldScale(BLOCK_PX);

	XWindow::setSize(Vector2i(1280, 720));

	XEngine::pushScene(scene::game);
}

void gameEnd()
{
	delete canvas;
	delete scene::mainMenu;
	delete scene::worldSelect;
	delete scene::worldCreate;
	delete scene::game;
}

void gameDraw()
{
	Game::draw();
}

void gameUpdate()
{
	Game::update();
}
#include "game.h"
#include "constants.h"

#include "game/world.h"
#include "game/blockdata.h"

#include "gui/canvas.h"
#include "gui/uiobject.h"

#include "game/camera.h"

UiObject *canvas = 0;

void gameMain()
{
	Game::init();

	// Set some key bindings
	XInput::bind(XD_KEY_ESCAPE, function<void()>(XEngine::exit));
	//XInput::bind(XD_KEY_I, &delegate<void()>(Camera::zoomIn));
	//XInput::bind(XD_KEY_O, &delegate<void()>(Camera::zoomOut));
	//XInput::bind(XD_KEY_C, &delegate<void()>(Debug::debugFunc));
	//XInput::bind(XD_BACKSPACE, &delegate<void()>());

	BlockData::init();
	World::init();

	XWindow::setSize(Vector2i(1280, 720));

	Game::gotoScene(SCENE_GAME);
}

void gameEnd()
{
	Game::destroy();
	delete canvas;
}

void gameDraw()
{
	Game::draw();
}

void gameUpdate()
{
	Game::update();
}
#include "game.h"
#include "constants.h"


#include "scenes/mainmenu.h"

#include "gui/canvas.h"
#include "gui/uiobject.h"

UiObject *canvas = 0;
MainMenu *scene::mainMenu = 0;

void gameMain()
{
	scene::mainMenu = new MainMenu;

	canvas = new Canvas();
	canvas->setSize(Vector2(1.0f, 1.0f));

	XInput::bind(XD_KEY_ESCAPE, &delegate<void()>(XEngine::exit));
	//XInput::bind(XD_KEY_I, &delegate<void()>(Camera::zoomIn));
	//XInput::bind(XD_KEY_O, &delegate<void()>(Camera::zoomOut));
	//XInput::bind(XD_KEY_C, &delegate<void()>(Debug::debugFunc));

	//Box2D::setGravity(0, 40);
	//Box2D::setWorldScale(TILE_PX);

	XWindow::setSize(Vector2i(1280, 720));

	XEngine::pushScene(scene::mainMenu);
}

void gameEnd()
{
	delete canvas;
	delete scene::mainMenu;
}

void gameDraw()
{
	XBatch batch;

	XShape shape(Rect(XInput::getPosition().x, XInput::getPosition().y, 50, 50));
	shape.setFillColor(Vector4(1,0,0,1));
	shape.draw(&batch);

	//((XFont*)XAssetManager::getAsset("font_large"))->draw(&batch, Vector2(25.0f), string("FPS: ") + util::floatToStr(XGraphics::getFPS()));

	XGraphics::renderBatch(batch);
}

void gameUpdate()
{
}
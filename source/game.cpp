#include "game.h"
#include "constants.h"

#include "scenes/mainmenu.h"
#include "gui/uiobject.h"

UiObject *uiRoot = 0;

XFont *font::large = 0;
MainMenu *scene::mainMenu = 0;
XTexture *texture::menuButton = 0;

void gameMain()
{
	scene::mainMenu = new MainMenu;

	texture::menuButton = new XTexture(":/sprites/gui/menu_button.png");

	uiRoot = new UiObject(0);
	uiRoot->setSize(Vector2(1.0f, 1.0f));

	font::large = new XFont(string("Arial Bold"), 24);

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
	delete scene::mainMenu;
	delete font::large;
	delete texture::menuButton;
}

void gameDraw()
{
	XBatch batch;

	XShape shape(Rect(XInput::getPosition().x, XInput::getPosition().y, 50, 50));
	shape.setFillColor(Vector4(1,0,0,1));
	shape.draw(&batch);

	font::large->draw(&batch, Vector2(25.0f), string("FPS: ") + util::floatToStr(XGraphics::getFPS()));

	XGraphics::renderBatch(batch);
}

void gameUpdate()
{
}
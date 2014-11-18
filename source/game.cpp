#include "game.h"
#include "constants.h"

#include "scenes/mainmenu.h"
#include "gui/uiobject.h"

UiObject *canvas = 0;

XFont *font::large = 0;
MainMenu *scene::mainMenu = 0;
XTexture *texture::menuButton = 0;

class Canvas : public UiObject
{
public:
	Canvas() :
		UiObject(0)
	{
		m_anchor.set(0.5f, 0.5f);
	}

	void update()
	{
		if(XWindow::getSize().x > XWindow::getSize().y) {
			// TODO: Fit height and use aspect ratio
		}
		else {
			// TODO: Fit width and use inverse aspect ratio
		}
		m_rect.size = Vector2(min(CANVAS_WIDTH, XWindow::getSize().x), min(CANVAS_HEIGHT, XWindow::getSize().y))/Vector2(XWindow::getSize());
		UiObject::update();
	}

	void draw(XBatch *batch)
	{
		Vector2 position = getPosition();
		Vector2 size = getSize();

		XShape shape(getRect());
		shape.setFillColor(Vector4(0,0,1,1));
		shape.draw(batch);
	}
};

void gameMain()
{
	scene::mainMenu = new MainMenu;

	texture::menuButton = new XTexture(":/sprites/gui/menu_button.png");

	canvas = new Canvas();
	canvas->setSize(Vector2(1.0f, 1.0f));

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
	delete canvas;
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
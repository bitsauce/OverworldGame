#include "DialogBoxState.h"
#include "Gui/Gui.h"
#include "Game/Game.h"

class Background2 : public UiObject
{
public:
	Background2(UiObject *parent) :
		UiObject(parent)
	{
		setSize(1.f, 1.f);
	}

	void onDraw(DrawEvent *e)
	{
		e->getGraphicsContext()->drawRectangle(getDrawRect(), Color(30, 30, 30, 127));
		UiObject::onDraw(e);
	}
};

class Background3 : public UiObject
{
public:
	Background3(UiObject *parent) :
		UiObject(parent)
	{
		setSize(1.f, 1.f);
	}

	void onDraw(DrawEvent *e)
	{
		e->getGraphicsContext()->drawRectangle(getDrawRect(), Color(222, 222, 222, 255));
		UiObject::onDraw(e);
	}
};

DialogBoxState::DialogBoxState(Overworld *game, Window *window, const string &message) :
	GameState(GAME_STATE_DIALOG_BOX, true)
{
	Canvas *canvas = new Canvas(window);
	addChildLast(canvas);

	Background2 *bg = new Background2(canvas);

	m_aspectRatioContainer = new AspectRatioContainer(bg, window, 1280, 1280.0f / 720.0f);
	m_aspectRatioContainer->setAnchor(0.5f, 0.5f);
	m_aspectRatioContainer->setOrigin(0.5f, 0.5f);

	Background3 *bg3 = new Background3(m_aspectRatioContainer);
	bg3->setSize(350.0f / 1280.0f, 230.0f / 720.0f);
	bg3->setAnchor(0.5f, 0.5f);
	bg3->setOrigin(0.5f, 0.5f);

	m_buttonOk = new Button(bg3, 60, 40);
	m_buttonOk->setText("OK");
	m_buttonOk->setSize(60.0f / 350.0f, 40.0f / 230.0f);
	m_buttonOk->setAnchor(0.5f, 1.0f);
	m_buttonOk->setOrigin(0.5f, 1.0f);
	m_buttonOk->setPosition(0.0f, -20.0f / 230.0f);
	m_buttonOk->setOnClickCallback(bind(&Overworld::popState, game));

	m_message = new Label(bg3);
	m_message->setText(message);
	m_message->setSize(300.0f / 350.0f, 150.0f / 230.0f);
	m_message->setAnchor(0.5f, 0.0f);
	m_message->setOrigin(0.5f, 0.0f);
	m_message->setPosition(0.0f, 30.0f / 230.0f);
}
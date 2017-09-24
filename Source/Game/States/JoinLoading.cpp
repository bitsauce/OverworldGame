#include "JoinLoading.h"
#include "Game/Game.h"

#include "Gui/Gui.h"
#include "InGameState.h"
#include "DialogBoxState.h"

class LoadingBar : public UiObject
{
public:
	LoadingBar(UiObject *parent) :
		UiObject(parent),
		m_progress(0.0f)
	{
	}

	void onDraw(DrawEvent *e)
	{
		RectI drawRect = getDrawRect();
		e->getGraphicsContext()->drawRectangle(drawRect.position.x, drawRect.position.y, drawRect.size.x * m_progress, drawRect.size.y, Color::Green);
	}

	float m_progress;
};

JoinLoading::JoinLoading(Overworld *game, const string &ip) :
	GameState(GAME_STATE_TEST_MENU, false)
{
	Window *window = game->getWindow();

	Canvas *canvas = new Canvas(window);
	addChildLast(canvas);

	GradientBackground *background = new GradientBackground(canvas);

	AspectRatioContainer *aspectRatioContainer = new AspectRatioContainer(background, window, 1280, 1280.0f / 720.0f);
	aspectRatioContainer->setAnchor(0.5f, 0.5f);
	aspectRatioContainer->setOrigin(0.5f, 0.5f);

	m_loadingBar = new LoadingBar(aspectRatioContainer);
	m_loadingBar->setSize(720.0f / 1280.0f, 35.0f / 720.0f);
	m_loadingBar->setPosition(0.0f, 0.0f);
	m_loadingBar->setOrigin(0.5f, 0.5f);
	m_loadingBar->setAnchor(0.5f, 0.5f);

	Client *client = game->getClient();
	client->join("Bitsauce", ip.empty() ? "127.0.0.1" : ip, DEFAULT_PORT);
}

void JoinLoading::onTick(TickEvent *e)
{
	Overworld *game = dynamic_cast<Overworld*>(Game::Get());
	Client *client = game->getClient();
	client->onEvent(e);
	m_loadingBar->m_progress = client->getJoinProgress();
	GameState::onTick(e);

	if(client->isJoinFinalized())
	{
		game->pushState(new InGameState(game, client->getWorld()));
	}
}

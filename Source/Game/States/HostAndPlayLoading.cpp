#include "HostAndPlayLoading.h"
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

HostAndPlayLoading::HostAndPlayLoading(Overworld *game, const string &name) :
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

	Server *server = game->getServer();
	server->host("Debug", DEFAULT_PORT);

	Client *client = game->getClient();
	client->join(name.empty() ? "Bitsauce" : name, "127.0.0.1", DEFAULT_PORT);
}

void HostAndPlayLoading::onTick(TickEvent *e)
{
	Overworld *game = dynamic_cast<Overworld*>(Game::Get());
	Server *server = game->getServer();
	Client *client = game->getClient();
	server->onEvent(e);
	client->onEvent(e);
	m_loadingBar->m_progress = client->getJoinProgress();
	GameState::onTick(e);

	if(client->isJoinFinalized())
	{
		game->pushState(new InGameState(game, client->getWorld()));
	}
}

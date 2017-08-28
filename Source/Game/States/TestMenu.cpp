#include "TestMenu.h"
#include "Game/Game.h"

#include "Gui/Gui.h"
#include "InGameState.h"
#include "DialogBoxState.h"

TestMenu::TestMenu(Overworld *game) :
	GameState(GAME_STATE_TEST_MENU, false)
{
	Window *window = game->getWindow();

	Canvas *canvas = new Canvas(window);
	addChildLast(canvas);

	GradientBackground *background = new GradientBackground(canvas);

	AspectRatioContainer *aspectRatioContainer = new AspectRatioContainer(background, window, 1280, 1280.0f / 720.0f);
	aspectRatioContainer->setAnchor(0.5f, 0.5f);
	aspectRatioContainer->setOrigin(0.5f, 0.5f);

	LineEdit *ipLineEdit = new LineEdit(aspectRatioContainer, 230, 40);
	ipLineEdit->setDefaultText("IP Address");
	ipLineEdit->setSize(230.0f / 1280.0f, 40.0f / 720.0f);
	ipLineEdit->setPosition(0.0f, 0.0f);
	ipLineEdit->setOrigin(0.5f, 1.0f);
	ipLineEdit->setAnchor(0.5f, 0.3f);
	ipLineEdit->setAcceptFunc(bind(&Overworld::pushState, game, new DialogBoxState(game, window, "Loading")));

	Button *hostAndPlay = new Button(aspectRatioContainer, 230, 40);
	hostAndPlay->setText("Host and Play");
	hostAndPlay->setSize(230.0f / 1280.0f, 40.0f / 720.0f);
	hostAndPlay->setPosition(0.0f, 0.2f);
	hostAndPlay->setOrigin(0.5f, 1.0f);
	hostAndPlay->setAnchor(0.5f, 0.3f);
	hostAndPlay->setOnClickCallback(bind(&TestMenu::navigateTo, this, "InGame"));
}

void TestMenu::navigateTo(const string &name)
{
	Overworld *game = dynamic_cast<Overworld*>(Game::Get());

	Server *server = game->getServer();
	server->host("Debug", DEFAULT_PORT);

	Client *client = game->getClient();
	client->join("Bitsauce", "127.0.0.1", DEFAULT_PORT);

	while(!client->isJoinFinalized())
	{
		TickEvent e(1.0/30.0);
		server->onEvent(&e);
		client->onEvent(&e);
	}

	game->pushState(new InGameState(game, client->getWorld()));
}

#include "TestMenu.h"
#include "Game/Game.h"

#include "Gui/Gui.h"
#include "HostAndPlayLoading.h"
#include "JoinLoading.h"
#include "DialogBoxState.h"

TestMenu::TestMenu(Overworld *game) :
	GameState(GAME_STATE_TEST_MENU, false),
	m_game(game)
{
	Window *window = game->getWindow();

	Canvas *canvas = new Canvas(window);
	addChildLast(canvas);

	GradientBackground *background = new GradientBackground(canvas);

	AspectRatioContainer *aspectRatioContainer = new AspectRatioContainer(background, window, 1280, 1280.0f / 720.0f);
	aspectRatioContainer->setAnchor(0.5f, 0.5f);
	aspectRatioContainer->setOrigin(0.5f, 0.5f);

	m_ipLineEdit = new LineEdit(aspectRatioContainer, 230, 40);
	m_ipLineEdit->setSize(230.0f / 1280.0f, 40.0f / 720.0f);
	m_ipLineEdit->setPosition(0.0f, 0.0f);
	m_ipLineEdit->setOrigin(0.5f, 1.0f);
	m_ipLineEdit->setAnchor(0.5f, 0.3f);
	m_ipLineEdit->setDefaultText("127.0.0.1");
	m_ipLineEdit->setAcceptFunc(bind(&Overworld::pushState, game, new DialogBoxState(game, window, "Loading")));


	m_nameLineEdit = new LineEdit(aspectRatioContainer, 230, 40);
	m_nameLineEdit->setSize(230.0f / 1280.0f, 40.0f / 720.0f);
	m_nameLineEdit->setPosition(0.0f, -50.0f / 720.0f);
	m_nameLineEdit->setOrigin(0.5f, 1.0f);
	m_nameLineEdit->setAnchor(0.5f, 0.3f);
	m_nameLineEdit->setDefaultText("Player name");

	Button *join = new Button(aspectRatioContainer, 230, 40);
	join->setSize(230.0f / 1280.0f, 40.0f / 720.0f);
	join->setPosition(0.0f, 50.0f / 720.0f);
	join->setOrigin(0.5f, 1.0f);
	join->setAnchor(0.5f, 0.3f);
	join->setText("Join");
	join->setOnClickCallback(bind(&TestMenu::navigateTo, this, "Join"));

	Button *hostAndPlay = new Button(aspectRatioContainer, 230, 40);
	hostAndPlay->setSize(230.0f / 1280.0f, 40.0f / 720.0f);
	hostAndPlay->setPosition(0.0f, 0.2f);
	hostAndPlay->setOrigin(0.5f, 1.0f);
	hostAndPlay->setAnchor(0.5f, 0.3f);
	hostAndPlay->setText("Host and Play");
	hostAndPlay->setOnClickCallback(bind(&TestMenu::navigateTo, this, "HostAndPlay"));
}

void TestMenu::navigateTo(const string &name)
{
	if(name == "HostAndPlay") m_game->pushState(new HostAndPlayLoading(m_game, m_nameLineEdit->getText()));
	else if(name == "Join") m_game->pushState(new JoinLoading(m_game, m_nameLineEdit->getText(), m_ipLineEdit->getText()));
}

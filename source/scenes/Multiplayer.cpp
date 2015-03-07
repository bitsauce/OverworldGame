#include "MessageIdentifiers.h"
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "PacketPriority.h"
#include "BitStream.h"
#include "NetworkIDObject.h"
#include "NetworkIDManager.h"
#include "PacketLogger.h"

#include "Entities/Player.h"

#include "Multiplayer.h"
#include "SceneManager.h"
#include "GameScene.h"

#include "Networking/Client.h"
#include "Networking/Server.h"

#include "Gui/UiObject.h"
#include "Gui/Button.h"
#include "Gui/LineEdit.h"
#include "Gui/Canvas.h"

#include "Constants.h"

MultiplayerScene::MultiplayerScene(World &world) :
	m_world(world)
{
	LOG("Scene: Multiplayer");
	
	// Setup canvas
	canvas = new Canvas(800, 600);
	canvas->update();
	
	m_ipLineEdit = new LineEdit(canvas);
	m_ipLineEdit->setText("127.0.0.1");
	m_ipLineEdit->setAnchor(Vector2(0.5f, 0.5f));
	m_ipLineEdit->setPosition(Vector2(-0.2f, -0.1f));
	m_ipLineEdit->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	addSceneObject(m_ipLineEdit);
	
	m_portLineEdit = new LineEdit(canvas);
	m_portLineEdit->setText("5555");
	m_portLineEdit->setAnchor(Vector2(0.5f, 0.5f));
	m_portLineEdit->setPosition(Vector2(0.2f, -0.1f));
	m_portLineEdit->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	addSceneObject(m_portLineEdit);
	
	Button *joinBtn = new Button("Join", function<void()>(bind(&MultiplayerScene::join, this)), canvas);
	joinBtn->setAnchor(Vector2(0.5f, 1.0f));
	joinBtn->setPosition(Vector2(0.2f, -0.1f));
	joinBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	addSceneObject(joinBtn);
	
	Button *hostBtn = new Button("Host", function<void()>(bind(&MultiplayerScene::host, this)), canvas);
	hostBtn->setAnchor(Vector2(0.5f, 1.0f));
	hostBtn->setPosition(Vector2(-0.2f, -0.1f));
	hostBtn->setSize(Vector2(300.0f/CANVAS_WIDTH, 70.0f/CANVAS_HEIGHT));
	addSceneObject(hostBtn);
}

void MultiplayerScene::host()
{
	//new Server(util::strToInt(m_portLineEdit->getText()));

	/*RakNet::BitStream bitStream;
	bitStream.Write((RakNet::MessageID)ID_CREATE_ENTITY);
	//bitStream.Write(ENTITY_PLAYER);
	((Server*)Connection::getInstance())->getRakPeer()->SendLoopback((const char*)bitStream.GetData(), bitStream.GetNumberOfBytesUsed());*/
	
	SceneManager::setScene(new GameScene(m_world));
}

void MultiplayerScene::join()
{
	vector<string> ipStrings = util::splitString(m_ipLineEdit->getText(), ".");
	if(ipStrings.size() != 4) return;

	new Client(m_world, m_ipLineEdit->getText(), util::strToInt(m_portLineEdit->getText()));

	SceneManager::setScene(new GameScene(m_world));
}
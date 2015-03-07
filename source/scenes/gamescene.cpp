#include "GameScene.h"
#include "World/World.h"

#include "Gui/UiObject.h"
#include "Gui/Canvas.h"
#include "Gui/GameOverlay.h"

#include "Networking/Server.h"
#include "Networking/Client.h"
#include "Networking/NetworkObject.h"

#include "Constants.h"

GameScene::GameScene(World &world) :
	m_world(world)
{
	LOG("Scene: Game Scene");

	// Setup canvas
	canvas = new Canvas();
	canvas->update();

	RakNet::BitStream bitStream;
	bitStream.Write((RakNet::MessageID)ID_CREATE_ENTITY);
	//bitStream.Write(ENTITY_PLAYER);
	if(Connection::getInstance()->isServer())
	{
		((Server*)Connection::getInstance())->getRakPeer()->SendLoopback((const char*)bitStream.GetData(), bitStream.GetNumberOfBytesUsed());
	}
	else
	{
		((Client*)Connection::getInstance())->sendPacket(&bitStream);
	}
}

GameScene::~GameScene()
{
	// Save and clear
	m_world.save();
	m_world.clear();

	((Connection*)Connection::getInstance())->m_networkObjects.clear();
}
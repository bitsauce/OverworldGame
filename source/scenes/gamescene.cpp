#include "GameScene.h"
#include "World/World.h"

#include "Gui/UiObject.h"
#include "Gui/Canvas.h"
#include "Gui/GameOverlay.h"

#include "Networking/Server.h"

#include "Constants.h"

GameScene::GameScene()
{
	LOG("Scene: Game Scene");

	// Setup canvas
	canvas = new Canvas();
	canvas->update();
	
	new Server(5555);

	RakNet::BitStream bitStream;
	bitStream.Write((RakNet::MessageID)ID_CREATE_ENTITY);
	//bitStream.Write(ENTITY_PLAYER);
	((Server*)Connection::getInstance())->getRakPeer()->SendLoopback((const char*)bitStream.GetData(), bitStream.GetNumberOfBytesUsed());
}

GameScene::~GameScene()
{
	// Save and clear
	World::save();
	World::clear();
}
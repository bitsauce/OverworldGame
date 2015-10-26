#include "InGameState.h"
#include "Game/Game.h"

InGameState::InGameState(OverworldGame *game) :
	GameState(GAME_STATE_IN_GAME, false),
	m_world(game->getWorld())
{
}

void InGameState::enter()
{
	/*RakNet::BitStream bitStream;
	bitStream.Write((RakNet::MessageID)ID_CREATE_ENTITY);
	//bitStream.Write(ENTITY_PLAYER);
	if(Connection::getInstance()->isServer())
	{
		((Server*)Connection::getInstance())->getRakPeer()->SendLoopback((const char*)bitStream.GetData(), bitStream.GetNumberOfBytesUsed());
	}
	else
	{
		((Client*)Connection::getInstance())->sendPacket(&bitStream);
	}*/

	Input::setContext(Input::getContext("game"));
}

void InGameState::update(const float delta)
{
	m_world->update(delta);
	m_scene.update(delta);
}

void InGameState::draw(SpriteBatch *spriteBatch, const float alpha)
{
	m_world->draw(spriteBatch, alpha);
	m_scene.draw(spriteBatch, alpha);
}
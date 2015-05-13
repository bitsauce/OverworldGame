#include "InGameState.h"
#include "Game/Game.h"

InGameState::InGameState(Game *game) :
	GameState(GAME_STATE_IN_GAME, false, new Scene()),
	m_world(game->getWorld())
{
	m_scene->getCanvas()->updateSize();
	game->m_gameOverlay = new GameOverlay(game, m_scene);
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
}

void InGameState::update(const float delta)
{
	m_world->update(delta);
	m_scene->update(delta);
}

void InGameState::draw(SpriteBatch *spriteBatch, const float alpha)
{
	m_world->draw(spriteBatch, alpha);
	m_scene->draw(spriteBatch, alpha);
}
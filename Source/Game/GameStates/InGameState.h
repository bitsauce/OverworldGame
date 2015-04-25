#ifndef IN_GAME_STATE_H
#define IN_GAME_STATE_H

#include "Config.h"
#include "GameState.h"
#include "Game/Scene.h"
#include "Gui/Canvas.h"

class World;
class Scene;

class InGameState : public GameState
{
public:
	InGameState(Game &game) :
		GameState(GAME_STATE_IN_GAME, false, new Scene()),
		m_world(game.getWorld())
	{
	}

	void enter()
	{
		// Setup canvas
		m_scene->getCanvas()->updateSize();

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

	void update(const float delta)
	{
		m_world->update(delta);
		m_scene->update(delta);
	}

	void draw(SpriteBatch *spriteBatch, const float alpha)
	{
		m_world->draw(spriteBatch, alpha);
		m_scene->draw(spriteBatch, alpha);
	}

private:
	World *m_world;
};

#endif // IN_GAME_STATE_H
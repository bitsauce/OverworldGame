#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Config.h"

enum GameStateID
{
	GAME_STATE_MULTIPLAYER,
	GAME_STATE_IN_GAME
};

class GameState
{
	friend class Game;
protected:
	GameState(const GameStateID id, const bool transparent, Scene *scene) :
		m_id(id),
		m_transparent(transparent),
		m_scene(scene)
	{
	}

	virtual void update(const float delta) {}
	virtual void draw(SpriteBatch *spriteBatch, const float alpha) {}

	virtual void enter() {}
	virtual void leave() {}
	
public:

	GameStateID getID() const { return m_id; }
	bool isTransparent() const { return m_transparent; }
	Scene *getScene() { return m_scene; }

private:
	const GameStateID m_id;
	const bool m_transparent;

protected:
	Scene *m_scene;
};

#endif // GAME_STATE_H
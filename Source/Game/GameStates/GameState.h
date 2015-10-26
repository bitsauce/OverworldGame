#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Config.h"
#include "../Scene.h"

enum GameStateID
{
	GAME_STATE_MULTIPLAYER,
	GAME_STATE_IN_GAME,
	GAME_STATE_DEBUG
};

class GameState
{
	friend class OverworldGame;
protected:
	GameState(const GameStateID id, const bool transparent) :
		m_id(id),
		m_transparent(transparent)
	{
		m_scene.getCanvas()->updateSize();
	}

	virtual void update(const float delta) {}
	virtual void draw(SpriteBatch *spriteBatch, const float alpha) {}

	virtual void enter() {}
	virtual void leave() {}

	//void processInput();
	//void draw();
	
public:

	GameStateID getID() const { return m_id; }
	bool isTransparent() const { return m_transparent; }
	Scene *getScene() { return &m_scene; }

private:
	const GameStateID m_id;
	const bool m_transparent;

protected:
	Scene m_scene;
};

#endif // GAME_STATE_H
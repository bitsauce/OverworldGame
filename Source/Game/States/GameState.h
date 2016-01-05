#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Config.h"

enum GameStateID;
class Canvas;

class GameState : public GameObject
{
	friend class OverworldGame;
protected:
	GameState(const GameStateID id, const bool transparent);

	virtual void onEnter() { }
	virtual void onLeave() { }

	//void processInput();
	//void draw();
	
public:

	GameStateID getID() const { return m_id; }
	bool isTransparent() const { return m_transparent; }
	//Scene *getScene() { return m_scene; }

private:
	const GameStateID m_id;
	const bool m_transparent;

protected:
	Canvas *m_canvas;
};

#endif // GAME_STATE_H
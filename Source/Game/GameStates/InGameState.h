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
	InGameState(OverworldGame *game);

	void enter();
	void update(const float delta);
	void draw(SpriteBatch *spriteBatch, const float alpha);

private:
	World *m_world;
};

#endif // IN_GAME_STATE_H
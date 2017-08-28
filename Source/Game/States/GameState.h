#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Config.h"

enum GameStateID;
class Canvas;

class GameState : public SceneObject
{
	friend class Overworld;
protected:
	GameState(const GameStateID id, const bool transparent);

	virtual void onTransitionBegin() { }

	virtual void onDraw(DrawEvent *e);
	virtual void onWindowSizeChanged(WindowEvent *e);

public:
	GameStateID getID() const { return m_id; }
	bool isTransparent() const { return m_transparent; }
	RenderTarget2D *getRenderTarget() const { return m_renderTarget; }

private:
	const GameStateID m_id;
	const bool m_transparent;
	RenderTarget2D *m_renderTarget;
};

#endif // GAME_STATE_H
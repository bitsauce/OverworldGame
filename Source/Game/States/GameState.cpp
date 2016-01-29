#include "GameState.h"
#include "Gui/Canvas.h"

GameState::GameState(const GameStateID id, const bool transparent) :
	m_id(id),
	m_transparent(transparent)
{
	m_canvas = new Canvas(Game::GetInstance()->getWindow(), 1280, 720);
	addChildLast(m_canvas);
}

#include "GameState.h"
#include "Gui/Canvas.h"

GameState::GameState(const GameStateID id, const bool transparent) :
	m_id(id),
	m_transparent(transparent)
{
	m_canvas = new Canvas();
	addChildLast(m_canvas);
	m_canvas->updateSize();
}

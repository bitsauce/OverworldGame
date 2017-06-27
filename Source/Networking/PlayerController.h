#pragma once

#include "Config.h"
#include "Controller.h"

class OverworldGame;

class PlayerController : public Controller
{
public:
	PlayerController(OverworldGame *game, const bool local);
	~PlayerController();

private:
	OverworldGame *m_game;

	void updateInputState(InputEvent *e, int type);
	void updateUseItemState(InputEvent *e);
};
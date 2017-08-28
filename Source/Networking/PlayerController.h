#pragma once

#include "Config.h"
#include "Controller.h"

class Overworld;

class PlayerController : public Controller
{
public:
	PlayerController(Overworld *game, const bool local);
	~PlayerController();

private:
	Overworld *m_game;

	void updateInputState(InputEvent *e, int type);
	void updateUseItemState(InputEvent *e);
};
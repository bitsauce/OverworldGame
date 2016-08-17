#pragma once

#include "Config.h"
#include "Controller.h"
#include "Networking/NetworkObject.h"
#include "Entity.h"

class OverworldGame;

class PlayerController : public Controller
{
public:
	PlayerController(OverworldGame *game, const bool local);
	~PlayerController();

private:
	OverworldGame *m_game;

	void updateInputState(KeyEvent *e, int type);
	void updateUseItemState(KeyEvent *e);
};
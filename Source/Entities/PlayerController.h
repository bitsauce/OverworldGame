#pragma once

#include "Config.h"
#include "Controller.h"
#include "Networking/NetworkObject.h"
#include "DynamicEntity.h"

class OverworldGame;

class PlayerController : public Controller
{
public:
	PlayerController(OverworldGame *game, const bool local);

private:
	OverworldGame *m_game;

	void updateClientInputState(KeyEvent *e, int type);
	void updateClientUseItemState(KeyEvent *e);
};
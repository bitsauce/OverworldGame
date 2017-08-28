#pragma once

#include "Config.h"
#include "GameState.h"
#include "Gui/UiObjects/Canvas.h"

class World;
class Connection;

class TestMenu : public GameState
{
public:
	TestMenu(Overworld *game);
	void navigateTo(const string &name);
};

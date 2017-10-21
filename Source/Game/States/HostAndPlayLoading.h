#pragma once

#include "Config.h"
#include "GameState.h"
#include "Gui/UiObjects/Canvas.h"

class World;
class Connection;

class HostAndPlayLoading : public GameState
{
public:
	HostAndPlayLoading(Overworld *game, const string &name);
	void onTick(TickEvent *e);

private:
	class LoadingBar *m_loadingBar;
};

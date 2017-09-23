#pragma once

#include "Config.h"
#include "GameState.h"
#include "Gui/UiObjects/Canvas.h"

class World;
class Connection;

class JoinLoading : public GameState
{
public:
	JoinLoading(Overworld *game, const string &ip);
	void onTick(TickEvent *e);

private:
	class LoadingBar *m_loadingBar;
};

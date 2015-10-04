//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include "Config.h"
#include "Game/Game.h"

#if defined(X2D_WINDOWS) && defined(X2D_DEBUG)
//#include <vld.h> // Visual Leak Detector
#endif

// Win32 entry point
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// Set engine flags
	int flags = 0;
	flags |= XD_EXPORT_LOG;
	flags |= XD_RUN_IN_BACKGROUND;
	flags |= XD_BLOCK_BACKGROUND_INPUT;

	Engine *engine = CreateEngine();

	Config config;
	Game *game = new Game();
	config.mainFunc = bind(&Game::main, game, placeholders::_1);
	config.updateFunc = bind(&Game::update, game, placeholders::_1);
	config.drawFunc = bind(&Game::draw, game, placeholders::_1, placeholders::_2);
	config.endFunc = bind(&Game::exit, game);
	config.workDir = "..\\..\\..\\Content\\";
	config.flags = flags;

	if(engine->init(config) != X2D_OK)
	{
		delete engine;
		return -1;
	}

	int r = engine->run();
	delete engine;
	return r;
}
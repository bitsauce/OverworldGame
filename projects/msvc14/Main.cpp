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
	// Create game object
	OverworldGame game;
	game.setFlags(XD_EXPORT_LOG | XD_RUN_IN_BACKGROUND | XD_BLOCK_BACKGROUND_INPUT);
	game.setWorkDir("../../../Content/");
	game.setInputConfig(":/KeyBinds.xml");

	// Create and init engine
	Engine *engine = CreateEngine();
	if(engine->init(&game) != X2D_OK)
	{
		// Engine init failed
		delete engine;
		return -1;
	}

	// Run game
	int r = engine->run();
	delete engine;
	return r;
}
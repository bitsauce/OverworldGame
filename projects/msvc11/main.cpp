//       ____  ____     ____                        _____             _            
// __  _|___ \|  _ \   / ___| __ _ _ __ ___   ___  | ____|_ __   __ _(_)_ __   ___ 
// \ \/ / __) | | | | | |  _ / _  |  _   _ \ / _ \ |  _| |  _ \ / _  | |  _ \ / _ \
//  >  < / __/| |_| | | |_| | (_| | | | | | |  __/ | |___| | | | (_| | | | | |  __/
// /_/\_\_____|____/   \____|\__ _|_| |_| |_|\___| |_____|_| |_|\__, |_|_| |_|\___|
//                                                              |___/     
//				Originally written by Marcus Loo Vergara (aka. Bitsauce)
//									2011-2014 (C)

#include <x2d/x2d.h>
#include "game.h"
#include "game/gamemanager.h"

#if defined(X2D_WINDOWS) && defined(X2D_DEBUG)
//#include <vld.h> // Visual Leak Detector
#endif

// Win32 entry point
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	// Process the command-line
	int flags = 0;
	string workDir;
	for(int i = 0; i < __argc; i++)
	{
		if(__argv[i][0] == '-')
		{
			switch(__argv[i][1])
			{
			case 'v': flags |= xd::XD_EXPORT_LOG; break;
			case 'w': workDir = string(__argv[i]+3); break;
			}
		}
	}
	flags |= xd::XD_EXPORT_LOG; // For now we force this flag

	xd::Engine *engine = xd::CreateEngine();

	xd::Config config;
	config.main = &GameManager::main;
	config.update = &GameManager::update;//gameUpdate;
	config.draw = &GameManager::draw;
	config.end = &GameManager::exit;
#ifdef X2D_DEBUG
	config.workDir = "..\\..\\game\\";
#endif
	config.flags = flags;

	if(engine->init(config) != xd::X2D_OK)
	{
		delete engine;
		return -1;
	}

	int r = engine->run();
	delete engine;
	return r;
}
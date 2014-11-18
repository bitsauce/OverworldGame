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

#if defined(X2D_WINDOWS) && defined(X2D_DEBUG)
#include <vld.h> // Visual Leak Detector
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
			case 'v': flags |= XD_EXPORT_LOG; break;
			case 'w': workDir = string(__argv[i]+3); break;
			}
		}
	}
	flags |= XD_EXPORT_LOG; // For now we force this flag

	XEngine *engine = CreateEngine();

	XConfig config;
	config.main = &gameMain;
	config.update = &gameUpdate;
	config.draw = &gameDraw;
	config.end = &gameEnd;
#ifdef X2D_DEBUG
	config.workDir = "C:\\Users\\Marcus\\Documents\\GitHub\\Overworld\\";
	//config.workDir = "C:\\Users\\Marcus\\Documents\\GitHub\\x2D-Game-Engine\\examples\\Debug\\";
	//config.workDir = "C:\\Users\\Marcus\\Dropbox\\Game Development\\Projects\\BlockShadows\\";
#else
	config.workDir = workDir.empty() ? _getcwd(0, 0) : workDir.c_str(); // TODO: The engine should do this by itself
#endif
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
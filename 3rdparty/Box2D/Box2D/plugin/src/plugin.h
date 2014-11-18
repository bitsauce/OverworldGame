#ifndef PLUGIN_H
#define PLUGIN_H

#include <x2d/x2d.h>

// Default plugin definition
extern "C"
{
	// Plugin factory function
	__declspec(dllexport) int CreatePlugin(asIScriptEngine*);
 
	// Plugin cleanup function
	__declspec(dllexport) void ReleasePlugin();
}

#endif // PLUGIN_H
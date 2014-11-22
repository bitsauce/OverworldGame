#ifndef WORLD_H
#define WORLD_H

#include <x2d/x2d.h>

class World
{
public:
	static void save();
	static void clear();
	static string getWorldPath();
};

#endif // WORLD_H
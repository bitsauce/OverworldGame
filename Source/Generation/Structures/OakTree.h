#ifndef OAK_TREE_H
#define OAK_TREE_H

#include "Generation/Structure.h"

class OakTree : public Structure
{
public:
	OakTree(const int x, const int y) :
		Structure(x, y)
	{
	}

	void place(WorldGenerator *worldGenerator, StructurePlacer *structPlacer);
};

#endif // OAK_TREE_H
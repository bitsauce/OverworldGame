#ifndef STRUCTURE_H
#define STRUCTURE_H
//>REMOVE ME<
#include "Constants.h"
#include "Generator.h"

class Structure
{
public:
	Structure(const int x, const int y);

	virtual void place(WorldGenerator *worldGenerator, StructurePlacer *structPlacer) { }

	int m_x, m_y;
};

#endif // STRUCTURE_H
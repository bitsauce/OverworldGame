#pragma once

#include "Formation.h"

class ChunkGenerator;

class OakForest : public Formation
{
public:
	OakForest(ChunkGenerator *gen) : Formation(gen, 1, 23), m_generator(gen) { }

	void generate(const int formX, const int formY, list<FormationElement*> &elements);

private:
	ChunkGenerator *m_generator;
};
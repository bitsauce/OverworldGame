#pragma once

#include "Formation.h"

class OakForest : public Formation
{
public:
	OakForest() : Formation(1, 1) { }

	void generate(const int formX, const int formY, BlockID *blocks);
};
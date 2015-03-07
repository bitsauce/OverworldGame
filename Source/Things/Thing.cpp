#include "Thing.h"
#include "Constants.h"

Thing::Thing(const int x, const int y) :
	GameObject(PRIORITY_BLOCK_ENTITY),
	m_x(x),
	m_y(y)
{
}
#ifndef BLOCK_ENTITY_H
#define BLOCK_ENTITY_H

#include "game/gameobject.h"

class BlockEntity : public GameObject
{
public:
	BlockEntity(const uint width, const uint height);

	void move(const int x, const int y);
	bool place();

	int getX() const { return m_x; }
	int getY() const { return m_y; }
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	bool isPlaced() const { return m_placed; }
	bool isPlaceable() const { return m_placeable; }
	
protected:
	virtual void moveEvent() { }
	virtual void placeEvent() { }

private:
	int m_x, m_y;
	int m_width, m_height;
	bool m_placed, m_placeable;
};

#endif // BLOCK_ENTITY_H
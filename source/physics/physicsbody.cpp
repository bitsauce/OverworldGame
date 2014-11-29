#include "physicsbody.h"
#include "constants.h"

#include "game/world.h"
#include "terrain/terrain.h"

#define PHYSICS_GRAVITY 1.0f

PhysicsBody::PhysicsBody() :
	GameObject(DRAW_ORDER_BLOCK_ENTITY),
	m_velocity(0.0f, 0.0f),
	m_position(0.0f, 0.0f),
	m_rotation(0.0f),
	m_gravityScale(1.0f),
	m_terrain(World::getTerrain())
{
}

void PhysicsBody::update()
{
	m_velocity.y += PHYSICS_GRAVITY * m_gravityScale;

	m_position.y += m_velocity.y;

	if(m_terrain->getBlockAt(XMath::floor(m_position.x/BLOCK_PXF), XMath::floor(m_position.y/BLOCK_PXF)+1, TERRAIN_LAYER_SCENE) > BLOCK_OCCUPIED)
	{
		m_position.y = XMath::floor(m_position.y/BLOCK_PXF) * BLOCK_PXF;
		m_velocity.y = 0.0f;
	}
}

void PhysicsBody::draw(XBatch *batch)
{
	XShape shape(Rect(m_position, m_size));
	shape.setFillColor(XColor(200, 150, 150, 255));
	shape.draw(batch);
}
#include "Entity.h"
#include "EntityData.h"
#include "Constants.h"
#include "World/World.h"
#include "Game/Game.h"

Entity::Entity(World *world, const Json::Value &attributes) :
	m_connection(world->getConnection()),
	m_world(world),
	m_acceleration(0.0f, 0.0f),
	m_velocity(0.0f, 0.0f),
	m_position(0.0f, 0.0f),
	m_lastPosition(0.0f, 0.0f),
	m_size(1.0f, 1.0f),
	m_rotation(45.0f),
	m_gravityScale(1.0f),
	m_allowRotation(false),
	m_contact(0),
	m_packetDelta(0),
	m_initialized(!attributes.empty())
{
	// JSON format: { "position": { "x": float, "y": float} }
	if(attributes.isMember("position"))
	{
		const Json::Value &position = attributes["position"];
		if(position.isMember("x")) m_lastPosition.x = m_position.x = position["x"].asFloat();
		if(position.isMember("y")) m_lastPosition.y = m_position.y = position["y"].asFloat();
	}

	Vector2I chunkPosition = math::floor(m_position / Vector2F(CHUNK_PXF));
	m_currentChunk = m_world->getTerrain()->getChunkManager()->getChunkAt(chunkPosition.x, chunkPosition.y, true);
	m_currentChunk->m_entities.push_back(this);
}

Entity::~Entity()
{
	Camera *camera = m_world->getCamera();
	if((Entity*)camera->getTargetEntity() == this)
	{
		camera->setTargetEntity(nullptr);
	}
	//m_world->removeEntity(this);
	m_currentChunk->m_entities.remove(this);
}

void Entity::onTick(TickEvent *e)
{
	m_lastContact = m_contact;
	m_lastPosition = m_position;
	m_contact = 0;
	m_acceleration.y += 2.0f * m_gravityScale;
	m_velocity += m_acceleration;
	m_acceleration.set(0.0f, 0.0f);

	if(m_allowRotation)
	{
		m_position.y += m_velocity.y;
		m_position.x += m_velocity.x;
		/*Vector2F points[4];
		m_sprite->getAABB(points);

		// Check down
		if(m_velocity.y > 0.0f)
		{
		for(uint i = 0; i < 4; ++i)
		{
		int x0 = floor(points[i].x/BLOCK_PXF);
		int y0 = floor(points[i].y/BLOCK_PXF);
		int v1 = floor((points[i].y+m_velocity.y)/BLOCK_PXF);

		for(int y = y0; y <= v1 && m_velocity.y > 0.0f; ++y)
		{
		if(m_terrain->getBlockAt(x0, y, WORLD_LAYER_MIDDLE) > BLOCK_ENTITY)
		{
		m_position.y = y * BLOCK_PXF - m_size.y;
		m_velocity.y = 0.0f;
		}
		}
		}
		}*/
	}
	else
	{
		Terrain *terrain = m_world->getTerrain();

		// Apply vertical velocity
		m_position.y += m_velocity.y;

		// Check collision vertically
		if(m_velocity.y > 0.0f)
		{
			int x0 = (int) floor(m_position.x / BLOCK_PXF), x1 = (int) floor((m_position.x + m_size.x - 1) / BLOCK_PXF);
			int y0 = (int) floor((m_position.y + m_size.y - 1) / BLOCK_PXF), y1 = (int) floor((m_position.y + m_size.y - 1 + m_velocity.y) / BLOCK_PXF);
			for(int y = y0; y <= y1 && m_velocity.y > 0.0f; ++y)
			{
				for(int x = x0; x <= x1 && m_velocity.y > 0.0f; ++x)
				{
					if(terrain->isBlockAt(x, y, WORLD_LAYER_MIDDLE))
					{
						m_position.y = y * BLOCK_PXF - m_size.y;
						m_velocity.y = 0.0f;
						m_contact |= SOUTH;
					}
				}
			}
		}
		else if(m_velocity.y < 0.0f)
		{
			int x0 = (int) floor(m_position.x / BLOCK_PXF), x1 = (int) floor((m_position.x + m_size.x - 1) / BLOCK_PXF);
			int y0 = (int) floor(m_position.y / BLOCK_PXF), y1 = (int) floor((m_position.y + m_velocity.y) / BLOCK_PXF);
			for(int y = y0; y >= y1 && m_velocity.y < 0.0f; --y)
			{
				for(int x = x0; x <= x1 && m_velocity.y < 0.0f; ++x)
				{
					if(terrain->isBlockAt(x, y, WORLD_LAYER_MIDDLE))
					{
						m_position.y = y * BLOCK_PXF + BLOCK_PXF;
						m_velocity.y = 0.0f;
						m_contact |= NORTH;
					}
				}
			}
		}

		// Apply horizontal velocity
		m_position.x += m_velocity.x;

		// Check collision horizontally
		if(m_velocity.x > 0.0f)
		{
			int y0 = (int) floor(m_position.y / BLOCK_PXF), y1 = (int) floor((m_position.y + m_size.y - 1) / BLOCK_PXF);
			int x0 = (int) floor((m_position.x + m_size.x - 1) / BLOCK_PXF), x1 = (int) floor((m_position.x + m_size.x - 1 + m_velocity.x) / BLOCK_PXF);
			for(int x = x0; x <= x1 && m_velocity.x > 0.0f; ++x)
			{
				for(int y = y0; y <= y1 && m_velocity.x > 0.0f; ++y)
				{
					if(terrain->isBlockAt(x, y, WORLD_LAYER_MIDDLE))
					{
						m_position.x = x * BLOCK_PXF - m_size.x;
						m_velocity.x = 0.0f;
						m_contact |= EAST;
					}
				}
			}
		}
		else if(m_velocity.x < 0.0f)
		{
			int y0 = (int) floor(m_position.y / BLOCK_PXF), y1 = (int) floor((m_position.y + m_size.y - 1) / BLOCK_PXF);
			int x0 = (int) floor(m_position.x / BLOCK_PXF), x1 = (int) floor((m_position.x + m_velocity.x) / BLOCK_PXF);
			for(int x = x0; x >= x1 && m_velocity.x < 0.0f; --x)
			{
				for(int y = y0; y <= y1 && m_velocity.x < 0.0f; ++y)
				{
					if(terrain->isBlockAt(x, y, WORLD_LAYER_MIDDLE))
					{
						m_position.x = x * BLOCK_PXF + BLOCK_PXF;
						m_velocity.x = 0.0f;
						m_contact |= WEST;
					}
				}
			}
		}
	}

	Vector2I chunkPosition = math::floor(m_position / Vector2F(CHUNK_PXF));
	Chunk *chunk = m_world->getTerrain()->getChunkManager()->getChunkAt(chunkPosition.x, chunkPosition.y, true);
	if(chunk!= m_currentChunk)
	{
		m_currentChunk->m_entities.remove(this);
		chunk->m_entities.push_back(this);
		m_currentChunk = chunk;
	}

	m_packetDelta++;
}

void Entity::setPosition(const Vector2F & pos)
{
	m_lastPosition = m_position = pos;

	Vector2I chunkPosition = math::floor(m_position / Vector2F(CHUNK_PXF));
	Chunk *chunk = m_world->getTerrain()->getChunkManager()->getChunkAt(chunkPosition.x, chunkPosition.y, true);
	if(chunk != m_currentChunk)
	{
		m_currentChunk->m_entities.remove(this);
		chunk->m_entities.push_back(this);
		m_currentChunk = chunk;
	}
}

void Entity::setOriginGUID(RakNet::RakNetGUID originGUID)
{
	m_originGUID = originGUID;
	m_isClientObject = m_connection->getGUID() == m_originGUID;
}

void Entity::packData(RakNet::BitStream *bitStream)
{
	// Write position and velocity
	bitStream->Write(m_position.x);
	bitStream->Write(m_position.y);
	bitStream->Write(m_velocity.x);
	bitStream->Write(m_velocity.y);
}

void Entity::unpackData(RakNet::BitStream *bitStream)
{
	// Read position and velocity
	bitStream->Read(m_position.x);
	bitStream->Read(m_position.y);
	bitStream->Read(m_velocity.x);
	bitStream->Read(m_velocity.y);
	if(!m_initialized)
	{
		m_lastPosition = m_position;
		m_initialized = true;
	}
	m_packetDelta = 0;
}

bool Entity::unpackAndValidate(RakNet::BitStream *bitStream)
{
	const int ticksDelta = m_packetDelta + 1;
	Vector2F position = m_position, velocity = m_velocity;
	unpackData(bitStream);

	// Verify using some time delta between this and previous packet
	float radius = (velocity.length() + 50 /*maxImpulse*/) * ticksDelta;
	float moved = (m_position - position).length();
	if(radius < moved)
	{
		// Invalid movement. Restore object state
		m_position = position;
		m_velocity = velocity;
		return false;
	}
	return true;
}
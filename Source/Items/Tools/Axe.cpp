#include "Axe.h"

#include "Constants.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"
#include "Entities/Player.h"
#include "World/Camera.h"
#include "Entities/ItemDrop.h"
#include "World/Terrain/Terrain.h"
#include "Game/Game.h"
#include "Blocks/BlockData.h"

Axe::Axe(Game *game) :
	m_game(game),
	m_cracksSprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Tools/Pickaxes/MiningCracks.png")),
	m_cracksAnimation(1, 4),
	m_sprite(ResourceManager::get<Texture2D>(":/Sprites/Items/Tools/Axes/IronAxe.png")),
	m_prevBlockPosition(0.0f, 0.0f),
	m_mineCounter(0.0f),
	m_mineTime(0.1f)
{
	m_cracksSprite.setRegion(TextureRegion(0.0f, 0.0f, 0.25f, 1.0f), true);
	m_sprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
}

void Axe::use(Pawn *pawn, const float delta)
{
	// Get block input position
	Vector2i position = m_game->getWorld()->getCamera()->getInputPosition();
	position.x = (int) floor(position.x/BLOCK_PXF);
	position.y = (int) floor(position.y/BLOCK_PXF);
	
	Terrain *terrain = m_game->getWorld()->getTerrain();

	// Is there a block at this position?
	if(terrain->getBlockAt(position.x, position.y, TERRAIN_LAYER_BACK) > BLOCK_ENTITY)
	{
		// Reset timer if block position have changed
		if(position != m_prevBlockPosition)
		{
			m_mineCounter = m_mineTime;
		}

		// Store previous position
		m_prevBlockPosition = position;

		// Count down and remove block when counter is 0
		m_mineCounter -= delta;
		if(m_mineCounter <= 0.0f)
		{
			BlockID blockID = terrain->getBlockAt(position.x, position.y, TERRAIN_LAYER_BACK);
			terrain->removeBlockAt(position.x, position.y, TERRAIN_LAYER_BACK);
			if(blockID == ITEM_OAK_BLOCK)
			{
				makeTreeFall(position.x + 1, position.y);
				makeTreeFall(position.x - 1, position.y);
				makeTreeFall(position.x, position.y + 1);
				makeTreeFall(position.x, position.y - 1);
			}
		}
		else
		{
			m_cracksSprite.setPosition(position * BLOCK_PXF);
			m_cracksSprite.setRegion(m_cracksAnimation.getKeyFrame((int) (4 * (1.0f - m_mineCounter/m_mineTime))));
		}

		m_game->getWorld()->getLocalPlayer()->getHumanoid().setPostAnimation(Humanoid::ANIM_MINE);
	}
	else
	{
		m_game->getWorld()->getLocalPlayer()->getHumanoid().setPostAnimation(Humanoid::ANIM_NULL);
	}
}

void Axe::draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha)
{
	// Is there a block at this position?
	if(m_game->getWorld()->getTerrain()->isBlockAt(m_prevBlockPosition.x, m_prevBlockPosition.y, TERRAIN_LAYER_BACK))
	{
		spriteBatch->drawSprite(m_cracksSprite);
	}
	//player->getHumanoid().drawRightHandSprite(m_sprite, Vector2(16.f, 32.f), spriteBatch);
}

void Axe::makeTreeFall(const int x, const int y)
{
	Terrain *terrain = m_game->getWorld()->getTerrain();

	if(terrain->getBlockAt(x, y, TERRAIN_LAYER_BACK) != BLOCK_WOOD_OAK) return;
	
	list<pair<int, int>> nodes;

	// Find connected blocks
	::set<pair<int, int>> processed;
	::set<pair<int, int>> queue;
	queue.insert(make_pair(x, y));
	while(!queue.empty())
	{
		pair<int, int> node = *queue.begin();
		queue.erase(node);

		if(processed.find(node) != processed.end()) continue;

		if(terrain->getBlockAt(node.first, node.second, TERRAIN_LAYER_BACK) == BLOCK_WOOD_OAK)
		{
			processed.insert(node);
			nodes.push_back(node);
			queue.insert(make_pair(node.first + 1, node.second));
			queue.insert(make_pair(node.first - 1, node.second));
			queue.insert(make_pair(node.first, node.second + 1));
			queue.insert(make_pair(node.first, node.second - 1));
		}
		
		if(terrain->getBlockAt(node.first, node.second, TERRAIN_LAYER_MIDDLE) != BLOCK_EMPTY)
		{
			return;
		}
	}

	// Remove blocks
	for(pair<int, int> n : nodes)
	{
		terrain->removeBlockAt(n.first, n.second, TERRAIN_LAYER_BACK);
	}
}
#include "Axe.h"

#include "Constants.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"
#include "Entities/Player.h"
#include "World/World.h"
#include "Entities/ItemDrop.h"
#include "Game/Game.h"
#include "Blocks/BlockData.h"

Axe::Axe(OverworldGame * game) :
	m_game(game),
	m_cracksSprite(Game::GetInstance()->getResourceManager()->get<Texture2D>("Sprites/Items/Tools/Pickaxes/MiningCracks")),
	m_cracksAnimation(1, 4),
	m_drawCracks(false),
	m_sprite(Game::GetInstance()->getResourceManager()->get<Texture2D>("Sprites/Items/Tools/Axes/IronAxe")),
	m_prevBlockPosition(0, 0),
	m_mineCounter(0.0f),
	m_mineTime(1.0f)
{
	m_cracksSprite.setRegion(TextureRegion(0.0f, 0.0f, 0.25f, 1.0f), true);
	m_sprite.setRegion(TextureRegion(0.0f, 0.0f, 1.0f, 1.0f), true);
}

void Axe::equip(Pawn *player)
{
	player->getHumanoid().setAttachmentTexture(Humanoid::ARM_RIGHT, 1, m_sprite.getTexture());
}

void Axe::unequip(Pawn *player)
{
	player->getHumanoid().setAttachmentTexture(Humanoid::ARM_RIGHT, 1, 0);
}

void Axe::update(Pawn *pawn, const float delta)
{
	// Get block input position
	Vector2I position = m_game->getWorld()->getCamera()->getInputPosition();
	position.x = (int) floor(position.x / BLOCK_PXF);
	position.y = (int) floor(position.y / BLOCK_PXF);

	Terrain *terrain = m_game->getWorld()->getTerrain();
	if(pawn->getController()->getInputState(Controller::INPUT_USE_ITEM) && // Do we have user input and...
		terrain->isBlockAt(position.x, position.y, WORLD_LAYER_BACK)) // ... is there a block at this position?
	{
		// Reset timer if block position have changed
		if(position != m_prevBlockPosition)
		{
			m_mineCounter = m_mineTime;
		}

		// Store previous position
		m_prevBlockPosition = position;

		// Count down and remove block when counter is less than or equal to 0
		m_mineCounter -= delta;
		if(m_mineCounter <= 0.0f)
		{
			BlockID blockID = terrain->getBlockAt(position.x, position.y, WORLD_LAYER_BACK);
			terrain->removeBlockAt(position.x, position.y, WORLD_LAYER_BACK);

			// If the block removed was a wood type block
			if(blockID == BLOCK_OAK_WOOD)
			{
				// Make the rest of the tree fall
				makeTreeFall(position.x + 1, position.y);
				makeTreeFall(position.x - 1, position.y);
				makeTreeFall(position.x, position.y + 1);
				makeTreeFall(position.x, position.y - 1);
			}

			// Don't draw cracks
			m_drawCracks = false;
		}
		else
		{
			// Update cracks sprite
			m_cracksSprite.setPosition(position * BLOCK_PXF);
			m_cracksSprite.setRegion(m_cracksAnimation.getKeyFrame((int) (4 * (1.0f - m_mineCounter / m_mineTime))));
			m_drawCracks = true;
		}

		// Set mining animation
		m_game->getWorld()->getLocalPlayer()->getHumanoid().setPostAnimation(Humanoid::ANIM_MINE);
	}
	else
	{
		m_mineCounter = m_mineTime; // Reset the counter
		m_game->getWorld()->getLocalPlayer()->getHumanoid().setPostAnimation(Humanoid::ANIM_NULL); // Reset animations
		m_drawCracks = false; // Don't draw cracks
	}
}

void Axe::draw(Pawn *player, SpriteBatch *spriteBatch, const float alpha)
{
	// Draw cracks
	if(m_drawCracks)
	{
		spriteBatch->drawSprite(m_cracksSprite);
	}
}

void Axe::makeTreeFall(const int x, const int y)
{
	Terrain *terrain = m_game->getWorld()->getTerrain();
	if(terrain->getBlockAt(x, y, WORLD_LAYER_BACK) != BLOCK_OAK_WOOD) return;

	list<pair<int, int>> nodes;

	// Use bucket fill to find connected blocks
	::set<pair<int, int>> processed;
	::set<pair<int, int>> queue;
	queue.insert(make_pair(x, y));
	while(!queue.empty())
	{
		pair<int, int> node = *queue.begin();
		queue.erase(node);

		if(processed.find(node) != processed.end()) continue;

		if(terrain->getBlockAt(node.first, node.second, WORLD_LAYER_BACK) == BLOCK_OAK_WOOD)
		{
			processed.insert(node);
			nodes.push_back(node);
			queue.insert(make_pair(node.first + 1, node.second));
			queue.insert(make_pair(node.first - 1, node.second));
			queue.insert(make_pair(node.first, node.second + 1));
			queue.insert(make_pair(node.first, node.second - 1));
		}

		// If this cluster is connected to the ground, we don't remove any blocks
		if(terrain->getBlockAt(node.first, node.second, WORLD_LAYER_MIDDLE) != BLOCK_EMPTY)
		{
			return;
		}
	}

	// Remove blocks
	for(pair<int, int> n : nodes)
	{
		terrain->removeBlockAt(n.first, n.second, WORLD_LAYER_BACK);
		makeLeavesFall(n.first, n.second);
	}
}
void Axe::makeLeavesFall(const int x, const int y)
{
	Terrain *terrain = m_game->getWorld()->getTerrain();

	if(terrain->getBlockAt(x, y, WORLD_LAYER_FRONT) != BLOCK_OAK_LEAVES) return;

	list<pair<int, int>> nodes;

	// Use bucket fill to find connected blocks
	::set<pair<int, int>> processed;
	::set<pair<int, int>> queue;
	queue.insert(make_pair(x, y));
	while(!queue.empty())
	{
		pair<int, int> node = *queue.begin();
		queue.erase(node);

		if(processed.find(node) != processed.end()) continue;

		if(terrain->getBlockAt(node.first, node.second, WORLD_LAYER_FRONT) == BLOCK_OAK_LEAVES)
		{
			processed.insert(node);
			nodes.push_back(node);
			queue.insert(make_pair(node.first + 1, node.second));
			queue.insert(make_pair(node.first - 1, node.second));
			queue.insert(make_pair(node.first, node.second + 1));
			queue.insert(make_pair(node.first, node.second - 1));
		}
	}

	// Remove blocks
	for(pair<int, int> n : nodes)
	{
		terrain->removeBlockAt(n.first, n.second, WORLD_LAYER_FRONT);
	}
}
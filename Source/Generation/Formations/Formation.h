#pragma once

#include "Config.h"
#include "Constants.h"
#include "Blocks/Block.h"

class FormationElement;
class ChunkGenerator;

class Formation
{
public:
	Formation(ChunkGenerator *gen, const int width, const int height);
	~Formation();
	
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	
	bool isGeneratedAt(const int formX, const int formY) const;

	void generate(const int formX, const int formY);
	void getBlocks(const int chunkX, const int chunkY, Block *blocks);

protected:
	virtual void generate(const int formX, const int formY, list<FormationElement*> &elements) = 0;
	XORHash m_xorhash;

private:
	const int m_width, m_height;
	set<uint> m_generated;
	unordered_map<uint, list<FormationElement*>> m_chunkToElementListMap;
};

class FormationElement
{
	friend class Formation;
public:
	FormationElement(const int x, const int y) :
		m_originX(x),
		m_originY(y),
		m_x1(x),
		m_y1(y),
		m_x2(x),
		m_y2(y)
	{
	}

	enum ReplacePolicy
	{
		REPLACE_ALWAYS,
		REPLACE_IF_EMPTY
	};

	void setBlockAt(const int x, const int y, const WorldLayer layer, const BlockID id, const ReplacePolicy replace = REPLACE_ALWAYS);
	void setBlockEntityAt(const int x, const int y, const WorldLayer layer, BlockEntity *blockEntityID);

private:
	struct Block
	{
		Block() :
			blockID(0),
			blockEntity(0),
			replace(REPLACE_IF_EMPTY)
		{
		}

		BlockID blockID;
		BlockEntity *blockEntity;
		ReplacePolicy replace;
	};

	const int m_originX, m_originY;
	int m_x1, m_y1, m_x2, m_y2;
	unordered_map<uint32_t, Block[WORLD_LAYER_COUNT]> m_blocks;
};
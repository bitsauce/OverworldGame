#pragma once

#include "Config.h"
#include "Constants.h"
#include "Blocks/Block.h"

class FormationElement;

class Formation
{
public:
	Formation(const int width, const int height);
	~Formation();
	
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	
	bool isGeneratedAt(const int formX, const int formY) const;

	void generate(const int formX, const int formY);
	void getBlocks(const int chunkX, const int chunkY, Block *blocks);

protected:
	virtual void generate(const int formX, const int formY, list<FormationElement*> &elements) = 0;

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

	void setBlockAt(const int x, const int y, const WorldLayer layer, const BlockID id)
	{
		// Expand bounding box
		if(x < 0) m_x1 = min(m_x1, m_originX + x);
		else m_x2 = max(m_x2, m_originX + x);
		if(y < 0) m_y1 = min(m_y1, m_originY + y);
		else m_y2 = max(m_y2, m_originY + y);

		// Set block at position
		m_blocks[CHUNK_KEY(m_originX + x, m_originY + y)][layer].setBlockDataByID(id);
	}

	void setBlockEntityAt(const int x, const int y, const WorldLayer layer, const BlockEntityData *data);

private:
	const int m_originX, m_originY;
	int m_x1, m_y1, m_x2, m_y2;
	unordered_map<uint, Block[WORLD_LAYER_COUNT]> m_blocks;
};
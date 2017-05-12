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
	void getChunkBlocks(const int chunkX, const int chunkY, ChunkBlock *blocks);

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
	FormationElement(int x, int y, int w, int h) :
		m_x(x),
		m_y(y),
		m_w(w),
		m_h(h)
	{
		m_blocks = new BlockID[m_w * m_h];
	}

//private:
	const int m_x, m_y, m_w, m_h;
	BlockID *m_blocks;
};
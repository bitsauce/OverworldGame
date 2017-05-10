#pragma once

#include "Config.h"
#include "Constants.h"
#include "Blocks/Block.h"

class Formation
{
public:
	Formation(const int width, const int height);
	
	int getWidth() const { return m_width; }
	int getHeight() const { return m_height; }
	
	bool existsAt(const int formX, const int formY) const;
	bool isGeneratedAt(const int formX, const int formY) const;

	void generateFormationBlocks(const int formX, const int formY);
	void getChunkBlocks(const int formX, const int formY, const int chunkOffsetX, const int chunkOffsetY, ChunkBlock *blocks);

protected:
	virtual void generate(const int formX, const int formY, BlockID* blocks) = 0;

private:
	const int m_width, m_height;
	unordered_map<uint, BlockID*> m_blocks;
};
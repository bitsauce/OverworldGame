#ifndef TORCH_H
#define TORCH_H

#include "Config.h"
#include "BlockEntity.h"
#include "BlockEntityData.h"
#include "Lighting/Spotlight.h"

class Torch : public BlockEntity
{
	friend class BlockEntityData;
public:
	Torch(const int x, const int y);

	void draw(SpriteBatch *spriteBatch);

private:
	Sprite m_sprite;
	Spotlight m_spotlight;
	//float durration;?

	static void Factory(const int x, const int y) { new Torch(x, y); }
};

#endif // TORCH_H
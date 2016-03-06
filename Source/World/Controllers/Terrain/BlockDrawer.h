#ifndef BLOCK_DRAWER_H
#define BLOCK_DRAWER_H

#include "Entities/Entity.h"

enum WorldLayer;
class ChunkManager;
class Camera;

class BlockDrawer : public SceneObject
{
public:
	BlockDrawer(World *world, const WorldLayer layer);

	void onDraw(DrawEvent *e);

private:
	ChunkManager *m_chunkManager;
	Camera *m_camera;
	WorldLayer m_layer;
};

#endif // BLOCK_DRAWER_H
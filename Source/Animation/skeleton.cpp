#include "skeleton.h"
#include "animation.h"
#include "slot.h"
#include "bone.h"
#include <spine/extension.h>

#ifndef SPINE_MESH_VERTEX_COUNT_MAX
#define SPINE_MESH_VERTEX_COUNT_MAX 1000
#endif

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
{
	Texture2DPtr *texture = new Texture2DPtr(ResourceManager::get<Texture2D>(path));
	if(texture)
	{
		self->rendererObject = texture;
		self->width = (*texture)->getWidth();
		self->height = (*texture)->getHeight();
	}
}

void _spAtlasPage_disposeTexture(spAtlasPage* self)
{
	if(self->rendererObject) {
		delete ((Texture2DPtr*)self->rendererObject);
	}
}

char* _spUtil_readFile(const char* path, int* length)
{
	string content;
	char *data = 0;
	if(FileSystem::ReadFile(path, content))
	{
		*length = content.size();
		data = MALLOC(char, *length);
		memcpy(data, content.c_str(), *length);
	}
	return data;
}

Skeleton::Skeleton(const string &jsonFile, const string &atlasFile, const float scale) :
	m_data(nullptr),
	m_atlas(nullptr)
{
	m_atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
	spSkeletonJson* json = spSkeletonJson_create(m_atlas);
	json->scale = scale;

	m_data = spSkeletonJson_readSkeletonDataFile(json, jsonFile.c_str());
	if(!m_data)
	{
		LOG("Skeleton::Skeleton(): JSON Error '%s'", json->error);
		return;
	}

	LOG("Skeleton::Skeleton(): Default skin name '%s'", m_data->defaultSkin->name);
	spSkeletonJson_dispose(json);

	m_worldVertices = MALLOC(float, SPINE_MESH_VERTEX_COUNT_MAX);
	m_self = spSkeleton_create(m_data);

	// Load animations
	for(int i = 0; i < m_data->animationCount; i++)
	{
		spAnimation *anim = m_data->animations[i];
		m_animations[anim->name] = new Animation(m_self, anim);
	}

	// Load slots
	for(int i = 0; i < m_self->slotCount; i++)
	{
		spSlot *slot = m_self->slots[i];
		m_slots[slot->data->name] = new Slot(slot);
	}

	// Load bone
	for(int i = 0; i < m_self->boneCount; i++)
	{
		spBone *bone = m_self->bones[i];
		m_bones[bone->data->name] = new Bone(bone);
	}
}

Skeleton::~Skeleton()
{
	for(map<string, Animation*>::iterator itr = m_animations.begin(); itr != m_animations.end(); ++itr) {
		delete itr->second;
	}
	for(map<string, Slot*>::iterator itr = m_slots.begin(); itr != m_slots.end(); ++itr) {
		delete itr->second;
	}
	for(map<string, Bone*>::iterator itr = m_bones.begin(); itr != m_bones.end(); ++itr) {
		delete itr->second;
	}
	spAtlas_dispose(m_atlas);
	spSkeleton_dispose(m_self);
	spSkeletonData_dispose(m_data);
}

Animation *Skeleton::findAnimation(const string &name)
{
	return m_animations.find(name) != m_animations.end() ? m_animations[name] : nullptr;
}

Slot *Skeleton::findSlot(const string &name)
{
	return m_slots.find(name) != m_slots.end() ? m_slots[name] : nullptr;
}

Bone *Skeleton::findBone(const string &name)
{
	return m_bones.find(name) != m_bones.end() ? m_bones[name] : nullptr;
}

void Skeleton::setPosition(const Vector2 &pos)
{
	m_self->x = pos.x;
	m_self->y = pos.y;
}

Vector2 Skeleton::getPosition() const
{
	return Vector2(m_self->x, m_self->y);
}

void Skeleton::setFlipX(const bool flip)
{
	m_self->flipX = flip;
}

bool Skeleton::getFlipX() const
{
	return m_self->flipX != 0;
}

void Skeleton::setFlipY(const bool flip)
{
	m_self->flipY = flip;
}

bool Skeleton::getFlipY() const
{
	return m_self->flipY != 0;
}

Texture2DPtr Skeleton::getTexture() const
{
	return *(Texture2DPtr*)m_atlas->pages->rendererObject;
}

TextureRegion Skeleton::getTextureRegion(const string &name) const
{
	spAtlasRegion *region = m_atlas->regions;
	while(region)
	{
		if(string(region->name) == name)
		{
			return TextureRegion(region->u, 1.f - region->v2, region->u2, 1.f - region->v);
		}
		region = region->next;
	}
	return TextureRegion();
}

void Skeleton::draw(GraphicsContext &context)
{
	// Update world transform
	spSkeleton_updateWorldTransform(m_self);

	// Draw vertices
	Vertex *vertices = new Vertex[4 * m_self->slotCount];
	uint *indices = new uint[6 * m_self->slotCount];
	Texture2DPtr texture = 0;
	for(int i = 0; i < m_self->slotCount; i++)
	{
		spSlot *slot = m_self->drawOrder[i];
		spAttachment *attachment = slot->attachment;
		if(!attachment)
			continue;
		if(attachment->type == SP_ATTACHMENT_REGION)
		{
			spRegionAttachment* regionAttachment = SUB_CAST(spRegionAttachment, attachment);
			texture = *(Texture2DPtr*)((spAtlasRegion*)regionAttachment->rendererObject)->page->rendererObject;
			spRegionAttachment_computeWorldVertices(regionAttachment, slot->skeleton->x, slot->skeleton->y, slot->bone, m_worldVertices);

			uchar r = uchar(m_self->r * slot->r * 255);
			uchar g = uchar(m_self->g * slot->g * 255);
			uchar b = uchar(m_self->b * slot->b * 255);
			uchar a = uchar(m_self->a * slot->a * 255);

			vertices[i * 4].set4ub(VERTEX_COLOR, r, g, b, a);
			vertices[i * 4].set4f(VERTEX_POSITION, m_worldVertices[SP_VERTEX_X1], m_worldVertices[SP_VERTEX_Y1]);
			vertices[i * 4].set4f(VERTEX_TEX_COORD, regionAttachment->uvs[SP_VERTEX_X1], 1.0f - regionAttachment->uvs[SP_VERTEX_Y1]);

			vertices[i * 4 + 1].set4ub(VERTEX_COLOR, r, g, b, a);
			vertices[i * 4 + 1].set4f(VERTEX_POSITION, m_worldVertices[SP_VERTEX_X2], m_worldVertices[SP_VERTEX_Y2]);
			vertices[i * 4 + 1].set4f(VERTEX_TEX_COORD, regionAttachment->uvs[SP_VERTEX_X2], 1.0f - regionAttachment->uvs[SP_VERTEX_Y2]);

			vertices[i * 4 + 2].set4ub(VERTEX_COLOR, r, g, b, a);
			vertices[i * 4 + 2].set4f(VERTEX_POSITION, m_worldVertices[SP_VERTEX_X4], m_worldVertices[SP_VERTEX_Y4]);
			vertices[i * 4 + 2].set4f(VERTEX_TEX_COORD, regionAttachment->uvs[SP_VERTEX_X4], 1.0f - regionAttachment->uvs[SP_VERTEX_Y4]);
			
			vertices[i * 4 + 3].set4ub(VERTEX_COLOR, r, g, b, a);
			vertices[i * 4 + 3].set4f(VERTEX_POSITION, m_worldVertices[SP_VERTEX_X3], m_worldVertices[SP_VERTEX_Y3]);
			vertices[i * 4 + 3].set4f(VERTEX_TEX_COORD, regionAttachment->uvs[SP_VERTEX_X3], 1.0f - regionAttachment->uvs[SP_VERTEX_Y3]);

			indices[i * 6 + 0] = i * 4 + 0;
			indices[i * 6 + 1] = i * 4 + 1;
			indices[i * 6 + 2] = i * 4 + 2;
			indices[i * 6 + 3] = i * 4 + 2;
			indices[i * 6 + 4] = i * 4 + 1;
			indices[i * 6 + 5] = i * 4 + 3;
		}
		/*else if (attachment->type == ATTACHMENT_MESH)
		{
			MeshAttachment* mesh = (MeshAttachment*)attachment;
			if (mesh->verticesCount > SPINE_MESH_VERTEX_COUNT_MAX) continue;
			texture = (Texture*)((AtlasRegion*)mesh->rendererObject)->page->rendererObject;
			MeshAttachment_computeWorldVertices(mesh, slot->skeleton->x, slot->skeleton->y, slot, worldVertices);

			Uint8 r = skeleton->r * slot->r * 255;
			Uint8 g = skeleton->g * slot->g * 255;
			Uint8 b = skeleton->b * slot->b * 255;
			Uint8 a = skeleton->a * slot->a * 255;
			vertex.color.r = r;
			vertex.color.g = g;
			vertex.color.b = b;
			vertex.color.a = a;

			Vector2u size = texture->getSize();
			for (int i = 0; i < mesh->trianglesCount; ++i) {
				int index = mesh->triangles[i] << 1;
				vertex.position.x = worldVertices[index];
				vertex.position.y = worldVertices[index + 1];
				vertex.texCoords.x = mesh->uvs[index] * size.x;
				vertex.texCoords.y = mesh->uvs[index + 1] * size.y;
				vertexArray->append(vertex);
			}

		} else if (attachment->type == ATTACHMENT_SKINNED_MESH) {
			SkinnedMeshAttachment* mesh = (SkinnedMeshAttachment*)attachment;
			if (mesh->uvsCount > SPINE_MESH_VERTEX_COUNT_MAX) continue;
			texture = (Texture*)((AtlasRegion*)mesh->rendererObject)->page->rendererObject;
			SkinnedMeshAttachment_computeWorldVertices(mesh, slot->skeleton->x, slot->skeleton->y, slot, worldVertices);

			Uint8 r = skeleton->r * slot->r * 255;
			Uint8 g = skeleton->g * slot->g * 255;
			Uint8 b = skeleton->b * slot->b * 255;
			Uint8 a = skeleton->a * slot->a * 255;
			vertex.color.r = r;
			vertex.color.g = g;
			vertex.color.b = b;
			vertex.color.a = a;

			Vector2u size = texture->getSize();
			for (int i = 0; i < mesh->trianglesCount; ++i) {
				int index = mesh->triangles[i] << 1;
				vertex.position.x = worldVertices[index];
				vertex.position.y = worldVertices[index + 1];
				vertex.texCoords.x = mesh->uvs[index] * size.x;
				vertex.texCoords.y = mesh->uvs[index + 1] * size.y;
				vertexArray->append(vertex);
			}
		}*/
	}

	context.setTexture(texture);
	context.drawIndexedPrimitives(GraphicsContext::PRIMITIVE_TRIANGLES, vertices, 4 * m_self->slotCount, indices, 6 * m_self->slotCount);
	context.setTexture(0);

	delete[] vertices;
}
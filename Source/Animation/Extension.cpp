#include "Config.h"
#include <spine/spine.h>
#include <spine/extension.h>

void _spAtlasPage_createTexture(spAtlasPage* self, const char* path)
{
	shared_ptr<Texture2D> *texture = new shared_ptr<Texture2D>(new Texture2D(Pixmap(path)));
	if(texture)
	{
		self->rendererObject = texture;
		self->width = (*texture)->getWidth();
		self->height = (*texture)->getHeight();
	}
}

void _spAtlasPage_disposeTexture(spAtlasPage* self)
{
	if(self->rendererObject)
	{
		delete ((shared_ptr<Texture2D>*)self->rendererObject);
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
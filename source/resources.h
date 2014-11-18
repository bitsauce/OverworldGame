#ifndef RESOURCES_H
#define RESOURCES_H

#include <x2d/x2d.h>

class ResourceManager
{
public:
	static void load();
	static void release();

	static XFont *getFont(const string &name);
	static XTexture &getTexture(const string &name);
	static XAudioSource &getSound(const string &name);

private:

	struct Resource
	{
		enum Type
		{
			FONT,
			TEXTURE,
			SOUND
		};

		Type type;
		void *object;
	};

	static map<string, Resource> s_resources;
};

#endif // RESOURCES_H
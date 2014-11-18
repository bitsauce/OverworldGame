#include "resources.h"

map<string, ResourceManager::Resource> ResourceManager::s_resources;

void ResourceManager::load()
{
	s_resources["large_font"].object = new XFont("Arial Bold", 30);
}

void ResourceManager::release()
{
	for(map<string, Resource>::iterator itr = s_resources.begin(); itr != s_resources.end(); ++itr)
	{
		delete itr->second.object;
	}
}

XFont *ResourceManager::getFont(const string &name)
{
	if(s_resources[name].type == Resource::FONT) return 0;
	return (XFont*)s_resources[name].object;
}
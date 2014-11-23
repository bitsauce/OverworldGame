#include "debug.h"
#include "constants.h"

map<string, string> Debug::s_variables;

void Debug::setVariable(const string &name, const string &value)
{
	s_variables[name] = value;
}

void Debug::draw(XBatch *batch)
{
	shared_ptr<XFont> font = xd::ResourceManager::get<XFont>(UI_MENU_BUTTON_FONT);
	font->setColor(XColor(0, 0, 0, 255));

	string drawString;
	for(map<string, string>::iterator itr = s_variables.begin(); itr != s_variables.end(); ++itr)
	{
		drawString += itr->first + ": " + itr->second + "\n";
	}

	font->draw(batch, Vector2(5.0f, 48.0f), drawString);
}
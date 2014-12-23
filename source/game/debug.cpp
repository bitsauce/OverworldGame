#include "debug.h"
#include "constants.h"

map<string, string> Debug::s_variables;

void Debug::setVariable(const string &name, const string &value)
{
	s_variables[name] = value;
}

void Debug::draw(xd::SpriteBatch *SpriteBatch)
{
	xd::FontPtr font = xd::ResourceManager::get<xd::Font>(UI_MENU_BUTTON_FONT);
	font->setColor(xd::Color(0, 0, 0, 255));

	string drawString;
	for(map<string, string>::iterator itr = s_variables.begin(); itr != s_variables.end(); ++itr)
	{
		drawString += itr->first + ": " + itr->second + "\n";
	}

	font->draw(SpriteBatch, Vector2(5.0f, 48.0f), drawString);
}
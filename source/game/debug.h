#ifndef DEBUG_H
#define DEBUG_H

#include <x2d/x2d.h>

class Debug
{
public:
	static void setVariable(const string &name, const string &value);
	static void draw(xd::SpriteBatch *SpriteBatch);

private:
	static map<string, string> s_variables;
};

#endif // DEBUG_H
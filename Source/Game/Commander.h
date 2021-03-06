#pragma once

#include "Config.h"

class Overworld;
class Chat;

class Commander
{
public:
	struct Command
	{
		Command(const string &name, const string &validArgCount, const string &helpString, function<void(Chat*, vector<string>)> function) :
			name(name),
			helpString(helpString),
			function(function)
		{
			vector<string> argCount = util::splitString(validArgCount, "|");
			for(uint i = 0; i < 10; ++i)
			{
				if(i < argCount.size())
				{
					this->validArgCount[i] = (char) util::strToInt(argCount[i]);
				}
				else
				{
					this->validArgCount[i] = -1;
				}
			}
		}

		const string name;
		char validArgCount[10];
		const string helpString;
		const function<void(Chat*, vector<string>)> function;
	};

	Commander(Overworld *game);
	~Commander();

	void execute(string command);

private:
	void spawn(Chat*, vector<string>);
	void give(Chat*, vector<string>);
	void place(Chat*, vector<string>);
	void setGravity(Chat*, vector<string>);
	void testclouds(Chat*, vector<string>);
	void setres(Chat*, vector<string>);
	void move(Chat*, vector<string>);

	void setAppearance(Chat*, vector<string>);
	void setApparel(Chat*, vector<string>);
	void setAppearanceColor(Chat*, vector<string>, const string &which);

	void setTime(Chat*, vector<string>);

	Overworld *m_game;

	list<Command> m_commands;
};


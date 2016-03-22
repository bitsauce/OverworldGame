#pragma once

#include "Config.h"

class OverworldGame;
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

	Commander(OverworldGame *game);
	~Commander();

	void execute(const string &command);

private:
	void spawn(Chat*, vector<string>);
	void give(Chat*, vector<string>);
	void place(Chat*, vector<string>);
	void setGravity(Chat*, vector<string>);
	void testclouds(Chat*, vector<string>);
	void setres(Chat*, vector<string>);

	OverworldGame *m_game;

	list<Command> m_commands;
};


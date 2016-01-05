#include "Commander.h"
#include "Game.h"

#include "Entities/Player.h"
#include "Entities/EntityData.h"

#include "Items/ItemData.h"

#include "Gui/GameOverlay/GameOverlay.h"

Commander::Commander(OverworldGame *game) :
	m_game(game)
{
	m_commands.push_back(Command("spawn", "1", "<EntityName> [x] [y] [dataTag]", bind(&Commander::spawn, this, placeholders::_1, placeholders::_2)));
	//m_commands.push_back(Command("place", "1", "<BlockEntityName> [x] [y] [dataTag]", bind(&Commander::place, this, placeholders::_1, placeholders::_2)));
	//m_commands.push_back(Command("setblock", "3", "<BlockName> <x> <y>", bind(&Commander::setBlock, this, placeholders::_1, placeholders::_2)));
	m_commands.push_back(Command("give", "1|2|3", "<ItemName> [amount] [player]", bind(&Commander::give, this, placeholders::_1, placeholders::_2)));
}

Commander::~Commander()
{
}

void Commander::execute(const string &cmdString)
{/*
	// Get chat
	Chat *chat = m_game->getGameOverlay()->getChat();

	// Split the command
	vector<string> cmdSplitString = util::splitString(cmdString, " ");
	if(cmdSplitString[0].empty()) // Empty command
	{
		chat->insertMessage("Type /help for more info.");
		return;
	}

	// Find the command
	for(Command cmd : m_commands)
	{
		if(cmd.name == cmdSplitString[0])
		{
			for(int i = 0; i < 10; ++i)
			{
				if(cmd.validArgCount[i] == -1) break;
				if(cmd.validArgCount[i] == (char) cmdSplitString.size() - 1)
				{
					// Call the command with parameters
					cmd.function(chat, vector<string>(cmdSplitString.begin() + 1, cmdSplitString.end()));
					return;
				}
			}

			chat->insertMessage("Incorrect number of arguments");
			chat->insertMessage("Usage: " + cmd.name + " " + cmd.helpString);

			return;
		}
	}

	// Command not recognized
	chat->insertMessage(cmdSplitString[0] + " is not a recognized command.");*/
}

void Commander::spawn(Chat *chat, vector<string> args)
{
	/*EntityData *data = EntityData::getByName(args[0]);
	if(data == 0)
	{
		chat->insertMessage("No entity named '" + args[0] + "'");
		return;
	}

	DynamicEntity *entity = (DynamicEntity*) data->create(m_game->getWorld());
	//entity->setPosition(m_game->getWorld()->getLocalPlayer()->getPosition() + Vector2(0.0f, 10.0f));

	chat->insertMessage(args[0] + " spawned");*/
}

void Commander::give(Chat *chat, vector<string> args)
{
	ItemData *data = ItemData::getByName(args[0]);
	/*if(data == 0)
	{
		chat->insertMessage("No item named '" + args[0] + "'");
		return;
	}

	if(args.size() == 1)
	{
		//m_game->getWorld()->getLocalPlayer()->getStorage()->addItem(data->getID());
		chat->insertMessage(args[0] + " given");
	}
	else
	{
		int amt = util::strToInt(args[1]);
		//m_game->getWorld()->getLocalPlayer()->getStorage()->addItem(data->getID(), amt);
		chat->insertMessage(args[1] + " " + args[0] + " given");
	}*/
}
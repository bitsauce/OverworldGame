#include "Commander.h"
#include "Game.h"

#include "Entities/Player.h"
#include "Entities/EntityData.h"
#include "Items/ItemData.h"
#include "Gui/GameOverlay/GameOverlay.h"
#include "Game/States/InGameState.h"
#include "Networking/Client.h"

Commander::Commander(OverworldGame *game) :
	m_game(game)
{
	m_commands.push_back(Command("spawn", "1", "<EntityName> [x] [y] [dataTag]", bind(&Commander::spawn, this, placeholders::_1, placeholders::_2)));
	m_commands.push_back(Command("place", "1|3", "<BlockEntityName> [x] [y] [dataTag]", bind(&Commander::place, this, placeholders::_1, placeholders::_2)));
	//m_commands.push_back(Command("setblock", "3", "<BlockName> <x> <y>", bind(&Commander::setBlock, this, placeholders::_1, placeholders::_2)));
	m_commands.push_back(Command("give", "1|2|3", "<ItemName> [amount] [player]", bind(&Commander::give, this, placeholders::_1, placeholders::_2)));
	m_commands.push_back(Command("testphysics", "6", "<gravity> <jumpforce> <jumpease> <movespeed> <maxspeed> <friction>", bind(&Commander::setGravity, this, placeholders::_1, placeholders::_2)));
	m_commands.push_back(Command("testclouds", "2", "<height> <offset>", bind(&Commander::testclouds, this, placeholders::_1, placeholders::_2)));
	m_commands.push_back(Command("setres", "2", "<w> <h>", bind(&Commander::setres, this, placeholders::_1, placeholders::_2)));
	m_commands.push_back(Command("connect", "1|2", "<ip> [port]", bind(&Commander::connect, this, placeholders::_1, placeholders::_2)));
	m_commands.push_back(Command("move", "2", "<x> <y>", bind(&Commander::move, this, placeholders::_1, placeholders::_2)));
	m_commands.push_back(Command("setAppearance", "0|2", "<slot> <name>", bind(&Commander::setAppearance, this, placeholders::_1, placeholders::_2)));
	m_commands.push_back(Command("setSkinColor", "3", "<R> <G> <B>", bind(&Commander::setAppearanceColor, this, placeholders::_1, placeholders::_2, "Skin")));
	m_commands.push_back(Command("setHairColor", "3", "<R> <G> <B>", bind(&Commander::setAppearanceColor, this, placeholders::_1, placeholders::_2, "Hair")));
}

Commander::~Commander()
{
}

void Commander::execute(const string &cmdString)
{
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
	chat->insertMessage(cmdSplitString[0] + " is not a recognized command.");
}

void Commander::spawn(Chat *chat, vector<string> args)
{
	EntityData *data = EntityData::getByName(args[0]);
	if(!data)
	{
		chat->insertMessage("No entity named '" + args[0] + "'");
		return;
	}

	DynamicEntity *entity = (DynamicEntity*) data->create(m_game->getWorld());
	entity->setPosition(m_game->getWorld()->getLocalPlayer()->getPosition());

	chat->insertMessage(args[0] + " spawned");
}

void Commander::give(Chat *chat, vector<string> args)
{
	ItemData *data = ItemData::getByName(args[0]);
	if(data == 0)
	{
		chat->insertMessage("No item named '" + args[0] + "'");
		return;
	}

	if(args.size() == 1)
	{
		m_game->getWorld()->getLocalPlayer()->getStorage()->addItem(data->getID());
		chat->insertMessage(args[0] + " given");
	}
	else
	{
		int amt = util::strToInt(args[1]);
		m_game->getWorld()->getLocalPlayer()->getStorage()->addItem(data->getID(), amt);
		chat->insertMessage(args[1] + " " + args[0] + " given");
	}
}

void Commander::place(Chat *, vector<string> args)
{
	if(args.size() == 1)
	{
		BlockEntityData *data = BlockEntityData::getByName(args[0]);
		if(data)
		{
			Vector2F blockPos = math::floor(m_game->getWorld()->getLocalPlayer()->getCenter() / BLOCK_PXF);
			m_game->getWorld()->getTerrain()->createBlockEntityAt(blockPos.x, blockPos.y, data->getID(), true);
		}
	}
	else if(args.size() == 3)
	{
		BlockEntityData *data = BlockEntityData::getByName(args[0]);
		if(data)
		{
			m_game->getWorld()->getTerrain()->createBlockEntityAt(util::strToInt(args[1]), util::strToInt(args[2]), data->getID(), true);
		}
	}
}

void Commander::setGravity(Chat *, vector<string> args)
{
	// <gravity> <jumpforce> <jumpease> <movespeed> <maxspeed> <friction>
	Pawn *player = m_game->getWorld()->getLocalPlayer();
	player->setGravityScale(util::strToFloat(args[0]));
	player->m_jumpForce = util::strToFloat(args[1]);
	player->m_jumpEase = util::strToFloat(args[2]);
	player->m_moveSpeed = util::strToFloat(args[3]);
	player->m_maxSpeed = util::strToFloat(args[4]);
	player->m_friction = util::strToFloat(args[5]);
}

void Commander::testclouds(Chat*, vector<string> args)
{
	Background *background = m_game->getWorld()->getBackground();
	background->m_cloudHeight = util::strToFloat(args[0]);
	background->m_cloudOffset = util::strToFloat(args[1]);
}

void Commander::setres(Chat *, vector<string> args)
{
	m_game->getWindow()->setSize(util::strToFloat(args[0]), util::strToFloat(args[1]));
}

void Commander::connect(Chat *, vector<string> args)
{
	ushort port = 45556;
	if(args.size() == 2)
	{
		port = util::strToInt(args[1]);
	}

	// Create client object
	Client *client = new Client(m_game, args[0], port);

	// Create game state
	InGameState *state = new InGameState(m_game, client);

	// Push game state
	m_game->popState();
	m_game->pushState(state);
}

void Commander::move(Chat *, vector<string> args)
{
	if(args.size() == 2)
	{
		m_game->getWorld()->getCamera()->lookAt(Vector2F(util::strToFloat(args[0]), util::strToFloat(args[1])));
	}
}

Humanoid::HumanoidSlot getSlotByName(const string &name)
{
	if(name == "Hips") return Humanoid::HIPS;
	if(name == "Left_Thigh") return Humanoid::LEFT_THIGH;
	if(name == "Right_Thigh") return Humanoid::RIGHT_THIGH;
	if(name == "Left_Leg") return Humanoid::LEFT_LEG;
	if(name == "Right_Leg") return Humanoid::RIGHT_LEG;
	if(name == "Torso") return Humanoid::TORSO;
	if(name == "Eyes") return Humanoid::EYES;
	if(name == "Hair") return Humanoid::HAIR;
	if(name == "Head") return Humanoid::HEAD;
	if(name == "Left_Shoulder") return Humanoid::LEFT_SHOULDER;
	if(name == "Right_Shoulder") return Humanoid::RIGHT_SHOULDER;
	if(name == "Left_Arm") return Humanoid::LEFT_ARM;
	if(name == "Right_Arm") return Humanoid::RIGHT_ARM;
	if(name == "Left_Hand") return Humanoid::LEFT_HAND;
	if(name == "Right_Hand") return Humanoid::RIGHT_HAND;
	return Humanoid::SLOT_COUNT;
}

void Commander::setAppearance(Chat *chat, vector<string> args)
{
	if(args.size() == 0)
	{
		// TODO: Randomize appearance
	}
	else if(args.size() == 2)
	{
		Player *player = m_game->getWorld()->getLocalPlayer();
		if(player)
		{
			Humanoid::HumanoidSlot slot = getSlotByName(args[0]);
			if(slot != Humanoid::SLOT_COUNT)
			{
				if(!player->getHumanoid().setAppearance(slot, args[1]))
				{
					chat->insertMessage("Invalid appearance name '" + args[1] + "'");
				}
			}
			else
			{
				chat->insertMessage("Invalid slot '" + args[0] + "'");
			}
		}
	}
}

void Commander::setAppearanceColor(Chat *, vector<string> args, const string &which)
{
	if(args.size() == 3)
	{
		Player *player = m_game->getWorld()->getLocalPlayer();
		if(player)
		{
			Color color(util::strToInt(args[0]), util::strToInt(args[1]), util::strToInt(args[2]), 255);
			if(which == "Skin")
			{
				player->getHumanoid().setSkinColor(color);
			}
			else if(which == "Hair")
			{
				player->getHumanoid().setHairColor(color);
			}
		}
	}
}

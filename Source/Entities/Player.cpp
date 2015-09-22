#include "BitStream.h"

#include "Player.h"
#include "World/World.h"
#include "World/Camera.h"
#include "World/Terrain/Terrain.h"
#include "Constants.h"
#include "Entities/Physics/DynamicEntity.h"
#include "Items/ItemData.h"
#include "Networking/Connection.h"
#include "Networking/Server.h"
#include "Animation/Animation.h"
#include "Animation/Skeleton.h"
#include "Animation/Bone.h"
#include "Gui/GameOverlay.h"
#include "Things/Thing.h"
#include "Game/Game.h"
#include "Game/GameStates/GameState.h"
#include "Game/Scene.h"

#include "Gui/Canvas.h"

Player::Player(Game *game) :
	Pawn(game, ENTITY_PLAYER)
{
}

Player::~Player()
{
}
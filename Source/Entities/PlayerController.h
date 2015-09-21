#pragma once

#include "Config.h"
#include "Controller.h"
#include "Networking/NetworkObject.h"
#include "Entities/Physics/DynamicEntity.h"

class PlayerController : public Controller
{
public:
	PlayerController(RakNet::RakNetGUID guid);

private:
	//RakNet::RakNetGUID m_guid;
};
#pragma once

#include "Config.h"
#include "Controller.h"
#include "Networking/NetworkObject.h"
#include "Entities/Dynamic/DynamicEntity.h"

class PlayerController : public Controller
{
public:
	PlayerController(const bool local);

private:
	//RakNet::RakNetGUID m_guid;
};
#include "DeleteUnitMessage.h"
#include "Game.h"
#include "GameMessageManager.h"
#include "UnitManager.h"
#include "SpriteManager.h"

DeleteUnitMessage::DeleteUnitMessage()
	: GameMessage(DELETE_UNIT_MESSAGE)
{
}

DeleteUnitMessage::~DeleteUnitMessage()
{
}

void DeleteUnitMessage::process()
{
	gpGame->getUnitManager()->deleteRandomUnit();
}

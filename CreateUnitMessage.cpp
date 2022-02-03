#include "Game.h"
#include "GameMessageManager.h"
#include "CreateUnitMessage.h"
#include "UnitManager.h"
#include "SpriteManager.h"

CreateUnitMessage::CreateUnitMessage()
	:GameMessage(CREATE_UNIT_MESSAGE)
{
}

CreateUnitMessage::~CreateUnitMessage()
{
}

void CreateUnitMessage::process()
{
	Unit* pUnit = gpGame->getUnitManager()->createSphereUnit(*gpGame->getSpriteManager()->getSprite(TARGET_SPRITE_ID));
}


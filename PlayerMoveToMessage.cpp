#include "Game.h"
#include "GameMessageManager.h"
#include "PlayerMoveToMessage.h"
#include "CreateUnitMessage.h"
#include "UnitManager.h"

PlayerMoveToMessage::PlayerMoveToMessage( const Vector2D& pos )
:GameMessage(PLAYER_MOVETO_MESSAGE)
,mPos(pos)
{
}

PlayerMoveToMessage::~PlayerMoveToMessage()
{
}

void PlayerMoveToMessage::process()
{
	Unit* pPlayer = gpGame->getUnitManager()->getPlayerUnit();
	if (pPlayer)
	{
		//Player will arrive and face the clicked position
		pPlayer->setSteering(Steering::BLEND, mPos);
	}
}


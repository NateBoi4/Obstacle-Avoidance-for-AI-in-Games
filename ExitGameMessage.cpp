#include "ExitGameMessage.h"
#include "GameMessageManager.h"
#include "Game.h"

ExitGameMessage::ExitGameMessage()
	: GameMessage(EXIT_GAME_MESSAGE)
{
}

ExitGameMessage::~ExitGameMessage()
{
}

void ExitGameMessage::process()
{
	gpGame->setRunning(true);
}

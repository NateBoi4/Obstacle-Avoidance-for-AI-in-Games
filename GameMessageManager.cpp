#include <cassert>
#include "Game.h"
#include "GameMessageManager.h"
#include "GameMessage.h"

using namespace std;

GameMessageManager::GameMessageManager(Uint32 size)
{
	mMessageBuffers[0] = new CircularQueue<GameMessage*>(size);
	mMessageBuffers[1] = new CircularQueue<GameMessage*>(size);
}

GameMessageManager::~GameMessageManager()
{
	GameMessage* pMessage;
	for (Uint32 i = 0; i < 2; i++)
	{
		while (mMessageBuffers[i]->popFront(pMessage))
		{
			delete pMessage;
		}

		delete mMessageBuffers[i];
	}
}

void GameMessageManager::processMessagesForThisframe()
{
	double currentTime = gpGame->getCurrentTime();
	GameMessage* pMessage;

	//switch buffers
	if (mCurrentBufferNum == 0)
	{
		mCurrentBufferNum = 1;
		mPendingBufferNum = 0;
	}
	else
	{
		mCurrentBufferNum = 0;
		mPendingBufferNum = 1;
	}

	while(mMessageBuffers[mCurrentBufferNum]->popFront(pMessage))
	{
		if( pMessage->getScheduledTime() <= currentTime )
		{
			pMessage->process();
			delete pMessage;
		}
		else
		{
			//not time to process yet - push to back
			mMessageBuffers[mPendingBufferNum]->pushBack(pMessage);
		}
	}

}

void GameMessageManager::addMessage( GameMessage* pMessage, int delay )
{
	double currentTime = gpGame->getCurrentTime();

	//set frame numbers
	pMessage->mSubmittedTime = currentTime;
	pMessage->mScheduledTime = currentTime + delay;

	//put it in the message list
	bool success = mMessageBuffers[mPendingBufferNum]->pushBack( pMessage );

	//there was no room for the message
	if (!success)
	{
		cout << "WARNING - trying to add message to full message buffer!\n";
		delete pMessage;
	}
	//cout << "message:" << pMessage->mType << " added to buffer:" << mPendingBufferNum << endl;
	//assert(success);
}
#pragma once

#include "Trackable.h"
#include "CircularQueue.h"

class GameMessage;

class GameMessageManager: public Trackable
{
public:
	explicit GameMessageManager(Uint32 size=64);
	~GameMessageManager();

	void addMessage( GameMessage* pMessage, int delay = 0 );//note - message manager will now "own" the message and will delete it at its pleasure
	void processMessagesForThisframe();

private:

	CircularQueue<GameMessage*>* mMessageBuffers[2];
	Uint32 mCurrentBufferNum = 0;
	Uint32 mPendingBufferNum = 1;
};


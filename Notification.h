#pragma once

/*#include <Trackable.h>

//Notifications are messages that are intended to be queued up for later processing
//     They contain callback functions which will actually handle the Notification when it is time to process them


class Notification;
class NotificationQueue;

typedef void(*NotificationCallback)(Notification*);

class Notification :public Trackable
{
	friend class NotificationQueue;
public:
	Notification(){ mID = msID++; };
	inline unsigned int getID(){ return mID; };
private:
	unsigned int mID;
	NotificationCallback mCallback;

	static unsigned int msID;
};*/
#pragma once

#include <vector>
#include <Trackable.h>
#include "Notification.h"

//NotificationQueue's hold lists of Notifications which are processed in order
//     each Notification will call their own callback with the Notification as the parameter

//class NotificationQueue :public Trackable
//{
/*public:
	NotificationQueue(unsigned int size, NotificationCallback callback) :mCallback(callback){};
	~NotificationQueue(){};

	void addNotification(const Notification& theNotification);
	void processNotifications();

private:
	std::vector<Notification> mNotifications;
	void doCallback(Notification* pMessage){ mCallback(pMessage); };*/
//};


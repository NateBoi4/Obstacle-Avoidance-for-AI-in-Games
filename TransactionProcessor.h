#pragma once

#include <Trackable.h>

//TransactionProcessor class is an abstract base class which mandates a post function - which will do something with a passed in Transaction

struct Transaction;

class TransactionProcessor : public Trackable
{
public:
	void virtual post(const Transaction& theTransaction) = 0;

};
#pragma once

#include <Trackable.h>
#include <Vector2D.h>

enum TransactionType
{
	INVALID_TRANSACTION_TYPE = -1,
	POSITION_TRANSACTION,
	PHYSICS_TRANSACTION
};

//Transactions are how components modify TransactionProcessors

struct Transaction: public Trackable
{
	Transaction(TransactionType theType) :type(theType){};

	TransactionType type;
};

struct PositionTransaction : public Transaction
{
	PositionTransaction(const Vector2D& thePosMod, float theFacingMod) :Transaction(POSITION_TRANSACTION), posMod(thePosMod), facingMod(theFacingMod){};
	PositionTransaction() :Transaction(POSITION_TRANSACTION), posMod(ZERO_VECTOR2D), facingMod(0.0f){};

	Vector2D posMod;//how much do we change the position
	float facingMod;//how much do we rotate
};
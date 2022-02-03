#pragma once

#include "GameMessage.h"
#include "Vector2D.h"

class Vector2D;

class DeleteUnitMessage : public GameMessage
{
public:
	DeleteUnitMessage();
	~DeleteUnitMessage();

	void process();
};


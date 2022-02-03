#pragma once

#include "GameMessage.h"
#include "Vector2D.h"

class Vector2D;

class CreateUnitMessage :public GameMessage
{
public:
	CreateUnitMessage();
	~CreateUnitMessage();

	void process();

private:
};
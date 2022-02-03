#pragma once

#include <Trackable.h>
#include "Steering.h"
#include <Vector2D.h>

class ObstacleAvoidance : public Steering
{
public:
	ObstacleAvoidance(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID);

protected:
	Steering* getSteering();
	bool checkForTopLeftWall(Vector2D ray);
	bool checkForBottomRightWall(Vector2D ray);
};


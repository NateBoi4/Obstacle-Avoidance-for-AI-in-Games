#pragma once
#include <Trackable.h>
#include "Steering.h"

class BlendedSteering : public Steering
{
public:
	BlendedSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID = INVALID_UNIT_ID);

	struct BehaviorAndWeight
	{
		Steering* behavior;
		float weight;
	};

protected:
	virtual Steering* getSteering();

private:
	static int mTargetBehavior;

	static const int MAX_BEHAVIORS = 2;

	static BehaviorAndWeight mBehaviors[MAX_BEHAVIORS];
};


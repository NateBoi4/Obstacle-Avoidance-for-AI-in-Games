#include "Steering.h"


Steering::Steering(SteeringType type, const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	:mType(type)
	, mOwnerID(ownerID)
	, mTargetLoc(targetLoc)
	, mTargetID(targetID)
	, mData(PhysicsComponent::getZeroPhysicsData())
{
}

void Steering::update()
{
	Steering* pTemp = getSteering();
	if (pTemp != NULL)
	{
		mData = pTemp->getData();
	}
}

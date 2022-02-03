#include <cassert>

#include "WanderSteering.h"
#include "Steering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Vector2D.h"

WanderSteering::WanderSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering()
{
	mType = Steering::WANDER;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* WanderSteering::getSteering()
{
	//Make Data Driven
	//Radius and Forward Offset of the wander circle
	float wanderOffset = 2.0f;
	float wanderRadius = 0.4f;
	//Maximum rate at which wander orientation can change
	float wanderRate = 0.1f;
	//Current Orientation of wander target
	float wanderOrient = 1.0f;

	//(Data) character
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (mTargetID != INVALID_UNIT_ID)
	{
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}
	
	//Update wander orientation
	//wanderOrientation += rand * wanderRate
	wanderOrient += rand() * wanderRate;

	//Calculate combined target orientation
	//targetOrientation = wanderOrientation + characterOrientation
	data.rotVel = wanderOrient + pOwner->getPhysicsComponent()->getRotationalVelocity();

	//Calculate center of wander circle
	//target = characterPosition + wanderOffset * characterOrientation
	mTargetLoc = pOwner->getPositionComponent()->getPosition() + Vector2D::getVectorInDirection(pOwner->getPhysicsComponent()->getRotationalVelocity()) * wanderOffset;
	
	//Calculate target location
	//target += wanderRadius * targetOrientation
	mTargetLoc += Vector2D::getVectorInDirection(data.rotVel) * wanderRadius;

	//face target
	FaceSteering face(mOwnerID, mTargetLoc);
	face.update();
	PhysicsData faceData = face.getData();
	data.rotAcc = faceData.rotAcc;
	data.rotVel = faceData.rotVel;

	//Accelerate in direction of the orientation
	//linear = maxAcceleration * characterOrientation
	data.acc = Vector2D::getVectorInDirection(pOwner->getPhysicsComponent()->getRotationalVelocity()) * pOwner->getMaxAcc();

	//Output steering
	//return steering
	this->mData = data;
	return this;
}

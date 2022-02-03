#include <cassert>

#include "WanderAndChaseSteering.h"
#include "Steering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "Vector2D.h"

WanderAndChaseSteering::WanderAndChaseSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering()
{
	mType = Steering::WANDER_AND_CHASE;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* WanderAndChaseSteering::getSteering()
{
	//Radius and Forward Offset of the wander circle
	float wanderOffset = 1.0f;
	float wanderRadius = 1.0f;
	//Maximum rate at which wander orientation can change
	float wanderRate = 1.0f;
	//Current Orientation of wander target
	float wanderOrient = 1.0f;
	//Radius to determine whether to wander or chase
	float targetRadius = 200.0f;

	//(Data) character
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	if (mTargetID != INVALID_UNIT_ID)
	{
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	Vector2D dir = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	float dist = dir.getLength();

	if (dist > targetRadius)
	{
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
	}
	else 
	{
		dir.normalize(); //steering.linear.normalize()
		dir *= pOwner->getMaxAcc(); //steering.linear *= maxAcceleration

		FaceSteering face(mOwnerID, mTargetLoc);
		face.update();
		PhysicsData faceData = face.getData();

		data.acc = dir;
		data.rotAcc = faceData.rotAcc;
		data.rotVel = faceData.rotVel;
	}

	//Output steering
	//return steering
	this->mData = data;
	return this;
}

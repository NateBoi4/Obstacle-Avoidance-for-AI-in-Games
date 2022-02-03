#include <cassert>

#include "ArriveAndFaceSteering.h"
#include "Steering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"
#include "FaceSteering.h"


ArriveAndFaceSteering::ArriveAndFaceSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	: Steering()
{
	mType = Steering::ARRIVE_AND_FACE;
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* ArriveAndFaceSteering::getSteering()
{
	//(data) character, target
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	//radius - satisfaction radius/arrived at target
	float radius = 0.2f;
	//slowRad - radius to begin to slow down
	float slowRadius = 200.0f;
	//timeToTarget - time to target constant
	float timeToTarget = 0.1f;

	if (mTargetID != INVALID_UNIT_ID)
	{
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	//Get direction to target
	//direction = targetPos - characterPos, distance = direction.length()
	Vector2D dir;
	dir = mTargetLoc - pOwner->getPositionComponent()->getPosition();
	float dist;
	dist = dir.getLength();

	//Check if within satisfaction radius
	//if(within radius) -> return nullptr
	if (dist < radius)
	{
		return nullptr;
	}

	//if(outside slow radius) -> speed = maxSpeed
	//else -> speed = maxSpeed * distance / slowRad
	if (dist > slowRadius)
	{
		//speed = maxSpeed;
		data.maxSpeed = pOwner->getMaxSpeed();
	}
	else
	{
		//speed = maxSpeed * dist / slowRadius
		data.maxSpeed = pOwner->getMaxSpeed() * dist / slowRadius;
	}

	//target velocity combines speed and direction
	//targetVel = direction, targetVel.normalize, targetVel *= speed
	data.vel = dir;
	data.vel.normalize();
	data.vel *= data.maxSpeed;

	//Acceleration tries to get to targetVel
	//linear = targetVel - character velocity, linear /= timeToTarget
	data.acc = data.vel - pOwner->getPhysicsComponent()->getVelocity();
	data.acc /= timeToTarget;

	//Check if Acceleration is too fast
	//if(linear.length() > maxAccel) -> linear.normalize, linear *= maxAccel
	if (data.acc.getLength() > pOwner->getMaxAcc())
	{
		data.acc.normalize();
		data.acc *= pOwner->getMaxAcc();
	}

	//Face direction we want to move - ONLY FOR ARRIVE AND FACE
	FaceSteering face(mOwnerID, mTargetLoc);
	face.update();
	PhysicsData faceData = face.getData();
	data.rotAcc = faceData.rotAcc;
	data.rotVel = faceData.rotVel;

	//Output steering
	//return steering
	this->mData = data;
	return this;
}

#include <cassert>

#include "Steering.h"
#include "SeekSteering.h"
#include "FaceSteering.h"
#include "Game.h"
#include "UnitManager.h"
#include "Unit.h"


SeekSteering::SeekSteering(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID, bool shouldFlee /*= false*/)
	: Steering()
{
	if (shouldFlee)
	{
		mType = Steering::FLEE;
	}
	else
	{
		mType = Steering::SEEK;
	}
	setOwnerID(ownerID);
	setTargetID(targetID);
	setTargetLoc(targetLoc);
}

Steering* SeekSteering::getSteering()
{
	Vector2D diff;
	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	//are we seeking a location or a unit?
	
	if (mTargetID != INVALID_UNIT_ID)
	{
		//seeking unit
		Unit* pTarget = gpGame->getUnitManager()->getUnit(mTargetID);
		assert(pTarget != NULL);
		mTargetLoc = pTarget->getPositionComponent()->getPosition();
	}

	if (mType == Steering::SEEK)
	{
		diff = mTargetLoc - pOwner->getPositionComponent()->getPosition(); //steering.linear = target.position - character.position
	}
	else
	{
		diff = pOwner->getPositionComponent()->getPosition() - mTargetLoc;
	}

	diff.normalize(); //steering.linear.normalize()
	diff *= pOwner->getMaxAcc(); //steering.linear *= maxAcceleration

	FaceSteering face(mOwnerID, mTargetLoc);
	face.update();
	PhysicsData faceData = face.getData();

	PhysicsData data = pOwner->getPhysicsComponent()->getData();
	data.acc = diff;
	data.rotAcc = faceData.rotAcc;
	data.rotVel = faceData.rotVel;

	this->mData = data;
	return this;
}


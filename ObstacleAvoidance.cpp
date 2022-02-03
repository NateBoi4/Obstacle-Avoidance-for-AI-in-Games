#include "ObstacleAvoidance.h"
#include "Game.h"
#include "UnitManager.h"
#include "SeekSteering.h"
#include "ArriveSteering.h"
#include "GraphicsSystem.h"
#include "GraphicsBufferManager.h"
#include "GraphicsBuffer.h"
#include "Color.h"
#include <cmath>
#include <iostream>

ObstacleAvoidance::ObstacleAvoidance(const UnitID& ownerID, const Vector2D& targetLoc, const UnitID& targetID)
	:Steering(Steering::AVOID, ownerID, targetLoc, targetID)
{
}

Steering* ObstacleAvoidance::getSteering()
{
	float avoidDist = 2000.0f;

	float lookAhead = 100.0f;

	Unit* pOwner = gpGame->getUnitManager()->getUnit(mOwnerID);
	PhysicsData data = pOwner->getPhysicsComponent()->getData();

	Vector2D rayVector = pOwner->getHeadingVector();
	rayVector.normalize();
	rayVector *= lookAhead;

	rayVector += pOwner->getPositionComponent()->getPosition();

	//DRAW THIS SOMEWHERE ELSE
	gpGame->getGraphicsSystem()->renderLine(*GraphicsSystem::getBackBuffer(), pOwner->getPositionComponent()->getPosition() + pOwner->getSprite().getCenterOffset(),
		rayVector, BLACK_COLOR);

	gpGame->getGraphicsSystem()->flip();

	bool collision = false; //Create Collision Detection. Either function or class.
	bool hitBottomRightWall = false;
	bool hitTopLeftWall = false;
	UnitID targetID = INVALID_UNIT_ID;

	//Collision Function could look something like this:
	hitBottomRightWall = checkForBottomRightWall(rayVector);
	hitTopLeftWall = checkForTopLeftWall(rayVector);

	collision = gpGame->getUnitManager()->hitUnitSprite(rayVector, targetID);

	//std::cout << collision << std::endl;

	if (!collision && !hitBottomRightWall && !hitTopLeftWall) 
	{
		this->mData = data;
		return this;
	}

	//Set our new target to delegate to seek.
	if (collision) 
	{
		mTargetLoc = gpGame->getUnitManager()->getUnit(targetID)->getPositionComponent()->getPosition()
			+ gpGame->getUnitManager()->getUnit(targetID)->getPositionComponent()->getPosition().getNormalizedVector() * avoidDist;
	}
	else if(hitBottomRightWall)
	{
		mTargetLoc = rayVector - rayVector.getNormalizedVector() * avoidDist;
	}
	else if (hitTopLeftWall) 
	{
		mTargetLoc = rayVector + rayVector.getNormalizedVector() * avoidDist;
	}

	//std::cout << mTargetLoc << std::endl;

	/*SeekSteering seek(mOwnerID, mTargetLoc);
	seek.update();
	PhysicsData seekData = seek.getData();*/

	ArriveSteering arrive(mOwnerID, mTargetLoc);
	arrive.update();
	PhysicsData arriveData = arrive.getData();

	data = arriveData;

	this->mData = data;
	return this;
}

bool ObstacleAvoidance::checkForTopLeftWall(Vector2D ray) 
{
	if (ray.getX() < 0.0f 
		|| ray.getY() < 0.0f) 
	{
		return true;
	}
	return false;
}

bool ObstacleAvoidance::checkForBottomRightWall(Vector2D ray)
{
	if (ray.getX() > gpGame->getGraphicsSystem()->getDisplayWidth()
		|| ray.getY() > gpGame->getGraphicsSystem()->getDisplayHeight())
	{
		return true;
	}
	return false;
}

#pragma once

/*Steering - implementation of Steering class from "Artificial Intelligence for Games" - Millington and Funge
*/

#include <Vector2D.h>
#include <Trackable.h>
#include "defines.h"
#include "PhysicsComponent.h"



class Steering: public Trackable
{
public:
	enum SteeringType
	{
		INVALID_TYPE = -1,
		SEEK,
		FLEE,
		ARRIVE,
		ARRIVE_AND_FACE,
		FACE,
		WANDER,
		WANDER_AND_CHASE,
		AVOID,
		BLEND
	};

	//constructors and destructors
	Steering(SteeringType type = INVALID_TYPE, const UnitID& ownerID = INVALID_UNIT_ID, const Vector2D& targetLoc = ZERO_VECTOR2D, const UnitID& targetID = INVALID_UNIT_ID);
	virtual ~Steering(){};

	//accessors
	void setTargetLoc(const Vector2D& loc) { mTargetLoc = loc; };
	const Vector2D& getTargetLoc() const { return mTargetLoc; };
	void setTargetID(const UnitID& id) { mTargetID = id; };
	const UnitID& getTargetID() const { return mTargetID; };
	void setOwnerID(const UnitID& id) { mOwnerID = id; };
	//void setData(const PhysicsData& data) { mData = data; };
	const PhysicsData& getData() const { return mData; };
	void update();
protected:
	SteeringType mType;
	Vector2D mTargetLoc;
	UnitID mTargetID;
	UnitID mOwnerID;
	PhysicsData mData;

	virtual Steering* getSteering() { return this; };//overridden by sub-classes

};

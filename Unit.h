#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <limits>
#include <Vector2D.h>

#include "Component.h"
#include "PositionComponent.h"
#include "Sprite.h"
#include "Steering.h"

class PhysicsComponent;
class SteeringComponent;
class Sprite;
class UnitManager;

class Unit : public Trackable
{
public:
	void draw() const;
	float getFacing() const;
	Vector2D getHeadingVector() const;

	void update(float elapsedTime){};

	PositionComponent* getPositionComponent() const;
	PhysicsComponent* getPhysicsComponent() const;
	SteeringComponent* getSteeringComponent() const;
	float getMaxAcc() const { return mMaxAcc; };
	float getMaxSpeed() const { return mMaxSpeed; };
	float getMaxRotAcc() const { return mMaxRotAcc; };
	float getMaxRotVel() const { return mMaxRotVel; };
	void setShowTarget(bool val) { mShowTarget = val; };
	UnitID getID() const { return mID; };
	Sprite getSprite() { return mSprite; };

	void setSteering(Steering::SteeringType type, Vector2D targetLoc = ZERO_VECTOR2D, UnitID targetUnitID = INVALID_UNIT_ID);

private:
	UnitID mID;
	ComponentID mPhysicsComponentID;
	ComponentID mPositionComponentID;
	ComponentID mSteeringComponentID;
	PositionComponent* mpPositionComponent = NULL;
	PhysicsComponent* mpPhysicsComponent = NULL;
	SteeringComponent* mpSteeringComponent = NULL;
	Sprite mSprite;
	float mMaxAcc = 0.0f;
	float mMaxSpeed = 0.0f;
	float mMaxRotAcc = 0.0f;
	float mMaxRotVel = 0.0f;
	bool mShowTarget = false;

	Unit(const Sprite& sprite);
	virtual ~Unit();

	Unit(Unit&);//invalidate copy constructor
	void operator=(Unit&);//invalidate assignment operator

	friend class UnitManager;
};

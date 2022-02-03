#pragma once

#include "Component.h"
#include "Game.h"
#include "DataLoader.h"
#include <DataRepository.h>
#include <Vector2D.h>
#include <Trackable.h>
#include <cfloat>

class PositionComponent;

struct PhysicsData : public Trackable
{
	PhysicsData(
		const Vector2D& theVel
		, const Vector2D& theAcc
		, float theRotVel
		, float theRotAcc
		, float theMaxSpeed = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_SPEED).getFloatVal()
		, float theMaxAcc = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_ACC).getFloatVal()
		, float theMaxRotVel = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_ROT_VEL).getFloatVal()
		, float theMaxRotAcc = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_ROT_ACC).getFloatVal())
		:vel(theVel)
		, acc(theAcc)
		, rotVel(theRotVel)
		, rotAcc(theRotAcc)
		, maxSpeed(theMaxSpeed)
		, maxAccMagnitude(theMaxAcc)
		, maxRotVel(theMaxRotVel)
		, maxRotAcc(theMaxRotAcc)
	{};

	PhysicsData() 
		:vel(ZERO_VECTOR2D)
		, acc(ZERO_VECTOR2D)
		, rotVel(0.0f)
		, rotAcc(0.0f)
		, maxSpeed(gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_SPEED).getFloatVal())
		, maxAccMagnitude(gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_ACC).getFloatVal())
		, maxRotVel(gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_ROT_VEL).getFloatVal())
		, maxRotAcc(gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_ROT_ACC).getFloatVal())
	{};

	Vector2D vel;
	Vector2D acc;
	float rotVel;
	float rotAcc;
	float maxSpeed;
	float maxAccMagnitude;
	float maxRotVel;
	float maxRotAcc;
};

class PhysicsComponent :public Component
{
public:

	void setData(const PhysicsData& data){ mData = data; };
	void modData(const PhysicsData& data);
	const PhysicsData& getData() const { return mData; };
	const Vector2D& getVelocity() const { return mData.vel; };
	const Vector2D& getAcceleration() const { return mData.acc; };
	float getRotationalVelocity() const { return mData.rotVel; };
	float getRotationalAcceleration() const { return mData.rotAcc; };

	static const PhysicsData& getZeroPhysicsData() { static PhysicsData data; return data; };

private:
	PhysicsData mData;
	ComponentID mPositionComponentID;
	PositionComponent* mpPositionComponent=NULL;

	PhysicsComponent(const ComponentID& id, const ComponentID& posID) :Component(id), mPositionComponentID(posID){};
	~PhysicsComponent(){};

	void integrate(PositionComponent& posComponent, float elapsedTime);

	void setPositionComponent(PositionComponent* pComp) { mpPositionComponent = pComp; };

	friend class ComponentManager;
};
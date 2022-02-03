#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <MemoryPool.h>
#include <unordered_map>
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "Unit.h"

class Unit;
class Sprite;
struct PositionData;
struct PhysicsData;

const UnitID PLAYER_UNIT_ID = 0;


class UnitManager : public Trackable
{
public:
	UnitManager(Uint32 maxSize);
	~UnitManager(){};

	Unit* createUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = PositionComponent::getZeroPositionData(), const PhysicsData& physicsData = PhysicsComponent::getZeroPhysicsData(), const UnitID& id = INVALID_UNIT_ID);
	Unit* createPlayerUnit(const Sprite& sprite, bool shouldWrap = true, const PositionData& posData = PositionComponent::getZeroPositionData(), const PhysicsData& physicsData = PhysicsComponent::getZeroPhysicsData());
	Unit* createRandomUnit(const Sprite& sprite);

	Unit* createSphereUnit(const Sprite& sprite);

	Unit* getUnit(const UnitID& id) const;
	void deleteUnit(const UnitID& id);
	void deleteRandomUnit();

	void drawAll() const;
	void updateAll(float elapsedTime);

	bool hitUnitSprite(Vector2D ray, UnitID& target);

	bool countCollisions();

	bool checkSpriteOverlap(Unit* pOne, Unit* pTwo);

	void switchSteering(bool steer);

	Unit* getPlayerUnit() const { return getUnit(PLAYER_UNIT_ID); };

	Uint32 getNumUnits() const { return mUnitMap.size(); };
private:
	static UnitID msNextUnitID;
	MemoryPool mPool;
	std::unordered_map<UnitID, Unit*> mUnitMap;
};


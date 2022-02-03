#include <map>

#include "UnitManager.h"
#include "Unit.h"
#include "Game.h"
#include "ComponentManager.h"
#include "GraphicsSystem.h"

UnitID UnitManager::msNextUnitID = PLAYER_UNIT_ID + 1;

using namespace std;

UnitManager::UnitManager(Uint32 maxSize)
	:mPool(maxSize, sizeof(Unit))
{
}

Unit* UnitManager::createUnit(const Sprite& sprite, bool shouldWrap, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/, const UnitID& id)
{
	Unit* pUnit = NULL;

	Byte* ptr = mPool.allocateObject();
	if (ptr != NULL)
	{
		//create unit
		pUnit = new (ptr)Unit(sprite);//placement new

		UnitID theID = id;
		if (theID == INVALID_UNIT_ID)
		{
			theID = msNextUnitID;
			msNextUnitID++;
		}

		//place in map
		mUnitMap[theID] = pUnit;

		//assign id and increment nextID counter
		pUnit->mID = theID;

		//create some components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		ComponentID id = pComponentManager->allocatePositionComponent(posData,shouldWrap);
		pUnit->mPositionComponentID = id;
		pUnit->mpPositionComponent = pComponentManager->getPositionComponent(id);
		pUnit->mPhysicsComponentID = pComponentManager->allocatePhysicsComponent(pUnit->mPositionComponentID, physicsData);
		pUnit->mpPhysicsComponent = pComponentManager->getPhysicsComponent(id);
		pUnit->mSteeringComponentID = pComponentManager->allocateSteeringComponent(pUnit->mPhysicsComponentID);
		pUnit->mpSteeringComponent = pComponentManager->getSteeringComponent(id);

		//set max's
		pUnit->mMaxSpeed = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_SPEED).getFloatVal();
		pUnit->mMaxAcc = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_ACC).getFloatVal();
		pUnit->mMaxRotAcc = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_ROT_ACC).getFloatVal();
		pUnit->mMaxRotVel = gpGame->getDataRepository()->getEntry(DataKeyEnum::MAX_ROT_VEL).getFloatVal();
	}

	return pUnit;
}


Unit* UnitManager::createPlayerUnit(const Sprite& sprite, bool shouldWrap /*= true*/, const PositionData& posData /*= ZERO_POSITION_DATA*/, const PhysicsData& physicsData /*= ZERO_PHYSICS_DATA*/)
{
	return createUnit(sprite, shouldWrap, posData, physicsData, PLAYER_UNIT_ID);
}


Unit* UnitManager::createRandomUnit(const Sprite& sprite)
{

	int posX = rand() % GraphicsSystem::getDisplayWidth();
	int posY = rand() % GraphicsSystem::getDisplayHeight();
	int velX = rand() % 50 - 25;
	int velY = rand() % 40 - 20;
	Unit* pUnit = createUnit(sprite, true, PositionData(Vector2D(posX,posY),0), PhysicsData(Vector2D(velX,velY),Vector2D(0.1f,0.1f), 0.1f, 0.05f));
	if (pUnit != NULL)
	{
		//New units use Wander and Chase steering with the player unit as the target
		pUnit->setSteering(Steering::WANDER_AND_CHASE, Vector2D(), gpGame->getUnitManager()->getPlayerUnit()->getID());
	}
	return pUnit;
}

Unit* UnitManager::createSphereUnit(const Sprite& sprite)
{

	int posX = rand() % GraphicsSystem::getDisplayWidth();
	int posY = rand() % GraphicsSystem::getDisplayHeight();
	int velX = rand() % 50 - 25;
	int velY = rand() % 40 - 20;
	Unit* pUnit = createUnit(sprite, true, PositionData(Vector2D(posX, posY), 0), PhysicsData(Vector2D(velX, velY), Vector2D(0.1f, 0.1f), 0.1f, 0.05f));
	//if (pUnit != NULL)
	//{
	//	//New units use Wander and Chase steering with the player unit as the target
	//	pUnit->setSteering(Steering::WANDER_AND_CHASE, Vector2D(), gpGame->getUnitManager()->getPlayerUnit()->getID());
	//}
	return pUnit;
}

Unit* UnitManager::getUnit(const UnitID& id) const
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void UnitManager::deleteUnit(const UnitID& id)
{
	auto it = mUnitMap.find(id);
	if (it != mUnitMap.end())//found?
	{
		Unit* pUnit = it->second;//hold for later

		//remove from map
		mUnitMap.erase(it);

		//remove components
		ComponentManager* pComponentManager = gpGame->getComponentManager();
		pComponentManager->deallocatePhysicsComponent(pUnit->mPhysicsComponentID);
		pComponentManager->deallocatePositionComponent(pUnit->mPositionComponentID);
		pComponentManager->deallocateSteeringComponent(pUnit->mSteeringComponentID);

		//call destructor
		pUnit->~Unit();

		//free the object in the pool
		mPool.freeObject((Byte*)pUnit);
	}
}

void UnitManager::deleteRandomUnit()
{
	if (mUnitMap.size() >= 1)
	{
		Uint32 target = rand() % mUnitMap.size();
		if (target == 0)//don't allow the 0th element to be deleted as it is the player unit
		{
			target = 1;
		}
		Uint32 cnt = 0;
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it, cnt++)
		{
			if (cnt == target)
			{
				//failsafe against deleting player unit
				if( getPlayerUnit() != it->second )
					deleteUnit(it->first);
				break;
			}
		}
	}
}

void UnitManager::drawAll() const
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->draw();
	}
}

void UnitManager::updateAll(float elapsedTime)
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		it->second->update(elapsedTime);
	}
}

bool UnitManager::hitUnitSprite(Vector2D ray, UnitID& target) 
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it)
	{
		if (ray.getX() > it->second->getPositionComponent()->getPosition().getX() && 
			ray.getX() < it->second->getPositionComponent()->getPosition().getX() + it->second->getSprite().getWidth() &&
			ray.getY() > it->second->getPositionComponent()->getPosition().getY() && 
			ray.getY() < it->second->getPositionComponent()->getPosition().getY() + it->second->getSprite().getHeight())
		{
			target = it->second->getID();
			return true;
		}
	}
	return false;
}

bool UnitManager::countCollisions() 
{
	for (auto it = mUnitMap.begin(); it != mUnitMap.end(); ++it) 
	{
		if (it->second != getPlayerUnit()) 
		{
			if (checkSpriteOverlap(getPlayerUnit(), it->second)) 
			{
				//gpGame->addCollision();
				return true;
			}
		}
	}
	return false;
}

bool UnitManager::checkSpriteOverlap(Unit* pOne, Unit* pTwo) 
{
	if (pOne->getPositionComponent()->getPosition().getX() >= pTwo->getPositionComponent()->getPosition().getX()
		&& pOne->getPositionComponent()->getPosition().getX() <= pTwo->getPositionComponent()->getPosition().getX() + pTwo->getSprite().getWidth()
		&& pOne->getPositionComponent()->getPosition().getY() >= pTwo->getPositionComponent()->getPosition().getY()
		&& pOne->getPositionComponent()->getPosition().getY() <= pTwo->getPositionComponent()->getPosition().getY() + pTwo->getSprite().getHeight()) 
	{
		return true;
	}
	return false;
}

//Toggle AI to switch between seeking the player unit and fleeing from it.
//Take an input to determine if the AI are already fleeing or seeking.
void UnitManager::switchSteering(bool steer) 
{
	if (steer) 
	{
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); it++)
		{
			//Go through every existing unit that is not the player.
			if (it->second->getID() != getPlayerUnit()->getID())
			{
				//Set units to flee from the player unit and wrap around the screen
				it->second->setSteering(Steering::FLEE, ZERO_VECTOR2D, getPlayerUnit()->getID());
				it->second->getPositionComponent()->setWrap(true);
			}
		}
	}
	else 
	{
		for (auto it = mUnitMap.begin(); it != mUnitMap.end(); it++)
		{
			if (it->second->getID() != getPlayerUnit()->getID())
			{
				//Set units to seek the player unit but not wrap around the screen
				it->second->setSteering(Steering::SEEK, Vector2D(), getPlayerUnit()->getID());
				it->second->getPositionComponent()->setWrap(false);
			}
		}
	}
}

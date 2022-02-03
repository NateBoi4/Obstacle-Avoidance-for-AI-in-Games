#include <cassert>
#include "ComponentManager.h"
#include "Component.h"
#include "PositionComponent.h"
#include "PhysicsComponent.h"
#include "SteeringComponent.h"


ComponentID ComponentManager::msNextPositionComponentID = 0;
ComponentID ComponentManager::msNextPhysicsComponentID = 0;
ComponentID ComponentManager::msNextSteeringComponentID = 0;

using namespace std;

ComponentManager::ComponentManager(Uint32 maxSize)
	:mPositionPool(maxSize, sizeof(PositionComponent))
	, mPhysicsPool(maxSize, sizeof(PhysicsComponent))
	, mSteeringPool(maxSize, sizeof(SteeringComponent))
{
}

ComponentManager::~ComponentManager()
{
	clear();
}


void ComponentManager::clear()
{
	//call destructor for all components
	for (auto& it : mPositionComponentMap)
	{
		PositionComponent* pComponent = it.second;
		pComponent->~PositionComponent();
	}
	for (auto& it : mPhysicsComponentMap)
	{
		PhysicsComponent* pComponent = it.second;
		pComponent->~PhysicsComponent();
	}
	for (auto& it : mSteeringComponentMap)
	{
		SteeringComponent* pComponent = it.second;
		pComponent->~SteeringComponent();
	}

	//clear maps
	mPositionComponentMap.clear();
	mPhysicsComponentMap.clear();
	mSteeringComponentMap.clear();

	//reset memory pools
	mPositionPool.reset();
	mPhysicsPool.reset();
	mSteeringPool.reset();
}


PositionComponent* ComponentManager::getPositionComponent(const ComponentID& id)
{
	auto it = mPositionComponentMap.find(id);
	if (it != mPositionComponentMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

ComponentID ComponentManager::allocatePositionComponent(const PositionData& data, bool shouldWrap)
{
	ComponentID newID = INVALID_COMPONENT_ID;

	Byte* ptr = mPositionPool.allocateObject();
	if (ptr != NULL)
	{
		newID = msNextPositionComponentID;
		PositionComponent* pComponent = new (ptr)PositionComponent(newID,shouldWrap);
		pComponent->setData(data);
		mPositionComponentMap[newID] = pComponent;
		msNextPositionComponentID++;//increment id
	}

	return newID;
}

void ComponentManager::deallocatePositionComponent(const ComponentID& id)
{
	auto it = mPositionComponentMap.find(id);

	if (it != mPositionComponentMap.end())//found it
	{
		PositionComponent* ptr = it->second;
		mPositionComponentMap.erase(it);

		ptr->~PositionComponent();
		mPositionPool.freeObject((Byte*)ptr);
	}
}

PhysicsComponent* ComponentManager::getPhysicsComponent(const ComponentID& id)
{
	auto it = mPhysicsComponentMap.find(id);
	if (it != mPhysicsComponentMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}

}

ComponentID ComponentManager::allocatePhysicsComponent(const ComponentID& positionComponentID, const PhysicsData& data)
{
	ComponentID newID = INVALID_COMPONENT_ID;

	Byte* ptr = mPhysicsPool.allocateObject();
	if (ptr != NULL)
	{
		newID = msNextPhysicsComponentID;
		PhysicsComponent* pComponent = new (ptr)PhysicsComponent(newID, positionComponentID);
		pComponent->setData(data);
		pComponent->setPositionComponent(getPositionComponent(positionComponentID));
		mPhysicsComponentMap[newID] = pComponent;
		msNextPhysicsComponentID++;//increment id
	}

	return newID;
}

void ComponentManager::deallocatePhysicsComponent(const ComponentID& id)
{
	auto it = mPhysicsComponentMap.find(id);

	if (it != mPhysicsComponentMap.end())//found it
	{
		PhysicsComponent* ptr = it->second;
		mPhysicsComponentMap.erase(it);

		//hold for later
		Byte* pByte = (Byte*)ptr;

		ptr->~PhysicsComponent();
		mPhysicsPool.freeObject(pByte);
	}
}

SteeringComponent* ComponentManager::getSteeringComponent(const ComponentID& id)
{
	auto it = mSteeringComponentMap.find(id);
	if (it != mSteeringComponentMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}

}

ComponentID ComponentManager::allocateSteeringComponent(const ComponentID& physicsComponentID, const SteeringData& data)
{
	ComponentID newID = INVALID_COMPONENT_ID;

	Byte* ptr = mSteeringPool.allocateObject();
	if (ptr != NULL)
	{
		newID = msNextSteeringComponentID;
		SteeringComponent* pComponent = new (ptr)SteeringComponent(newID, physicsComponentID);
		pComponent->setData(data);
		pComponent->setPhysicsComponent(getPhysicsComponent(physicsComponentID));
		mSteeringComponentMap[newID] = pComponent;
		msNextSteeringComponentID++;//increment id
	}

	return newID;
}

void ComponentManager::deallocateSteeringComponent(const ComponentID& id)
{
	auto it = mSteeringComponentMap.find(id);

	if (it != mSteeringComponentMap.end())//found it
	{
		SteeringComponent* ptr = it->second;
		mSteeringComponentMap.erase(it);

		//hold for later
		Byte* pByte = (Byte*)ptr;

		ptr->~SteeringComponent();
		mSteeringPool.freeObject(pByte);
	}
}

void ComponentManager::update(float elapsedTime)
{
	updateSteering(elapsedTime);
	updatePhysics(elapsedTime);
}

void ComponentManager::updatePhysics(float elapsedTime)
{
	for (auto& it : mPhysicsComponentMap)
	{
		PhysicsComponent* pPhysics = it.second;
		assert(pPhysics != NULL);
		PositionComponent* pPos = pPhysics->mpPositionComponent;
		assert( pPos != NULL);
		pPhysics->integrate(*pPos, elapsedTime);
	}
}

void ComponentManager::updateSteering(float elapsedTime)
{
	for (auto& it : mSteeringComponentMap)
	{
		SteeringComponent* pSteering = it.second;
		assert(pSteering != NULL);
		PhysicsComponent* pPhysics = pSteering->mpPhysicsComponent;
		assert(pPhysics != NULL);
		pSteering->applySteering(*pPhysics);
	}

}

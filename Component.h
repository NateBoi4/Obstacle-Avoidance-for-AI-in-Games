#pragma once

#include <Trackable.h>
#include <DeanLibDefines.h>
#include <limits>

typedef Uint32 ComponentID;
const Uint32 INVALID_COMPONENT_ID = UINT_MAX;

class Component :public Trackable
{
public:
	Component(const ComponentID& id) :mID(id){};
	virtual ~Component(){};

	inline ComponentID getID(){ return mID; };
private:
	ComponentID mID;
};
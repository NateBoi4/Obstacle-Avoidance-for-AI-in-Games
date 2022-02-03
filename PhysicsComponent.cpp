#include <cassert>

#include "PhysicsComponent.h"
#include "PositionComponent.h"

void PhysicsComponent::modData(const PhysicsData& data)
{
	mData.vel += data.vel;
	mData.acc += data.acc;
	mData.rotVel += data.rotVel;
	mData.rotAcc += data.rotAcc;
}

void PhysicsComponent::integrate(PositionComponent& posComponent, float elapsedTime)
{
	//make sure elapsedTime is at least 0.0f
	assert(elapsedTime >= 0.0f);
	if (elapsedTime < 0.0f)
	{
		elapsedTime = 0.0f;
	}

	if (elapsedTime > 0.0f)//no need to do this for no elapsed time
	{
		//cap accelerations
		if (mData.acc.getLengthSquared() > (mData.maxAccMagnitude * mData.maxAccMagnitude))
		{
			mData.acc.normalize();
			mData.acc *= mData.maxAccMagnitude;
		}
		if (mData.rotAcc > mData.maxRotAcc)
		{
			mData.rotAcc = mData.maxRotAcc;
		}
		else if (mData.rotAcc < -mData.maxRotAcc)
		{
			mData.rotAcc = -mData.maxRotAcc;
		}

		//apply accelerations to velocities
		mData.vel += (mData.acc * elapsedTime);
		mData.rotVel += (mData.rotAcc * elapsedTime);

		//cap velocities
		if (mData.vel.getLengthSquared() > (mData.maxSpeed * mData.maxSpeed) )
		{
			mData.vel.normalize();
			mData.vel *= mData.maxSpeed;
			mData.acc = ZERO_VECTOR2D;
		}
		if (mData.rotVel > mData.maxRotVel)
		{
			mData.rotVel = mData.maxRotVel;
			mData.rotAcc = 0.0f;
		}
		else if (mData.rotVel < -mData.maxRotVel)
		{
			mData.rotVel = -mData.maxRotVel;
			mData.rotAcc = 0.0f;
		}

		//apply velocities to position component
		posComponent.modPosition(mData.vel * elapsedTime);
		posComponent.modFacing(mData.rotVel * elapsedTime);
	}

}

#include "PositionComponent.h"
#include "Game.h"
#include "GraphicsSystem.h"

void PositionComponent::wrapCoords()
{
	if (mWrap)
	{
		float width = (float)GraphicsSystem::getDisplayWidth();
		float height = (float)GraphicsSystem::getDisplayHeight();

		float x = mData.pos.getX();
		float y = mData.pos.getY();

		if (x < 0.0f)
		{
			mData.pos.setX(width);
		}
		else if (x > width)
		{
			mData.pos.setX(0.0f);
		}

		if (y < 0.0f)
		{
			mData.pos.setY(height);
		}
		else if (y > height)
		{
			mData.pos.setY(0.0f);
		}
	}
}


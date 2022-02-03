#pragma once
#include <Trackable.h>
#include <DataRepository.h>
#include <string>

class DataLoader : public Trackable
{
public:
	DataLoader( const std::string& filename, DataRepository* pDepository );
	~DataLoader(){};
private:

};

//class only exists to hold a class level enum
class DataKeyEnum
{
public:
	enum DataKeyVals
	{
		TARGET_FPS,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		MAX_UNITS,
		MAX_ACC,
		MAX_SPEED,
		MAX_ROT_ACC,
		MAX_ROT_VEL,
		WALLPAPER,
		ARROW,
		ENEMY_ARROW,
		TARGET,
		SPAWN_COUNT
	};

};

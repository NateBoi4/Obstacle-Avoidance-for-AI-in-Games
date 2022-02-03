#include "DataLoader.h"
#include "DataRepository.h"
#include <fstream>
#include <sstream>
#include <windows.h>

using namespace std;

DataLoader::DataLoader(const string& filename, DataRepository* pRepository)
{
	ifstream input( filename );
	if( input.good() )
	{
		while( !input.eof() )
		{
			string line;
			getline( input, line);
			stringstream sstream( line );
			string id;
			sstream >> id;

			if( id == "screen_size" )
			{
				UINT width, height;
				sstream >> width >> height;
				pRepository->addEntry( DataKeyEnum::SCREEN_WIDTH, width );
				pRepository->addEntry( DataKeyEnum::SCREEN_HEIGHT, height );
			}
			else if (id == "target_fps")
			{
				UINT fps;
				sstream >> fps;
				pRepository->addEntry(DataKeyEnum::TARGET_FPS, fps);
			}
			else if (id == "max_units")
			{
				UINT max;
				sstream >> max;
				pRepository->addEntry(DataKeyEnum::MAX_UNITS, max);
			}
			else if (id == "unit_max_acc")
			{
				float max;
				sstream >> max;
				pRepository->addEntry(DataKeyEnum::MAX_ACC, max);
			}
			else if (id == "unit_max_speed")
			{
				float max;
				sstream >> max;
				pRepository->addEntry(DataKeyEnum::MAX_SPEED, max);
			}
			else if (id == "unit_max_rot_acc")
			{
				float max;
				sstream >> max;
				pRepository->addEntry(DataKeyEnum::MAX_ROT_ACC, max);
			}
			else if (id == "unit_max_rot_vel")
			{
				float max;
				sstream >> max;
				pRepository->addEntry(DataKeyEnum::MAX_ROT_VEL, max);
			}
			else if (id == "wallpaper")
			{
				string text;
				sstream >> text;
				pRepository->addEntry(DataKeyEnum::WALLPAPER, text);
			}
			else if (id == "arrow")
			{
				string text;
				sstream >> text;
				pRepository->addEntry(DataKeyEnum::ARROW, text);
			}
			else if (id == "enemy_arrow")
			{
				string text;
				sstream >> text;
				pRepository->addEntry(DataKeyEnum::ENEMY_ARROW, text);
			}
			else if (id == "target")
			{
				string text;
				sstream >> text;
				pRepository->addEntry(DataKeyEnum::TARGET, text);
			}
			else if (id == "spawn_count")
			{
				float count;
				sstream >> count;
				pRepository->addEntry(DataKeyEnum::SPAWN_COUNT, count);
			}
			else
			{
				cout << "Unknown tag:" << id << endl;
			}
		}
	}

}



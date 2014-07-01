/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   MapManager.cpp
				created:    6/10/2012
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "MapLoader.h"
#include "MapManager.h"
#include "ObjectManager.h"
#include <math.h>
#include <ostream>
#include "GameApp.h"
#include "ContextApp.h"
#include "Config.h"
#include "GUIManager.h"

namespace Tank
{
	//----------------------------------------------------------------------------------------

	std::string GAME_MODE_STR[3] = { "Single Player Mode", "Two Player Mode", "Multiple Player Mode"};

	//----------------------------------------------------------------------------------------

	std::string getGameModeTypeStr(GAME_MODE_TYPE modeType)
	{
		return GAME_MODE_STR[modeType];
	}

	//----------------------------------------------------------------------------------------

	GAME_MODE_TYPE getGameModeType(const std::string &gameMode)
	{
		if (gameMode == getGameModeTypeStr(GAME_MODE_ONE))
			return GAME_MODE_ONE;
		else if (gameMode == getGameModeTypeStr(GAME_MODE_TWO))
			return GAME_MODE_TWO;
		else
			return GAME_MODE_MULTIPLE;
	}

	//----------------------------------------------------------------------------------------

	MapManager::MapManager()
	{

		std::string path = TANKWAR_RES_ROOT_PATH + "map/";

		Util::loadResouse(path, FILE_TYPE);

		Config maps(TANKWAR_RES_ROOT_PATH + "map/maps.cfg");
		int mapSize = maps.getFloat("MAPS", "SIZE");

		std::string mapName;
		for(int i = 1; i <= mapSize; i++)
		{
			mapName = "map" + Util::toString(i);

			MapPre mapPre;
			{
				mapPre.fileName				= path + maps.getString(mapName, "MAP");
				mapPre.name						= maps.getString(mapName, "NAME");
				mapPre.image						= maps.getString(mapName, "IMAGE");
				mapPre.enemyNumber	= maps.getFloat(mapName, "ENEMYNUMBER");
				mapPre.maxEnemyInScene	= maps.getFloat(mapName, "MAXENEMYNUMBER");

				
				GUIManager::createImageset(mapPre.name, mapPre.image);
			}

			mMaps.push_back(mapPre);
		}
	}

	//----------------------------------------------------------------------------------------

	Ogre::Vector3 MapManager::getStartPosition(TANK_TYPE tank, int index)
	{
		if (tank == TANK_GREEN)
			return mMapInfo.mBirthPoint[index];
		else
			return mMapInfo.mEnemyBirthPoint[index];
	}

	//----------------------------------------------------------------------------------------

	bool MapManager::loadMap(const std::string &mapName)
	{
		unloadMap();

		ObjectManager* objectMgr = ObjectManager::getSingletonPtr();
		MapLoader loader(mapName, objectMgr);
		bool flag = loader.loadMap();

		if (flag)
		{
			mMapInfo = loader.getMapInfo();

			Object *mapObj= objectMgr->createStaticObject(mMapInfo.mName, mMapInfo.mMeshName);
			mapObj->getBodyEntity()->setQueryFlags(COLLSION_COLLISION);
			mapObj->setDestructible(false);

			//std::ofstream out;
			//out.open("map.txt");
			//for (int i = 0; i < mMapInfo.mGridZ; i++)
			//{
			//	for (int j = 0; j < mMapInfo.mGridX; j++)
			//		out << mMapInfo.mValues[i][j]<<"  ";
			//	out << std::endl;
			//}
			//out.close();
		}

		return flag;
	}

	//----------------------------------------------------------------------------------------

	// unload map
	void MapManager::unloadMap()
	{
		ObjectManager::getSingletonPtr()->destroyStaticObject(mMapInfo.mName);
	}

	//----------------------------------------------------------------------------------------

	bool MapManager::exsitInMap(int row, int col)
	{
		if ((row < 0 || row >= mMapInfo.mGridZ) || (col < 0 || col >= mMapInfo.mGridX))
			return true;
		return mMapInfo.mValues[row][col] != 0;
	}

	//----------------------------------------------------------------------------------------

	Ogre::Vector3 MapManager::to3DMap(float row, float col, float height)
	{
		float blockSize = mMapInfo.mBlockSize;

		GameApp::log("2D  : "  + Util::toString(row) + " " + Util::toString(col));
		// Calculate the location of the center of the grid
		row = row * blockSize + blockSize / 2;
		col = col * blockSize + blockSize / 2;

		//Coordinate recovery
		row = row - getMapMaxZ();
		col = col - getMapMaxX();
		GameApp::log("3D : "  + Util::toString(row) + " " + Util::toString(col));
		
		return Ogre::Vector3(col, height, row);
	}

	//----------------------------------------------------------------------------------------

	bool MapManager::exsitInMap(const Ogre::Vector3&pos)
	{
		Ogre::Vector2 rePos = to2DMap(pos);
		return exsitInMap(rePos.x, rePos.y);
	}

	//----------------------------------------------------------------------------------------

	Ogre::Vector2 MapManager::to2DMap(const Ogre::Vector3 &pos)
	{
		// To convert the coordinates (0,0) is the minimum point coordinates
		float x = pos.x + getMapMaxX();
		float z = pos.z + getMapMaxZ();

		float blockSize = mMapInfo.mBlockSize;

		// Calculation of the 2D map grid location, where the use floor
		// Because when x is -0.1 when using int conversion may be 0, but should be -1
		x = std::floor(x / blockSize);
		z = std::floor(z / blockSize);

		return Ogre::Vector2(z, x);
	}

	//----------------------------------------------------------------------------------------

	Ogre::Vector3 MapManager::revisePosition(const Ogre::Vector3 &pos)
	{
		// To convert the coordinates (0,0) is the minimum point coordinates
		float x = pos.x + getMapMaxX();
		float z = pos.z + getMapMaxZ();

		float blockSize = mMapInfo.mBlockSize;

		// Calculation of the 2D map grid location, where the use floor
		// Because when x is -0.1 when using int conversion may be 0, but should be -1
		x = std::floor(x / blockSize);
		z = std::floor(z / blockSize);

		// Calculate the location of the center of the grid
		x = x * blockSize + blockSize / 2;
		z = z * blockSize + blockSize / 2;

		// Coordinate recovery
		x = x - getMapMaxX();
		z = z - getMapMaxZ();

		return Ogre::Vector3(x, pos.y, z);
	}

	//----------------------------------------------------------------------------------------

	void MapManager::removeFromMap(const std::string &objectName, int mapValue)
	{
		std::map<std::string, Ogre::Vector2>::iterator it = mMapInfo.mMapIndex.find(objectName);

		// If present, a first position on the model reduction -1 is used herein
		//it is possible to have more than one object in the model
		// Inside the same block, for example, 1/4 wall
		if (it != mMapInfo.mMapIndex.end())
		{
			Ogre::Vector2 pos = it->second;
			mMapInfo.mValues[(int)pos.x][(int)pos.y] -= mapValue;
			mMapInfo.mMapIndex.erase(objectName);
		}		
	}

	//----------------------------------------------------------------------------------------
}
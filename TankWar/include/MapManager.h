/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   MapManager.h
				created:    6/10/2012
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include "SingletonT.h"
#include "Object.h"
#include <vector>
#include <string>

namespace Tank
{
	// Game type
	enum GAME_MODE_TYPE
	{
		GAME_MODE_ONE			= 0,				// Singler game
		GAME_MODE_TWO			= 1,				// Pair game ---Not achieved
		GAME_MODE_MULTIPLE	= 2,				// Multiplayer mode  ---Not achieved
	};

	// Gets a string
	std::string getGameModeTypeStr(GAME_MODE_TYPE modeType);
	GAME_MODE_TYPE getGameModeType(const std::string &gameMode);

	// Map preview information
	struct MapPre
	{
		std::string fileName;	// File Name
		std::string name;			// Map Name
		std::string image;		// Map preImage
		int				enemyNumber;	// enemyNumber
		int				maxEnemyInScene;	// The max number of enemy
	};

	// Terrain info
	struct MapInfo
	{
		std::string mName;			// Map Name
		std::string mMeshName;	// Terrain Name

		std::string mGameMode;	// mode

		int				mGridX;						// x
		int				mGridZ;						// z
		int				mBlockSize;				// size of grip

		std::vector<std::vector<int> >					mValues;						// 2Dmap
		std::map<std::string, Ogre::Vector2>		mMapIndex;			// 2Dmap index

		std::vector<Ogre::Vector3>			mBirthPoint;					// origin of player
		std::vector<Ogre::Vector3>			mEnemyBirthPoint;	// origin of enemy
	};

	/**
		Map Manager is used to manage the entire game map of the scene
		such as loading maps, unloading map, traverse maps
		
		Managing a 2D map, 3D scene map will be mapped onto 2D matrix
	*/
	class MapManager : public SingletonT<MapManager>
	{
		SINGLETON_DEFINE(MapManager)

	public:
		
		MapInfo& getMapInfo() { return mMapInfo; }

	
		inline int getEnemyPointSize() { return (int)mMapInfo.mEnemyBirthPoint.size(); }
		inline int getPlayerPointSize() { return (int)mMapInfo.mBirthPoint.size(); }

		Ogre::Vector3 getStartPosition(TANK_TYPE tank, int index);

		// Get the preview map information under the specified index, you need to determine the legality of their own
		MapPre getMapPreByIndex(int index) { return mMaps[index]; }
		
		int getMapPreSize() const { return (int)mMaps.size(); }

		// query 2D x, z coordinates whether there are obstacles
		// true that there is an obstacle, false obstacle does not exist
		bool exsitInMap(int row, int clo);
		// query 3Dcoordinates whether there are obstacles
		// true that there is an obstacle, false obstacle does not exist
		bool exsitInMap(const Ogre::Vector3&pos);

		// Remove the spcified namepoint 
		void removeFromMap(const std::string &objectName, int mapValue);

		// The specified 3D coordinates into 2D matrix in the location x, y
		Ogre::Vector2 to2DMap(const Ogre::Vector3 &pos);
		Ogre::Vector3 to3DMap(float row, float col, float height);

		// Corrected, amended to correspond to the 3D world pos in the center of the box
		Ogre::Vector3 revisePosition(const Ogre::Vector3 &pos);
		
		inline Ogre::Vector3 getBlockSize() const 
		{ 
			return Ogre::Vector3(mMapInfo.mBlockSize, mMapInfo.mBlockSize,mMapInfo.mBlockSize);
		}

		/**
			(Min X,Z)------------
							|					 |
							|		(0,0)	 |
							|					 |
							------------(Max X,Z)
		*/
	
		inline float	getMapMaxX() { return mMapInfo.mGridX * mMapInfo.mBlockSize / 2; }
	
		inline float	getMapMinX() { return -mMapInfo.mGridX * mMapInfo.mBlockSize / 2; }

		inline float	getMapMaxZ() { return mMapInfo.mGridZ * mMapInfo.mBlockSize / 2; }

		inline float	getMapMinZ() { return -mMapInfo.mGridZ * mMapInfo.mBlockSize / 2; }

		bool loadMap(const std::string &mapName);

		void unloadMap();

	private:
		MapManager();

	private:
		MapInfo															mMapInfo;

		std::vector<MapPre>									mMaps;
	};
}
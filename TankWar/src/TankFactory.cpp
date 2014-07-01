/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   TankFactory.cpp
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "GreenTank.h"
#include "BlueTank.h"
#include "TankFactory.h"
#include "ObjectManager.h"
#include "MapManager.h"
#include "ContextApp.h"

namespace Tank
{

	std::vector<std::string> TANK_NAME;



	TankFactory::TankFactory(ObjectManager* objectMgr) : mObjectManager(objectMgr), mTankID(1)
	{
		mTanksConfig = new Config(TANKWAR_RES_ROOT_PATH + "models/Tanks/Tanks.cfg");

		for (int i = 1; i <= mTanksConfig->getFloat("TANKS", "SIZE"); i++)
			TANK_NAME.push_back("Tank" + Util::toString(i));
	}



	TankFactory::~TankFactory()
	{
		SAFE_DELETE(mTanksConfig);
		clear();
	}



	void TankFactory::clear()
	{
		for (TankIterator itr = mAITanks.begin(); itr != mAITanks.end(); ++itr)
			SAFE_DELETE(*itr);

		for (TankIterator itr = mPlayerTanks.begin(); itr != mPlayerTanks.end(); ++itr)
			SAFE_DELETE(*itr);

		mAITanks.clear();
		mPlayerTanks.clear();
		mTankIndex.clear();
	}



	BaseTank* TankFactory::create(TANK_TYPE type, const std::string &name, const std::string &meshName, 
																		const Ogre::Vector3 &pos, float heightOffset, float fireHeightOffset)
	{
		if (type ==  TANK_GREEN)
			return create(type, name, meshName, pos, PLAYER_INIT_SPEED, PLAYER_INIT_POWER, heightOffset, fireHeightOffset);
		else
			return create(type, name, meshName, pos, AI_INIT_SPEED, AI_INIT_POWER, heightOffset, fireHeightOffset);
	}



	BaseTank* TankFactory::create(TANK_TYPE type, const std::string &name, const std::string &meshName, 
																const Ogre::Vector3 &pos, float speed, float damage, float heightOffset, float fireHeightOffset)
	{
		BaseTank* tank = NULL;
		TankIterator itr;

		switch(type)
		{
		case TANK_GREEN:
			{
				tank = new GreenTank(name, meshName, pos, speed, damage, heightOffset, fireHeightOffset);
				itr = mPlayerTanks.insert(mPlayerTanks.end(), tank);
			}
			break;
		case TANK_BLUE:
			{
				tank = new BlueTank(name, meshName, pos, speed, damage, heightOffset, fireHeightOffset);
				itr = mAITanks.insert(mAITanks.end(), tank);

				ContextApp::getMiniMap()->addPoint(name);
			}
			break;
		}

		mTankIndex.insert(std::make_pair(tank->mFireEntity->getName(), itr));
		mTankIndex.insert(std::make_pair(tank->mBodyEntity->getName(), itr));
		mTankIndex.insert(std::make_pair(tank->getName(), itr));

		return tank;
	}




	void TankFactory::update( float timeSinceLastFrame ) 
	{
		updatePlayers(timeSinceLastFrame);
		updateAIs(timeSinceLastFrame);

		generateBlueTanks(timeSinceLastFrame);
	}



	void TankFactory::updateAIs(float timeSinceLastFrame)
	{
		BaseTank *tank = 0;
		int size = mAITanks.size();

		for (TankIterator itr = mAITanks.begin(); itr != mAITanks.end();)
		{
			tank = *itr;
			tank->update(timeSinceLastFrame);
	
			//ContextApp::getMiniMap()->updatePoint(tank->getName(), tank->getPosition());

		
			if (!tank->isAlive())
			{
				mTankIndex.erase(tank->getName());
				mTankIndex.erase(tank->mFireEntity->getName());
				mTankIndex.erase(tank->mBodyEntity->getName());
				
				//ContextApp::getMiniMap()->removePoint(tank->getName());
				ContextApp::destroyOneEnemy();
				ContextApp::updateScores();

				SAFE_DELETE(tank);
				mAITanks.erase(itr++);
			}
			else
				++itr;
		}
	}



	void TankFactory::generateBlueTanks(float timeSinceLastFrame)
	{
		static MapManager* mapMgr = MapManager::getSingletonPtr();
		static int point = 0;
		static float flushTime = 500;

		flushTime += timeSinceLastFrame * 0.05f;
		if (ContextApp::isEnemyDieOut() || (!ContextApp::canBirthEnemy())) return;

		
		int size = ContextApp::getMaxEnemyNumberInScene();
	
		int pointSize = (int)mapMgr->getEnemyPointSize();

		int tankSize = (int)mAITanks.size();
	
		if (flushTime > 500 && tankSize < size)
		{
			flushTime = 0;

			for (int i = 0; i < size - tankSize; i++)
			{
				int tankType = (int)Util::randomRange(0, mTanksConfig->getFloat("TANKS", "SIZE"));

				float life = mTanksConfig->getFloat(TANK_NAME[tankType], "LIFE");
				float defense = mTanksConfig->getFloat(TANK_NAME[tankType], "DEFENSE");

				float speed = mTanksConfig->getFloat(TANK_NAME[tankType], "SPEED");
				float damage = mTanksConfig->getFloat(TANK_NAME[tankType], "DAMAGE");

				float heightOffset = mTanksConfig->getFloat(TANK_NAME[tankType], "HEIGHTOFFSET");
				float fireHeightOffset = mTanksConfig->getFloat(TANK_NAME[tankType], "FIREHEIGHTOFFSET");

				//! debug only
				BaseTank* tank = create(TANK_BLUE, "AI_" + Util::toString(getNextTankID()), TANK_NAME[tankType]
				, mapMgr->getStartPosition(TANK_BLUE, point),speed, damage, heightOffset, fireHeightOffset);

				tank->setDefenseValue(defense);
				tank->setLifeValue(life);
				// 轮到下一个出生点
				point = (point + 1) % pointSize;
			}
		}
	}



	void TankFactory::updatePlayers(float timeSinceLastFrame)
	{
		for (TankIterator itr = mPlayerTanks.begin(); itr != mPlayerTanks.end(); ++itr)
		{
			(*itr)->update(timeSinceLastFrame);
		}
	}



	
	Object* TankFactory::get(const std::string &name)
	{
		if (mTankIndex.count(name) > 0)
			return (*mTankIndex[name]);

		return NULL;
	}




	bool TankFactory::destroy(const std::string &name)
	{
		if (mTankIndex.count(name) > 0)
		{
			(*mTankIndex[name])->destory();
			return true;
		}

		return false;
	}


}

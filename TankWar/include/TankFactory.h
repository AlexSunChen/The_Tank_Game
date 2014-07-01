/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   TankFactory.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include "Config.h"
#include "IFactory.h"
#include "BaseTank.h"
#include <list>
#include <map>

namespace Tank
{

	const static float PLAYER_INIT_SPEED = 5.0f;
	
	const static float PLAYER_INIT_POWER = 15.0f;

	const static float AI_INIT_SPEED = 1.25f;

	const static float AI_INIT_POWER = 3.0f;

	class ObjectManager;
	class TankFactory : public IFactory
	{
		typedef std::list<BaseTank*>::iterator TankIterator;
		typedef std::map<std::string, TankIterator>::iterator TankIndexIterator;

	public:
		~TankFactory();

		BaseTank* create(TANK_TYPE type, const std::string &name, const std::string &meshName, 
											const Ogre::Vector3 &pos, float heightOffset, float fireHeightOffset);

		BaseTank* create(TANK_TYPE type, const std::string &name, const std::string &meshName, 
											const Ogre::Vector3 &pos, float speed, float damage, float heightOffset, float fireHeightOffset);


		virtual void update( float timeSinceLastFrame ) ;

		virtual Object* get(const std::string &name);

		
		virtual bool destroy(const std::string &name);

		void clear();

	protected:
		friend class ObjectManager;
		TankFactory(ObjectManager* objectMgr);


		inline int getNextTankID() { return mTankID++; }


		void updateAIs(float timeSinceLastFrame);

		void updatePlayers(float timeSinceLastFrame);

		void generateBlueTanks(float timeSinceLastFrame);

	protected:
		ObjectManager												*mObjectManager;

		Config																*mTanksConfig;
		int																		mTankID;				

		std::list<BaseTank*>										mPlayerTanks;
		std::list<BaseTank*>										mAITanks;

		std::map<std::string, TankIterator>			mTankIndex;		
	};
}
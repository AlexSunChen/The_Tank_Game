/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   BlueTank.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/
#pragma once

#include "BaseTank.h"

namespace Tank
{

	class TankFactory;
	class BlueTank : public BaseTank
	{
	public:
		~BlueTank();
		void update(float timeSinceLastFrame);

		void fire();

		bool onHit(Attacker *attacker);

	protected:
		friend class TankFactory;
		
		BlueTank(const std::string &name, const std::string &meshName, 
								const Ogre::Vector3 &pos = Ogre::Vector3::ZERO, float speed = 1, 
								float damage = 5, float heightOffset = 0.3f, float fireHeightOffset = 1.0f);

		void idleRoateHead();
	
		void searchEnemy();
		void updateMove(float timeSinceLastFrame);

		void nextPosition();

	protected:
		Ogre::ParticleSystem*	mExplosionParticle;
		Ogre::SceneNode			*mExplosionNode;		

		bool									mMoveUp;		
		Ogre::Vector3					mGoal;					

		bool									mFindEnemy;		
	};
}
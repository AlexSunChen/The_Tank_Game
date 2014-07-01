/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   BaseTank.h
			created:    6/29/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#pragma once

#include <Ogre.h>
#include <string>
#include "Object.h"
#include "BaseBullet.h"

namespace Tank
{
	const static int BODY_ROTATE_DEGREE = 15;

	enum TANK_TYPE
	{
		TANK_GREEN,		// player
		TANK_BLUE,			// enemy
	};

	const static Ogre::Vector3 MOVE_UP(0, 0, -1);
	const static Ogre::Vector3 MOVE_DOWN(0, 0, 1);
	const static Ogre::Vector3 MOVE_LEFT(-1, 0, 0);
	const static Ogre::Vector3 MOVE_RIGHT(1, 0, 0);

	class ChaseCamera;
	class TankFactory;
	class BaseTank : public Object, public Attacker
	{
		friend class ChaseCamera;
		friend class TankFactory;
	public:
		
		BaseTank(TANK_TYPE tankID, const std::string &name,	const std::string &meshName, 
			const Ogre::Vector3 &pos = Ogre::Vector3::ZERO, float damage = 5, 
			float heightOffset = 1, float fireHeightOffset = 1);

		~BaseTank();

		
		inline Ogre::Vector3 getFireDirection() 
		{
			Ogre::Vector3 fireFront = mFireNode->_getDerivedOrientation().zAxis() ;
			
			if (!mFrontZ) fireFront*= -1.0f;


			return fireFront;
		
			//return mLookAt->_getDerivedPosition() - mFireNode->_getDerivedPosition(); 
			//return mFireNode->_getDerivedOrientation().zAxis(); 
		}

		
		inline Ogre::Vector3 getFirePosition() { return mFireNode->_getDerivedPosition() + mFireHeightOffset; }

		void setPosition(const Ogre::Vector3 &pos);

	
		virtual void fire(){}
		virtual void update(float timeSinceLastFrame){}

	
		void addHealth(float health) ;

		inline int getCurrentLifeValue() const { return mLife - mCurrentLife;}

		virtual void setVisible(bool visible);


		TANK_TYPE getTankID() const{ return mTankID; }

		BULLET_TYPE	getBulletType() const { return mBulletType; }
		void setBulletType(BULLET_TYPE bulletType) { mBulletType = bulletType; }

		Ogre::Vector3 getBodyDirection() const { return mMove; }

	protected:

		virtual void updateDirection();
	
		bool updateBodyDirection();

	protected:
		Ogre::Entity *mFireEntity;

		Ogre::SceneNode *mFireNode;				

		//Ogre::SceneNode *mLookAt;				

		Ogre::ParticleSystem* mSmokeParticle;
	
		Ogre::SceneNode * mSmokeNode;
	
		Ogre::Vector3 mFireHeightOffset;
	
		Ogre::Vector3 mMove; 			


		Ogre::Vector3 mPreMove;			
		float mSpeed;


		float mFireCD;
	
		Ogre::Degree mRoateDegree;		
	
		Ogre::Degree mCurrentDegree;		
	
		int  mRotateBody;		

		TANK_TYPE 	mTankID;

		BULLET_TYPE mBulletType;
	};
}
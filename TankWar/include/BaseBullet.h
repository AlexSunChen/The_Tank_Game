/*
	-----------------------------------------------------------------------------
    ***********************************************************************
			filename:   BaseBullet.h
			created:    6/28/2014
			author:     Sun Chen
    ************************************************************************
    ************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include <Ogre.h>
#include <string>
#include "Object.h"

namespace Tank
{

	enum BULLET_TYPE
	{
		BULLET_NORMAL	= 0,		// normal bullet
		BULLET_ROCKET		= 1,		// rocket bullet
		BULLET_IMPULSE	= 2,		//impulse
		BULLET_FIRE				= 3,		// Fire
		BULLET_LIGHTING	= 4,		// Lighting
	};

	/**
		Bullet base class, implements the basic framework of the bullet
		the bullet if there is a new type, you need to be inheritedBullet base class
		implements the basic framework of the bullet, the bullet if there is a new type, you need to be inherited
		Reaction of different types of bullets after the collision particle effects and different
		and to achieve this kind of inheritance
	*/
	class BaseTank;
	class BulletFactory;
	class BaseBullet : public Object
	{
		friend class BulletFactory;
	public:
		virtual ~BaseBullet(){}

		inline BaseTank* getOnwer(){ return mOwner; }

		inline float getPower() const { return mPower; }
	
		inline float getSpeed() const { return mSpeed; }

	
		inline Ogre::Vector3 getDirection() const { return mDirection; }
	
		inline Ogre::Vector3 getStartPosition() const { return mStartPosition; }

		void update(float timeSinceLastFrame);
		bool onHit(Attacker *attacker);

	protected:
		/**
			@ID					
			@objectFactroy	Creating this object Object Manager
			@owner				
			@direction			
			@startPosition	
			@speed					
			@power				
			@meshName		
			@lifeTime				
		*/
		BaseBullet(int ID, BaseTank* owner, const Ogre::Vector3 &direction, 
			const Ogre::Vector3 &startPosition, float speed, float power, const std::string &meshName, float lifeTime = 40);

		// Detect whether a collision with other objects bullet
		// If happend a collision with other objects returns true, otherwise it returns false
		virtual bool checkCollision();
		// After the end of the life cycle of a bullet or collision with other objects, the display is updated explosions
		//When the bullets explode represents execution of its life cycle has ended, and stop moving,
		// Need to be set after the explosion mMove = false
		virtual void updateExplosion(){}

	protected:
		float									mPower;							// power

		bool									mMove;								// if can move
		float									mSpeed;							// speed

		Ogre::Vector3					mDirection;						// direction
		Ogre::Vector3					mStartPosition;				// poisiton

		BaseTank							*mOwner;							// owner
	};
}
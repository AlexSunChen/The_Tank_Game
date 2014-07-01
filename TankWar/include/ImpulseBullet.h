/*
	-----------------------------------------------------------------------------
    ***********************************************************************
			filename:   ImpulseBullet.h
			created:    6/28/2014
			author:     Sun Chen
    ************************************************************************
    ************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include "BaseBullet.h"

namespace Tank
{
	// Model Name pulse bullets
	const static std::string BULLET_IMPULSE_MESH_MANE = "BulletBall.mesh";

	// The initial velocity pulse bomb
	const static float BULLET_IMPULSE_SPEED = 2.5f;

	// The initial pulse bomb attack
	const static float BULLET_IMPULSE_POWER = 10.0f;

	
	class BulletFactory;
	class ImpulseBullet : public BaseBullet
	{
	public:
		~ImpulseBullet();

		bool checkCollision();

	protected:
		friend class BulletFactory;

		/**
			@ID						
			@objectFactroy	
			@owner				
			@direction			
			@startPosition	
			@speed					
			@power				
			@lifeTime				
		*/
		ImpulseBullet(int ID, BaseTank* owner, const Ogre::Vector3 &direction, 
									const Ogre::Vector3 &startPosition, float lifeTime = 40);

		// After the end of the life cycle of a bullet or collision with other objects, the display is updated explosions
	   //When the bullets explode represents execution of its life cycle has ended, and stop moving,
	   // Need to be set after the explosion mMove = false
		virtual void updateExplosion();

		void createChildBullet(const Ogre::Vector3 &normal, const Ogre::Degree &degree);

	protected:
		Ogre::ParticleSystem		*mParticle;							
		Ogre::ParticleSystem		*mExplosionParticle;				
		Ogre::SceneNode			*mParticleNode;
		Ogre::SceneNode			*mExplosionParticleNode;

		bool									mSpawn;									
	};
}
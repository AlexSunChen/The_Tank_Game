/*
	-----------------------------------------------------------------------------
    ***********************************************************************
			filename:   RocketBullet.h
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
	const static std::string BULLET_ROCKET_MESH_MANE = "RocketBullet.mesh";


	const static float BULLET_ROCKET_SPEED = 2.0f;


	const static float BULLET_ROCKET_POWER = 15.0f;

	/**
	
	*/
	class BulletFactory;
	class RocketBullet : public BaseBullet
	{
	public:
		~RocketBullet();

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
		RocketBullet(int ID, BaseTank* owner, const Ogre::Vector3 &direction, 
								const Ogre::Vector3 &startPosition, float lifeTime = 40);

		
		virtual void updateExplosion();

	protected:
		Ogre::ParticleSystem		*mParticle;					
		Ogre::ParticleSystem		*mExplosionParticle;		
		Ogre::SceneNode			*mParticleNode;
		Ogre::SceneNode			*mExplosionParticleNode;
	};
}
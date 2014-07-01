/*
	-----------------------------------------------------------------------------
    ***********************************************************************
			filename:   RocketBullet.cpp
			created:    6/28/2014
			author:     Sun Chen
    ************************************************************************
    ************************************************************************
	-----------------------------------------------------------------------------
*/

#include "BaseTank.h"
#include "RocketBullet.h"
#include "AudioManager.h"

namespace Tank
{


	RocketBullet::RocketBullet(int ID, BaseTank* owner, const Ogre::Vector3 &direction, const Ogre::Vector3 &startPosition , float lifeTime)
		: BaseBullet(ID, owner, direction, startPosition, BULLET_ROCKET_SPEED, BULLET_ROCKET_POWER, BULLET_ROCKET_MESH_MANE, lifeTime)
	{
		// Get the rotation angle of the missile, the missile positive direction - negative Z-axis, and the direction of the missile launch
		Ogre::Degree rotate = mDirection.angleBetween(Ogre::Vector3::NEGATIVE_UNIT_Z);
		//Get the angle between the positive and negative
		if (mDirection.crossProduct(Ogre::Vector3::NEGATIVE_UNIT_Z).y > 0)
			mBodyNode->yaw(-rotate);
		else
			mBodyNode->yaw(rotate);

		
		mParticle = mSceneManager->createParticleSystem(mName + "Robot", "TankWar/Rocket");
		mParticleNode = mBodyNode->createChildSceneNode();
		mParticleNode->attachObject(mParticle);


		mExplosionParticle = mSceneManager->createParticleSystem(mName + "Exposion", "TankWar/BulletRocketExplosion");
		mExplosionParticleNode = mBodyNode->createChildSceneNode();

		if (owner->getTankID() == TANK_GREEN)
			AudioManager::play(AUDIO_FIRE, 0.75f, false, false);
		else
			AudioManager::play(AUDIO_FIRE, 0.1f, false, false);
	}



	RocketBullet::~RocketBullet()
	{
		mSceneManager->destroyParticleSystem(mParticle);
		mSceneManager->destroySceneNode(mParticleNode);

		mSceneManager->destroyParticleSystem(mExplosionParticle);
		mSceneManager->destroySceneNode(mExplosionParticleNode);
	}



	
	void RocketBullet::updateExplosion()
	{
		if (mMove)
		{
			if (mOwner->getTankID() == TANK_GREEN)
				AudioManager::play(AUDIO_EXPLOSION, 0.75f);
			else
				AudioManager::play(AUDIO_EXPLOSION, 0.25f);

			mParticleNode->setVisible(false);
			mBodyNode->setVisible(false);
			mExplosionParticleNode->attachObject(mExplosionParticle);

		
			mMove = false;
		}
	}


}
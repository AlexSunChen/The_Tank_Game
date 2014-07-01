/*
	-----------------------------------------------------------------------------
    ***********************************************************************
			filename:   ImpulseBullet.cpp
			created:    6/28/2014
			author:     Sun Chen
    ************************************************************************
    ************************************************************************
	-----------------------------------------------------------------------------
*/

#include "ImpulseBullet.h"
#include "AudioManager.h"
#include "ObjectManager.h"

namespace Tank
{


	ImpulseBullet::ImpulseBullet(int ID, BaseTank* owner, const Ogre::Vector3 &direction, 
		const Ogre::Vector3 &startPosition, float lifeTime)
		: BaseBullet(ID, owner, direction, startPosition, 
								BULLET_IMPULSE_SPEED, BULLET_IMPULSE_POWER, 
								BULLET_IMPULSE_MESH_MANE, lifeTime), mSpawn(true)
	{
		// Tail of smoke particles missile
		mParticle = mSceneManager->createParticleSystem(mName + "Impulse", "TankWar/Impulse");
		mParticleNode = mBodyNode->createChildSceneNode();
		mParticleNode->attachObject(mParticle);

		// Missile explosion particles
		mExplosionParticle = mSceneManager->createParticleSystem(mName + "Exposion", 
																						"TankWar/BulletImpulseExplosion");
		mExplosionParticleNode = mBodyNode->createChildSceneNode();

		AudioManager::play(AUDIO_IMPULSE, 0.75f, false, false);
	}



	ImpulseBullet::~ImpulseBullet()
	{
		mSceneManager->destroyParticleSystem(mParticle);
		mSceneManager->destroySceneNode(mParticleNode);

		mSceneManager->destroyParticleSystem(mExplosionParticle);
		mSceneManager->destroySceneNode(mExplosionParticleNode);
	}



	bool ImpulseBullet::checkCollision()
	{
		// Pulse bombs hit the target, divided into two
		if(BaseBullet::checkCollision() && mSpawn)
		{
			Ogre::Vector3 normal = -getDirection();

			// Save the current bullet type, bullet type to the current pulse bomb
			BULLET_TYPE bulletType = mOwner->getBulletType();
			mOwner->setBulletType(BULLET_IMPULSE);
			// Generating sub-pulse bomb
			createChildBullet(normal, Ogre::Degree(Util::randomRange(25, 75)));
			createChildBullet(normal, Ogre::Degree(-Util::randomRange(25, 75)));
			// Restore the previous bullet type
			mOwner->setBulletType(bulletType);

			return true;
		}

		return false;
	}



	// After the end of the life cycle of a bullet or collision with other objects, the display is updated explosions
	//When the bullets explode represents execution of its life cycle has ended, and stop moving,
	// Need to be set after the explosion mMove = false
	void ImpulseBullet::updateExplosion()
	{
		if (mMove)
		{
			AudioManager::play(AUDIO_IMPULSE, 0.75f);

			mParticleNode->setVisible(false);
			mBodyNode->setVisible(false);
			mExplosionParticleNode->attachObject(mExplosionParticle);

			
			mMove = false;
		}
	}



	void ImpulseBullet::createChildBullet(const Ogre::Vector3 &normal, const Ogre::Degree &degree)
	{
		static ObjectManager* objectMgr = ObjectManager::getSingletonPtr();

		// The normal vector degree horizontal rotation degree
		Ogre::Vector3 dir = Ogre::Quaternion(degree, Ogre::Vector3::UNIT_Y) * normal;
		Ogre::Vector3 pos = getPosition();

		ImpulseBullet* bullet = static_cast<ImpulseBullet*>(objectMgr->createBullet(mOwner, dir, pos));
		// Set the child no longer pulse bomb exploded, and the sub-pulse bomb speed reduction
		bullet->mSpawn = false;
		bullet->mSpeed = bullet->mSpeed * 0.6f;
	}


}
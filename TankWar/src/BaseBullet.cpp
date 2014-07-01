/*
	-----------------------------------------------------------------------------
    ***********************************************************************
			filename:   BaseBullet.cpp
			created:    6/28/2014
			author:     Sun Chen
    ************************************************************************
    ************************************************************************
	-----------------------------------------------------------------------------
*/

#include "BaseBullet.h"
#include "BaseTank.h"
#include "Util.h"
#include "BulletFactory.h"
#include "ObjectManager.h"
#include "GameApp.h"
#include "AudioManager.h"
#include "ContextApp.h"

namespace Tank
{


	BaseBullet::BaseBullet(int ID, BaseTank* owner, const Ogre::Vector3 &direction, const Ogre::Vector3 &startPosition
			, float speed, float power, const std::string &meshName, float lifeTime) 
	: mOwner(owner), mDirection(direction), mStartPosition(startPosition), mMove(true), mSpeed(speed), mPower(power)
	{
		mID = ID;
		mLife = lifeTime;
		mName = mOwner->getName() + Util::toString(ID);

		mSceneManager = mOwner->getSceneManager();
		mBodyEntity = mSceneManager->createEntity(mName, meshName);
		mBodyNode = mSceneManager->getRootSceneNode()->createChildSceneNode(startPosition);
		mBodyNode->attachObject(mBodyEntity);


		mBodyEntity->setQueryFlags(COLLSION_BULLET);

		mDirection.normalise();
	}



	bool BaseBullet::onHit(Attacker *attacker)
	{

		destory();


		updateExplosion();

		return true;
	}



	bool BaseBullet::checkCollision()
	{
		static ObjectManager* objectMgr = ObjectManager::getSingletonPtr();
		static int COLLISION_MASK = COLLSION_COLLISION | COLLSION_BULLET;

		if (mMove)
		{
			Ogre::Vector3 size = mBodyEntity->getBoundingBox().getHalfSize() ;
			// Calculate the offset direction on both sides of the flight in accordance with the direction of the bullet flying
			Ogre::Vector3 offset = getDirection().crossProduct(Ogre::Vector3::UNIT_Y) * size;

			Ogre::Vector3 endPos = getPosition() + getDirection();
			// Here the use of triple detection, namely bullets left and right sides and the middle of the model to detect collisions
			Object *object = objectMgr->collision(getPosition(), endPos, 2.0f, COLLISION_MASK);
			if (!object)	 object = objectMgr->collision(getPosition() + offset, endPos + offset, 2.0f, COLLISION_MASK);
			if (!object) object = objectMgr->collision(getPosition() - offset, endPos - offset, 2.0f, COLLISION_MASK);
			
			// Missiles have been bumping into objects, and not their own
			if (object && object != this && object != mOwner)
			{
				// Collision object, then display explosion
				updateExplosion();

				// If you hit the target && is the player who hit the tank, then update the UI scores
				if (object->onHit(mOwner) && mOwner->getTankID() == TANK_GREEN)
					ContextApp::updateScores(mOwner);

				return true;
			}
		}

		return false;
	}



	void BaseBullet::update(float timeSinceLastFrame)
	{
		if (mMove)
		{
			// Moves in the direction of the transmitter
			mBodyNode->translate(mDirection * timeSinceLastFrame * mSpeed);

			// Check whether objects collide and crash
			checkCollision();
			
			// If the current bullets life> 80% bullets can survive life, then converted to an explosion mode
			if (mCurrentLife > mLife * 0.8f)
				updateExplosion();
		}

		mCurrentLife += timeSinceLastFrame;
	}


}
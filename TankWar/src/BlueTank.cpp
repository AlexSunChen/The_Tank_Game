/*
	----------------------------------------------------------------------------
	***********************************************************************
				filename:   BlueTank.cpp
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "BlueTank.h"
#include "ContextApp.h"
#include "ObjectManager.h"
#include "MapManager.h"
#include "GameApp.h"

namespace Tank
{
	const Ogre::Vector3 DIRECTION[4] = { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT};

	const static float MAX_RADIUS_POWER_2 = 800.0f;

	const static float IDLE_ROTATE_DEGREE = 0.5f;



	BlueTank::BlueTank(const std::string &name, const std::string &meshName, 
		const Ogre::Vector3 &pos, float speed, float damage, float heightOffset, float fireHeightOffset)
		: BaseTank(TANK_BLUE, name, meshName, pos, damage, heightOffset, fireHeightOffset), 
		mFindEnemy(false), mExplosionNode(0), mExplosionParticle(0)
	{
		mSpeed = speed;
		mMove = MOVE_DOWN;
		
		mPreMove = mMove;


		mBodyEntity->setQueryFlags(COLLSION_PASS);
		mFireEntity->setQueryFlags(COLLSION_COLLISION);

	
		mFireNode->setFixedYawAxis(true);

		mFrontZ = false;

	}	



	BlueTank::~BlueTank()
	{
		if (mExplosionParticle)
		{
			mSceneManager->destroyParticleSystem(mExplosionParticle);
			mSceneManager->destroySceneNode(mExplosionNode);
		}
	}



	void BlueTank::update(float timeSinceLastFrame)
	{
		timeSinceLastFrame *= 0.01f;
		mFireCD += timeSinceLastFrame;

		if (mDeath)
			mCurrentLife += timeSinceLastFrame;
		else
		{
			
			updateMove(timeSinceLastFrame);

			updateBodyDirection();
		
			searchEnemy();
			idleRoateHead();

			fire();
		}
	}



	void BlueTank::updateMove(float timeSinceLastFrame)
	{
		MapManager* mapMgr = MapManager::getSingletonPtr();
		static float flushTime = 0;

		if (mRotateBody != 0) return;

		Ogre::Vector3 pos = getPosition() + mMove * mapMgr->getBlockSize() / 2;
		if (!mapMgr->exsitInMap(pos))
		{
			flushTime += timeSinceLastFrame;

		
			Ogre::Vector3 point = mapMgr->revisePosition(getPosition()) - getPosition();
			if (point.squaredLength() < 0.1f  && flushTime > 200)
			{
			
				int turnLeft = (int)Util::randomRange(0,9);
				if (turnLeft % 2 == 0)
				{
					mMove = Util::randomBool() ? MOVE_RIGHT : MOVE_LEFT;
					updateDirection();
					flushTime = 0;
				}
			}

			if (mRotateBody == 0)
			{
				
				if (mFindEnemy) timeSinceLastFrame *= 0.1f;

				Ogre::Vector3 move = mSpeed * mMove * timeSinceLastFrame;
				
				mBodyNode->translate(move);
				mFireNode->translate(move);
			}
		}
		else
				nextPosition();
	}



	void BlueTank::nextPosition()
	{
		static int dir = 1;
		static int preDir = dir;

		int temp = (int)Util::randomRange(0, 4);
		int times = 0;
		while((temp == dir || temp == preDir) && times < 5)
		{
		
			temp = (int)Util::randomRange(2, 4);
			times++;
		}

		preDir = dir;
		dir = temp;

		mMove = DIRECTION[dir];
		updateDirection();
	}



	void BlueTank::idleRoateHead()
	{
		static float degree = 0;
		static int direction = 1;

		if (mFindEnemy) return;

		if (direction == 1)
		{
			if (degree < 180)
				degree += IDLE_ROTATE_DEGREE;
			else
				direction = -1;
		}
		else
		{
			if (degree > -180)
				degree -= IDLE_ROTATE_DEGREE;
			else
				direction = 1;
		}

		mFireNode->yaw(Ogre::Degree(direction));
	}



	void BlueTank::searchEnemy()
	{
		static ObjectManager* objectMgr = ObjectManager::getSingletonPtr();

		BaseTank* enemy =  ContextApp::getPlayerA();
		Ogre::Vector3 length = enemy->getPosition() - getPosition();
		Ogre::Vector3 offset = length.normalisedCopy();

		if (length.squaredLength() < MAX_RADIUS_POWER_2 && 
			!objectMgr->collision(getPosition(), enemy->getPosition(), 0.5f, COLLSION_COLLISION))
		{
			mFindEnemy = true;
			mFireNode->lookAt(enemy->getPosition(), Ogre::Node::TS_WORLD);

			if (mFrontZ)
			{
				mFireNode->yaw( Radian(Degree(180) ) );
			}
		}
		else
			mFindEnemy = false;
	}



	bool BlueTank::onHit(Attacker *attacker)
	{
		mCurrentLife += (attacker->getDamageValue() - mDefense) * 5;
		if (!mDamaged && mCurrentLife > mLife * 0.5f)
		{
			mSmokeNode->attachObject(mSmokeParticle);
			mDamaged = true;
		}

		if (!mDeath && mCurrentLife > mLife * 0.95f)
		{
			setVisible(false);
			mDeath = true;

		
			mCurrentLife = mLife - 6.5f;
			mSmokeNode->setVisible(false);


			mExplosionNode = mSceneManager->getRootSceneNode()->createChildSceneNode(getFirePosition());
			mExplosionParticle = mSceneManager->createParticleSystem(mName + "_ExplosionParticle", "TankWar/TankExplosion");
			mExplosionNode->attachObject(mExplosionParticle);


			Object* object = ObjectManager::getSingletonPtr()->createStaticObject(mName + "Bomb", "tiretrial");
			{
				Ogre::Vector3 size = getBoundingBox().getHalfSize() * 0.9f;
				object->setPosition(getPosition() - Ogre::Vector3(0, size.y, 0));
				object->setScale(Ogre::Vector3(100, 1, 100));
				object->getBodyEntity()->setCastShadows(false);
				object->getBodyEntity()->setMaterialName("Examples/Bomb");
				object->getBodyNode()->yaw(Ogre::Degree(Util::randomRange(0, 180)));
			}

			attacker->earnMoney(Util::randomRange(50, 100));
			return true;
		}	

		return false;
	}



	void BlueTank::fire()
	{
		if (!mDeath && mFireCD > 10.0f)	
		{
			if (!mFindEnemy)
			{
			
				int randomFire = (int)Util::randomRange(1, 10);
				if(randomFire % 3 != 0)
				{
					mFireCD = 0;
					return;
				}
			}
			ObjectManager::getSingletonPtr()->createBullet(this);
			mFireCD = 0;
		}
	}


}
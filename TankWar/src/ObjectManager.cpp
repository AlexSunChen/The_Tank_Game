/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   ObjectManager.cpp
			created:    5/29/2012
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "ObjectManager.h"
#include "GameApp.h"
#include "Util.h"
#include "AudioManager.h"
#include "StaticObject.h"
#include "ContextApp.h"

namespace Tank
{


	ObjectManager::ObjectManager()
	{
		mSceneMgr =  GameApp::getSingletonPtr()->getGameSceneMgr();
		mCollision = new MOC::CollisionTools(mSceneMgr);

		mRayQuery = mSceneMgr->createRayQuery(Ogre::Ray());

		mStaticObjectFactory = new StaticObjectFactory(this);
		mBulletFactory = new BulletFactory(this);
		mItemFactory = new ItemFactory(this);
		mTankFactory = new TankFactory(this);
	}



	ObjectManager::~ObjectManager()
	{
	}



	Object* ObjectManager::collision(const Ogre::Vector3& fromPoint, const Ogre::Vector3& toPoint, 
		float collisionRadius, int queryMask)
	{
			Ogre::Entity* entity = mCollision->collidesWithObject(fromPoint, toPoint, collisionRadius, queryMask);
		if (entity && entity->isVisible())
			return getObject(entity->getName());

		return 0;
	}



	Object* ObjectManager::collisionAABB(const Ogre::Vector3& fromPoint, const Ogre::Vector3& toPoint, int queryMask)
	{
		Ogre::Vector3 direction = toPoint - fromPoint;
		Ogre::Ray ray(fromPoint, direction);
		mRayQuery->setRay(ray);
		mRayQuery->setQueryMask(queryMask);
		// The rays are sorted by the distance query set, [very important]
		mRayQuery->setSortByDistance(true);

		Ogre::RaySceneQueryResult& result = mRayQuery->execute();
		Ogre::RaySceneQueryResult::iterator itr = result.begin();

		// Just get the nearest object
		if (itr != result.end() && itr->movable)
		{
			Ogre::MovableObject *object =  itr->movable;
			Ogre::Vector3 pos = object->getParentSceneNode()->getPosition();
			// If the current starting point of an object in the distance is less than a predetermined range
			// then return this obj
			// Avoid using the square root operation squaredLength
			if ((pos - fromPoint).squaredLength() <= direction.squaredLength())
				return getObject(itr->movable->getName());
		}

		return NULL;
	}



	Ogre::Vector3 ObjectManager::collisionFromScreen(const Ogre::Vector2& pos)
	{
		static Ogre::RenderWindow *renderWindow = GameApp::getSingletonPtr()->getRenderWindow();
		Ogre::Camera *camera = mSceneMgr->getCameraIterator().peekNextValue();

		Ogre::Vector3 result(0.0f);
		Ogre::MovableObject *node;
		float	ra = 5.0f;
		mCollision->raycastFromCamera(renderWindow, camera, pos, result, node, ra, COLLSION_COLLISION);

		return result;
	}



	Object* ObjectManager::getObject(const std::string &name)
	{
		Object *object = mStaticObjectFactory->get(name);
		if (!object) object = mTankFactory->get(name);
		if (!object) object = mBulletFactory->get(name);
		if (!object) object = mItemFactory->get(name);

		return object;
	}



	BaseTank* ObjectManager::createTank(TANK_TYPE type, const std::string &name, const std::string &meshName, 
		const Ogre::Vector3 &pos, float heightOffset, float fireHeightOffset)
	{
		return mTankFactory->create(type, name, meshName, pos, heightOffset, fireHeightOffset);
	}



	BaseBullet* ObjectManager::createBullet(BaseTank *tank)
	{
		return mBulletFactory->create(tank);
	}



	BaseBullet* ObjectManager::createBullet(BaseTank *tank, const Ogre::Vector3 &fireDirection, const Ogre::Vector3 &firePosition)
	{
		return mBulletFactory->create(tank, fireDirection, firePosition);
	}



	Item* ObjectManager::createItem(ITEM_ID itemID, const Ogre::Vector3 &pos)
	{
		return mItemFactory->create(itemID, pos);
	}


	Object* ObjectManager::createStaticObject(const std::string &name, const std::string &meshName)
	{
		return mStaticObjectFactory->create(name, meshName);
	}



	void ObjectManager::destroyStaticObject(const std::string &name)
	{
		mStaticObjectFactory->destroy(name);
	}



	void ObjectManager::clear()
	{
		mBulletFactory->clear();
		mItemFactory->clear();
		mStaticObjectFactory->clear();
		mTankFactory->clear();
	}



	void ObjectManager::release()
	{
		SAFE_DELETE(mRayQuery);
		SAFE_DELETE(mCollision);
		SAFE_DELETE(mBulletFactory);
		SAFE_DELETE(mItemFactory);
		SAFE_DELETE(mStaticObjectFactory);
		SAFE_DELETE(mTankFactory);
	}



	void ObjectManager::update(float timeSinceLastFrame)
	{
		
		mTankFactory->update(timeSinceLastFrame);
		
		mItemFactory->update(timeSinceLastFrame);
		
		mBulletFactory->update(timeSinceLastFrame);
	}


}
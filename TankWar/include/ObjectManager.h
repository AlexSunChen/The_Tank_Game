/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   ObjectManager.h
				created:    5/29/2012
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include <Ogre.h>
#include <vector>

#include "SingletonT.h"
#include "Common.h"
#include "CollisionTools.h"

#include "BaseTank.h"
#include "TankFactory.h"
#include "BulletFactory.h"
#include "ItemFactory.h"
#include "StaticObjectFactory.h"

namespace Tank
{
	// Collision Detection
	enum OBJECT_COLLSION_MASK
	{
		COLLSION_PASS					= 1 << 0,		// No inquiry
		COLLSION_COLLISION		= 1 << 1,		// Collision Detection
		COLLSION_BULLET				= 1 << 2,		// Bullet collision mask
		COLLSION_ITEM					= 1 << 3,		// Aids
	};

	/**
		Scene object manager for all objects within the scene creation and destruction.
	*/
	class GameApp;
	class MOC::CollisionTools;
	class ObjectManager : public SingletonT<ObjectManager>, public IFrameListener
	{
		SINGLETON_DEFINE(ObjectManager)

	public:
		~ObjectManager();

		void clear();

		// Get sceneManager
		Ogre::SceneManager *getSceneManager() { return mSceneMgr; }

		// Get collision Manager
		inline MOC::CollisionTools* getCollision() { return mCollision; }

		/** 
			Model was based on triangle collision queries
			check a given starting point and 
			ending point to see if can detect the special object(ie collision)
			@fromPoint				Starting point
			@toPoint			    ending point
			@collisionRadius	Query radius
			@queryMask			Query Mask	OBJECT_COLLSION_MASK
		*/
		Object* collision(const Ogre::Vector3& fromPoint, const Ogre::Vector3& toPoint, 
										float collisionRadius, int queryMask);
		
		/**
			AABB collision with inquiries
			@fromPoint		
			@toPoint			
			@queryMask		OBJECT_COLLSION_MASK
		*/
		Object* collisionAABB(const Ogre::Vector3& fromPoint, const Ogre::Vector3& toPoint, int queryMask);

		/** 
			According to the screen coordinates to queries the world coordinate point 
			@pos	
		*/
		Ogre::Vector3 collisionFromScreen(const Ogre::Vector2& pos);

		/**
			Create a player tank
			@name						Player name, globally unique
			@meshName			Tank Model Name
			@pos				Tanks initial position
			@heightOffset			Tank height offset
			@fireHeightOffset	Tanks fired height offsets
		*/
		BaseTank* createTank(TANK_TYPE type, const std::string &name, const std::string &meshName, 
													const Ogre::Vector3 &pos, float heightOffset, float fireHeightOffset);

		/**
			Create a Bullet
			@tank	Create a bullet owner
		*/
		BaseBullet* createBullet(BaseTank *tank);
		/**
			Create a Bullet
			@tank					Create a bullet owner
			@fireDirection		Fire direction
			@firePosition		Firing position
		*/
		BaseBullet* createBullet(BaseTank *tank, const Ogre::Vector3 &fireDirection, const Ogre::Vector3 &firePosition);
		
		/**
			Creating an article
			@itemID	
			@pos			Initialization coordinates
		*/
		Item* createItem(ITEM_ID itemID, const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);

		/**
			Creating an Object
			@name				
			@meshName	
		*/
		Object* createStaticObject(const std::string &name, const std::string &meshName);

		/**
			Destroy an Object
			@name	
		*/
		void destroyStaticObject(const std::string &name);

		/**
			Object get any object by name
			if the specified name exists Object, Object pointer is returned, otherwise returns NULL
			Object can be any object, including all inherited from Object to Object Bullet, Item, StaticObject etc.
			@ name
		*/
		Object* getObject(const std::string &name);

		// All objects perform updates
		void update(float timeSinceLastFrame);

		// Clear all objects in the Object Manager
		void release();

	private:
		ObjectManager();

	private:
		Ogre::RaySceneQuery						*mRayQuery;
		Ogre::SceneManager							*mSceneMgr;
		MOC::CollisionTools							*mCollision;

		StaticObjectFactory							*mStaticObjectFactory;		// Static goods factory
		BulletFactory										*mBulletFactory;					// Bullet Factory
		ItemFactory											*mItemFactory;					// goods Factory
		TankFactory											*mTankFactory;					// tanker factory
	};
}
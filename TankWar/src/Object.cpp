/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   Object.cpp
			created:    6/29/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "Object.h"
#include "GameApp.h"
#include "ObjectManager.h"

namespace Tank
{

	Object::Object() 
		: mSceneManager(0), mDestructible(true), mLife(0), mDefense(0), mDeath(false),mID(0),
		mCollision(true), mBodyEntity(0), mBodyNode(0), mCurrentLife(0), mDamaged(false), mObjectType(OBJECT_OBJECT)
	{

		mMoney = 0;

		mFrontZ = true;

	}

	Object::Object(Ogre::SceneManager* sceneMgr, const std::string &name, const std::string &meshName) 
		: mSceneManager(sceneMgr), mName(name), mMeshName(meshName), mCurrentLife(0), mID(0),
		mDamaged(false), mDeath(false),mObjectType(OBJECT_OBJECT)
	{
		mMoney = 0;

		mBodyEntity = mSceneManager->createEntity(mName, mMeshName);
		mBodyNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
		mBodyNode->attachObject(mBodyEntity);

		mFrontZ = true;

	}

	Object::~Object()
	{
		if (mSceneManager)
		{
			mSceneManager->destroyEntity(mBodyEntity);
			mSceneManager->destroySceneNode(mBodyNode);
		}
	}

	void Object::destory()
	{
		mCurrentLife = 9999;
	}

	bool Object::isFrontZ()
	{
		return mFrontZ;
	}

}
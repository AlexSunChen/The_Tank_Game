/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   BaseTank.cpp
			created:    6/29/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "BaseTank.h"
#include "ContextApp.h"

#include <ostream>
using namespace std;

namespace Tank
{


	BaseTank::BaseTank(TANK_TYPE tankID, const std::string &name, const std::string &meshName, const Ogre::Vector3 &pos
			, float damage, float heightOffset, float fireHeightOffset)
	 : mTankID(tankID), mPreMove(MOVE_UP), mMove(0,0,0), mFireCD(0), mRoateDegree(0), mCurrentDegree(0), mRotateBody(0)
		, mBulletType(BULLET_ROCKET), mSmokeParticle(0), mSpeed(1)
	{
		mDamage = damage;
		mName = name;
		Attacker::mMoney = 0;

		mSceneManager = ContextApp::getGameSceneManager();

		// tank body
		mBodyEntity = mSceneManager->createEntity(name + "_Body", meshName + "_Body.mesh");
		mBodyNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
		mBodyNode->attachObject(mBodyEntity);




		// tank header
		mFireEntity = mSceneManager->createEntity(name + "_Head", meshName + "_Head.mesh");
		mFireNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
		mFireNode->attachObject(mFireEntity);


		// Tank barrel junction
		//mLookAt = mFireNode->createChildSceneNode(Ogre::Vector3(0, 0, 3.0f));
		mSmokeParticle = mSceneManager->createParticleSystem(mName, "TankWar/Smoke");
		mSmokeNode = mFireNode->createChildSceneNode(Ogre::Vector3(0, 2.5f, 0));


		
		mFireHeightOffset = Ogre::Vector3(0, mFireEntity->getBoundingBox().getSize().y - fireHeightOffset, 0);
		Ogre::Vector3 offset(0, getBoundingBox().getHalfSize().y - heightOffset, 0);

		setPosition(pos + offset);

#if 0
		static int idx = 0;
		////! 
		{
			ostringstream oss;
			oss<<"axes_"<<idx++;
			Entity* ent = mSceneManager->createEntity("axes.mesh");
			SceneNode* axesNode = mBodyNode->createChildSceneNode(oss.str() );
			axesNode->attachObject(ent);
			axesNode->scale(0.1 , 0.1 , 0.1);
			ent->setQueryFlags( 0  );
		}

		//! 
		{
			ostringstream oss2;
			oss2<<"axes_"<<idx++;
			Ogre::Entity* ent = mSceneManager->createEntity("axes.mesh");
			Ogre::SceneNode* axesNode = mFireNode->createChildSceneNode(oss2.str() , Ogre::Vector3(0, 2.5f, 0) );
			axesNode->attachObject(ent);
			axesNode->scale(0.1 , 0.1 , 0.1);
			ent->setQueryFlags( 0  );
		}
#endif


	}



	BaseTank::~BaseTank()
	{
		mSceneManager->destroyEntity(mFireEntity);
		//mSceneManager->destroySceneNode(mLookAt);
		mSceneManager->destroyParticleSystem(mSmokeParticle);
		mSceneManager->destroySceneNode(mSmokeNode);
		mSceneManager->destroySceneNode(mFireNode);
	}



	void BaseTank::setPosition(const Ogre::Vector3 &pos)
	{
		mFireNode->setPosition(pos);
		mBodyNode->setPosition(pos);
	}



	void BaseTank::setVisible(bool visible)
	{
		mBodyNode->setVisible(visible);
		mFireNode->setVisible(visible);
	}



	void BaseTank::addHealth(float health) 
	{ 
		if (mCurrentLife - health >= 0)
			mCurrentLife -= health;
		else
			mCurrentLife = 0;
	}



	void BaseTank::updateDirection()
	{
		if ( mMove == mPreMove || mRotateBody != 0) return;

		mRoateDegree = Ogre::Degree(mMove.angleBetween(mPreMove));
		// 
		if (mMove.crossProduct(mPreMove).y > 0)
			mRotateBody = -1;
		else
			mRotateBody = 1;

		mCurrentDegree = Ogre::Degree(0);

		// 
		mPreMove = mMove;
	}



	bool BaseTank::updateBodyDirection()
	{
		if (mRotateBody != 0)
		{
			if (mCurrentDegree < mRoateDegree)
			{
				mCurrentDegree += Ogre::Degree(BODY_ROTATE_DEGREE);
				mBodyNode->yaw(Ogre::Degree( mRotateBody * BODY_ROTATE_DEGREE));
			}
			else
				mRotateBody = 0;

			return false;
		}

		return true;
	}


}
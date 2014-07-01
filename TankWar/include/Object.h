
/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   Object.h
			created:    6/29/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#pragma once

#include <string>
#include <Ogre.h>
#include <vector>
#include "Common.h"
#include "Util.h"

namespace Tank
{



	class Attacker
	{
	public:
		virtual ~Attacker(){}

		
		virtual float getDamageValue() { return mDamage; }

		
		virtual float getTotalMoney() const { return mMoney; }

		
		virtual void earnMoney(float money) { 	mMoney += money; 	}

	protected:
		float		mDamage;	
		float		mMoney;		
	};



	
	class Assailable
	{
	public:
		virtual ~Assailable(){}
		virtual float getMoney() { return mMoney; }

		
		virtual bool onHit(Attacker *attacker) = 0;

	protected:
		float mMoney;	
	};



	enum OBJET_ITEM_TYPY
	{
		OBJECT_OBJECT,	
		OBJECT_ITEM,		
	};

	
	class MapLoader;
	class ObjectManager;

	class Object : public Assailable, public IFrameListener
	{
		friend class MapLoader;
	public:
		Object();

		
		Object(Ogre::SceneManager* sceneMgr, const std::string &name, const std::string &meshName);
		virtual ~Object();

		
		inline int getID() const { return mID; }
		
		inline std::string getName() const { return mName; }

		
		inline Ogre::SceneManager	*getSceneManager() { return mSceneManager; }

		
		inline Ogre::Entity *getBodyEntity() { return mBodyEntity; }
		
		inline Ogre::SceneNode *getBodyNode() { return mBodyNode; }

	
		inline std::string getNormalMat() { return mNormalMat; }
		inline std::string getDamageMat() { return mDamageMat; }

		
		inline bool isDestructible() const { return mDestructible; }
		inline void setDestructible(bool destructible) { mDestructible = destructible; }
	
		inline int getLifeValue() const { return mLife; }
		inline void setLifeValue(float value) { mLife = value; }
		inline void addLifeValue(float value) { mLife += value; mCurrentLife += value; }
		
		inline void resetFullLifeValue() { mCurrentLife = 0;}
		
		inline int getDefenseValue() const { return mDefense; }
		inline void setDefenseValue(float defense) { mDefense = defense; }
		inline int getMapValue() const { return mMapValue; }
		
		inline int isCollision() const { return mCollision; }

		
		inline bool isAlive() const
		{ 
			float  rest =  mLife - mCurrentLife;
			return rest>0 ;
			//return mCurrentLife < mLife; 
		}

	
		virtual void setPosition(const Ogre::Vector3 &pos) { mBodyNode->setPosition(pos); }
		virtual void setScale(const Ogre::Vector3 &scale) { mBodyNode->setScale(scale); }
		virtual void setOrientation(const Ogre::Quaternion &quat) { mBodyNode->setOrientation(quat); }
		virtual Ogre::Vector3 getPosition() { return mBodyNode->_getDerivedPosition(); }
		virtual Ogre::Vector3 getScale() const { return mBodyNode->getScale(); }
		virtual Ogre::Quaternion getOrientation() const { return mBodyNode->getOrientation(); }

		
		virtual Ogre::AxisAlignedBox getBoundingBox() { return mBodyEntity->getBoundingBox(); }

		virtual void update(float timeSinceLastFrame){ }

		
		OBJET_ITEM_TYPY getObjectItemType() { return mObjectType; }

		
		bool isVisible() const{ return mBodyEntity->isVisible(); }
		
		virtual void destory();

	protected:
		int mID;								
		Ogre::SceneManager		*mSceneManager;

		Ogre::Entity						*mBodyEntity;				
		Ogre::SceneNode			*mBodyNode;				

		std::string							mMeshName;
		std::string							mName;						

		std::string							mNormalMat;				
		std::string							mDamageMat;			

		bool mDestructible;				
		bool mCollision;				

		float mLife;								
		float mCurrentLife;				
		float mDefense;					
		bool mDamaged;					
		bool mDeath;						

		int mMapValue;				

		OBJET_ITEM_TYPY			mObjectType;


		bool mFrontZ;

	public:
		bool isFrontZ();
	};
}
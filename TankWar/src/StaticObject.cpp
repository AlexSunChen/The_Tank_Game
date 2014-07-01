/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   StaticObject.cpp
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "StaticObject.h"
#include "ObjectManager.h"
#include "AudioManager.h"
#include "MapManager.h"

namespace Tank
{
	StaticObject::StaticObject(Ogre::SceneManager* sceneMgr, const std::string &name, const std::string &meshName)
		: Object(sceneMgr, name, meshName){}



	StaticObject::StaticObject(){}



	bool StaticObject::onHit(Attacker *attacker)
	{
		
		if (!mDestructible) 
			return false;

		

		float damage = attacker->getDamageValue();
		mCurrentLife += damage * 7 - mDefense * 5;
		if (!mDamaged && mCurrentLife > mLife * 0.6)
		{
			mBodyEntity->setMaterialName(mDamageMat);
			mDamaged = true;
		}

		
		if (!mDeath && mCurrentLife > mLife)
		{
			
			attacker->earnMoney((int)Util::randomRange(20, 30));

			
			MapManager::getSingletonPtr()->removeFromMap(mName, mMapValue);
	
			ObjectManager::getSingletonPtr()->destroyStaticObject(mName);

			mDeath = true;
			return true;
		}	
		else
			return false;
	}


}
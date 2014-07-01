
/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   BulletFactory.cpp
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "BaseTank.h"
#include "BaseBullet.h"
#include "BulletFactory.h"
#include "ObjectManager.h"
#include "AudioManager.h"
#include "RocketBullet.h"
#include "ImpulseBullet.h"

namespace Tank
{


	BulletFactory::BulletFactory(ObjectManager *objectFactory) : mBulletID(0), mObjectFactory(objectFactory)
	{

	}

	BulletFactory::~BulletFactory()
	{
		clear();
	}

	void BulletFactory::clear()
	{
		for (BulletIterator itr = mBullets.begin(); itr != mBullets.end(); ++itr)
			SAFE_DELETE(*itr);

		mBullets.clear();
		mBulletIndex.clear();
	}


	/**
		Create bullet
		@tank	
	*/
	BaseBullet* BulletFactory::create(BaseTank *tank)
	{
		return create(tank, tank->getFireDirection(), tank->getFirePosition());
	}



	BaseBullet* BulletFactory::create(BaseTank *tank, const Ogre::Vector3 &fireDirection, const Ogre::Vector3 &firePosition)
	{
		BaseBullet *bullet = NULL;
		
		switch(tank->getBulletType())
		{
		case BULLET_ROCKET:
			bullet = new RocketBullet(getNextBulletID(), tank, fireDirection, firePosition);
			break;
		case BULLET_IMPULSE:
			bullet = new ImpulseBullet(getNextBulletID(), tank, fireDirection, firePosition);
			break;
		default:
			bullet = new RocketBullet(getNextBulletID(), tank, fireDirection, firePosition);
		}
		
		BulletIterator itr = mBullets.insert(mBullets.end(), bullet);
		mBulletIndex.insert(std::make_pair(bullet->getName(), itr));
		return bullet;
	}



	// 更新工厂
	void BulletFactory::update( float timeSinceLastFrame ) 
	{
		timeSinceLastFrame /= 50;
		int bulletCnt = mBullets.size();
		for (BulletIterator it = mBullets.begin(); it != mBullets.end();)
		{
			BaseBullet * bullet = *it;
			bullet->update(timeSinceLastFrame);

			// 如果生存时间到期, 则删除
			if (!bullet->isAlive())
			{
				const std::string& name = bullet->getName();
				mBulletIndex.erase(name);
				SAFE_DELETE( bullet );
				mBullets.erase(it++);
			}
			else
			{
				++it;
			}
		}
	}



	Object* BulletFactory::get(const std::string &name)
	{
		BulletIndexIterator bi = mBulletIndex.find(name);
		if (bi == mBulletIndex.end())
			return NULL;

		return *bi->second;
	}




	bool BulletFactory::destroy(const std::string &name) 
	{
		BulletIndexIterator bi = mBulletIndex.find(name);
		if (bi == mBulletIndex.end())
			return false;

		(*bi->second)->destory();
		return  true;
	}


}

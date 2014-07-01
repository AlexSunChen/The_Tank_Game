
/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   BulletFactory.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include "IFactory.h"
#include <map>

namespace Tank
{
	class BaseBullet;
	class BaseTank;
	class ObjectManager;
	class BulletFactory : public IFactory
	{
		typedef std::list<BaseBullet*>::iterator BulletIterator;
		typedef std::map<std::string, BulletIterator>::iterator BulletIndexIterator;

	public:
		~BulletFactory();

		/**
			Create bulltet
			@tank	
		*/
		BaseBullet* create(BaseTank *tank);

		/**
			Create bullet
			@tank	
			@fireDirection		
			@firePosition		
		*/
		BaseBullet* create(BaseTank *tank, const Ogre::Vector3 &fireDirection, const Ogre::Vector3 &firePosition);

		
		void update( float timeSinceLastFrame ) ;

	
		Object* get(const std::string &name);

		bool destroy(const std::string &name);

		void clear();

	private:
		friend class ObjectManager;
		BulletFactory(ObjectManager *objectFactory) ;

		inline int getNextBulletID() { return mBulletID++; }

	private:
		int																		mBulletID;				

		std::list<BaseBullet*>									mBullets;
		std::map<std::string, BulletIterator>			mBulletIndex;		

		ObjectManager												*mObjectFactory;
	};
}
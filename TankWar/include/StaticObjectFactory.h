/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   StaticObjectFactory.h
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
	/**
		Static Item Factory
	*/
	class StaticObject;
	class ObjectManager;
	class StaticObjectFactory
	{
		typedef std::map<std::string, StaticObject*>::iterator ObjectIterator;

	public:
		~StaticObjectFactory();

		/**
			Create a static eninty
			@name 
			@meshName	
		*/
		StaticObject *create(const std::string &name, const std::string &meshName);

		
		Object* get(const std::string &name);

	
		bool destroy(const std::string &name);

		void update( float timeSinceLastFrame ) {}

		void clear();

	private:
		friend class ObjectManager;
		StaticObjectFactory(ObjectManager *factoryManager);

	private:
		ObjectManager											*mObjectManager;
		std::map<std::string, StaticObject*>		mObjects;				
	};
}
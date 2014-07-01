/*
	----------------------------------------------------------------------------
	***********************************************************************
				filename:   StaticObjectFactory.cpp
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "StaticObjectFactory.h"
#include "StaticObject.h"
#include "ObjectManager.h"

namespace Tank
{


	StaticObjectFactory::StaticObjectFactory(ObjectManager *objectManager) : mObjectManager(objectManager){}



	StaticObjectFactory::~StaticObjectFactory()
	{
		clear();
	}



	void StaticObjectFactory::clear()
	{
		for (ObjectIterator itr = mObjects.begin(); itr != mObjects.end(); ++itr)
			SAFE_DELETE(itr->second);

		mObjects.clear();
	}



	StaticObject* StaticObjectFactory::create(const std::string &name, const std::string &meshName)
	{
		StaticObject* object = new StaticObject(mObjectManager->getSceneManager(), name, meshName + ".mesh");
		mObjects.insert(std::make_pair(object->getName(), object));
		return object;
	}




	Object* StaticObjectFactory::get(const std::string &name)
	{
		if (mObjects.count(name) > 0)
			return mObjects[name];
		else
			return NULL;
	}




	bool StaticObjectFactory::destroy(const std::string &name)
	{
		ObjectIterator itr = mObjects.find(name);
		if (itr != mObjects.end())
		{
			SAFE_DELETE(itr->second);
			mObjects.erase(itr);
			return true;
		}

		return false;
	}


}
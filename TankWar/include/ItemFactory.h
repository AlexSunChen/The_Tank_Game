/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   ItemFactory.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include "IFactory.h"
#include "Item.h"
#include <map>

namespace Tank
{
	/**
		Item factory
	*/
	class Item;
	class ObjectManager;
	class ItemFactory : public IFactory
	{
		typedef std::map<std::string, Item*>::iterator ItemIterator;

	public:
		~ItemFactory();

		/**
			Create a item
			@itemID	
			@pos			
		*/
		Item* create(ITEM_ID itemID, const Ogre::Vector3 &pos = Ogre::Vector3::ZERO);

		
		Object* get(const std::string &name);

	
		bool destroy(const std::string &name) { return true; }


		void update( float timeSinceLastFrame ) ;

		void clear();

	private:
		friend class ObjectManager;
		ItemFactory(ObjectManager *objectManager);

		
		void generateMapItems(float timeSinceLastFrame);

		
		ITEM_ID randomItems();

		int getNextID() { return mNextID++; }

	private:
		int															mNextID;
		ObjectManager									*mObjectManager;

		std::map<std::string, Item*>			mItems;					

		std::string												ITEMS[ITEM_SIZE];
	};
}
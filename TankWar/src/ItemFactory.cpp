/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   ItemFactory.cpp
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "Item.h"
#include "ItemFactory.h"
#include "ObjectManager.h"
#include "MapManager.h"

namespace Tank
{


	const static int MAX_ITEMS_IN_MAP = 10;



	ItemFactory::ItemFactory(ObjectManager *objectManager) 
		: mObjectManager(objectManager), mNextID(0) 
	{
		ITEMS[ITEM_MONEY] = "Money.mesh";
		ITEMS[ITEM_POWER] = "Power.mesh";
		ITEMS[ITEM_HEALTH] = "Health.mesh";
		ITEMS[ITEM_DEFENSE] = "Defense.mesh";
	}



	ItemFactory::~ItemFactory()
	{
		clear();
	}



	void ItemFactory::clear()
	{
		for (ItemIterator itr = mItems.begin(); itr != mItems.end(); ++itr)
			SAFE_DELETE(itr->second);

		mItems.clear();
	}




	void ItemFactory::update( float timeSinceLastFrame ) 
	{
		timeSinceLastFrame /= 50;
		for (ItemIterator itr = mItems.begin(); itr != mItems.end();)
		{
			itr->second->update(timeSinceLastFrame);
	
			if (!itr->second->isAlive())
			{
				SAFE_DELETE(itr->second);
				mItems.erase(itr++);
			}
			else
				++itr;
		}

		generateMapItems(timeSinceLastFrame);
	}



	ITEM_ID ItemFactory::randomItems()
	{
		int radom = (int)Util::randomRange(0, 11);
		if (radom % 2 == 0)
			return ITEM_MONEY;
		else if (radom % 3 == 0)
			return ITEM_HEALTH;
		else if (radom % 7 == 0)
			return ITEM_POWER;
		else if (radom % 5 == 0)
			return ITEM_DEFENSE;
		else
			return ITEM_MONEY;
	}



	void ItemFactory::generateMapItems(float timeSinceLastFrame)
	{
		static float flushTime = 0;
		static MapManager *mapMgr = MapManager::getSingletonPtr();

		flushTime += timeSinceLastFrame;
		if (flushTime < 300)
			return;

		flushTime = 0;
		int i = 0;
		std::map<int, int> index;	
		while (i < (MAX_ITEMS_IN_MAP - (int)mItems.size()))
		{
	
			int row = (int)Util::randomRange(0, mapMgr->getMapInfo().mGridZ);
			int col = (int)Util::randomRange(0, mapMgr->getMapInfo().mGridX);

			if (!mapMgr->exsitInMap(row,col))
			{
			
				if (index.count(row) == 0 || index[row] != col)
				{
					
					int radom = Util::randomRange(0, 6);
					if (radom % 3 == 0) return;

					index.insert(std::make_pair(row, col));
					create(randomItems(), mapMgr->to3DMap(row, col, 3));
					i++;
				}
			}
		}
	}



	Item* ItemFactory::create(ITEM_ID itemID, const Ogre::Vector3 &pos)
	{
		std::string mesh = ITEMS[itemID];
		std::string name = mesh + Util::toString(getNextID());
		
		Item *item = new Item(itemID, mObjectManager->getSceneManager(), name , mesh);
		item->setPosition(pos);
		mItems.insert(std::make_pair(name, item));

		return item;
	}




	Object* ItemFactory::get(const std::string &name) 
	{
		if (mItems.count(name) > 0)
			return mItems[name];

		return NULL;
	}


}
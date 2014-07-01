/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   Item.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include "Object.h"

namespace Tank
{
	enum ITEM_ID
	{
		ITEM_MONEY		= 0,	// money
		ITEM_HEALTH		= 1,	// HP
		ITEM_POWER		= 2,	// add power
		ITEM_DEFENSE		= 3,	// defend
		ITEM_SIZE,
	};

	/**
		Categories of goods, supplies used to create items such as HP, speed, increased attacks
	*/
	class ItemFactory;
	class Item : public Object
	{
	public:
		bool onHit(Attacker *attacker);

		void update(float timeSinceLastFrame);

		ITEM_ID getItem() const { return mItemType; }
		float getItemValue() const { return mItemValue; }

	protected:
		friend class ItemFactory;
		Item(ITEM_ID item, Ogre::SceneManager* sceneMgr, const std::string &name, const std::string &meshName);

		void addAttrabution(Attacker *attacker);

	protected:
		float				mItemValue;
		ITEM_ID			mItemType;

		bool				mOver;			
	};
}
/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   Item.cpp
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "Item.h"
#include "ItemFactory.h"
#include "ObjectManager.h"
#include "GUIManager.h"
#include "AudioManager.h"
#include "ContextApp.h"

namespace Tank
{

	const static Ogre::Degree ITEM_ROTATE_DEGREE(2.0f);



	Item::Item(ITEM_ID item, Ogre::SceneManager* sceneMgr, const std::string &name, const std::string &meshName)
		: Object(sceneMgr, name, meshName), mItemType(item), mOver(false)
	{

		mObjectType = OBJECT_ITEM;

		mLife = 100;
		mBodyEntity->setQueryFlags(COLLSION_ITEM);
	}



	void Item::update(float timeSinceLastFrame)
	{
		mBodyNode->yaw(ITEM_ROTATE_DEGREE * timeSinceLastFrame);

		if (mOver)
		{

			Ogre::Vector3 pos = getPosition();
			pos.y += 0.1f;

			if (pos.y < 7.5)
			{
				setPosition(pos);
				setScale(getScale() * 0.8f);
			}


			mCurrentLife += timeSinceLastFrame;
		}
	}



	bool Item::onHit(Attacker *attacker)
	{
		if (mOver) return false;

		mOver = true;

		BaseTank* tank = static_cast<BaseTank*>(attacker);
		switch(getItem())
		{
		case ITEM_DEFENSE:
			GUIManager::setProperty("GameUI/StateImage1", "Image", "set:InfosImageSet image:Defense"); 
			tank->setDefenseValue(tank->getDefenseValue() * 1.5f);
			tank->addLifeValue(20);
			break;
		case ITEM_POWER:
			GUIManager::setProperty("GameUI/StateImage2", "Image", "set:InfosImageSet image:Power"); 
			tank->setBulletType(BULLET_IMPULSE);
			break;
		case ITEM_MONEY:
			tank->earnMoney((int)Util::randomRange(50, 100)); 
			break;
		case ITEM_HEALTH:
			tank->addHealth((int)Util::randomRange(20, 40));
			break;
		}

		ContextApp::updateScores(tank);


		AudioManager::play(AUDIO_ITEM_TAKE);
		return true;
	}


}
/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   Trail.cpp
			created:    6/28/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "Trail.h"

namespace Tank
{


	static int MESHID = 1;



	Trail::Trail(Ogre::SceneManager* sceneMgr, const std::string meshName, const std::string matName, float lifeTime)
		: mSceneMgr(sceneMgr), mMeshName(meshName), mFirstDraw(0), mMatName(matName), mLifeTime(lifeTime), mDraw(true)
	{
		mCloneEntity = mSceneMgr->createEntity("ClonedTrail" + Util::toString(MESHID++), mMeshName);
		mCloneEntity->setMaterialName(matName);
		mCloneEntity->setCastShadows(false);
	}

	Trail::~Trail(void)
	{
		mSceneMgr->destroyEntity(mCloneEntity);

		for (std::list<OneTrail*>::iterator it = mTrails.begin(); it != mTrails.end(); ++it)
			SAFE_DELETE (*it);

		mTrails.clear();
	}



	void Trail::addOneTrail(const Ogre::Vector3 &pos, const Ogre::Vector3 &normal)
	{
		if (!mDraw || !mFirstDraw) return;

		if (mFirstDraw == 1)
		{
			OneTrail* trail = new OneTrail(mSceneMgr, mMeshName, mCloneEntity, mLifeTime);
			trail->setPosition(getPosition(pos));
			mTrails.push_back(trail);

			mFirstDraw = 2;
			return;
		}

		Ogre::Vector3 p = mTrails.back()->mPos;
		float length = mTrails.back()->getSize();
		int dist = (pos - p).length();

		for (float i = length; i < dist; i += length)
		{

			Ogre::Vector3 offset = i * ((pos - p).normalisedCopy());

			OneTrail* trail = new OneTrail(mSceneMgr, mMeshName, mCloneEntity, mLifeTime);
			trail->setPosition(getPosition(p + offset));

			
			offset.y = 0;
			trail->setDirection(offset);
			mTrails.push_back(trail);
		}
	}




	Ogre::Vector3 Trail::getPosition(const Ogre::Vector3 &pos)
	{
		return pos;
	}



	void Trail::update(float timeSinceLastFrame)
	{
		timeSinceLastFrame /= 50;
		for(std::list<OneTrail*>::iterator itr = mTrails.begin(); itr != mTrails.end();)
		{
			if (!((*itr)->updata(timeSinceLastFrame)))
			{
				SAFE_DELETE(*itr);
				mTrails.erase(itr++);
			}
			else
				++itr;
		}
	}



	Trail::OneTrail::OneTrail(Ogre::SceneManager *sceMgr, const std::string &meshName, Ogre::Entity* clone, 
		float lifeTime, float scaleFactor) : mSceneMgr(sceMgr), mScaleFactor(scaleFactor), mPastTime(0), mColor(0.1f, 0.1f, 0.1f, 0.425f)
	{
		std::string name = meshName + Ogre::StringConverter::toString(MESHID++);
		mLifeTime = Util::randomRange(0.5f *lifeTime, 1.5f * lifeTime);

		
		if (!clone)
			mEntity = mSceneMgr->createEntity(name, meshName);
		else
			mEntity = clone->clone(name);

		mMaterial = mEntity->getSubEntity(0)->getMaterial()->clone(name);
		mEntity->setMaterial(mMaterial);
	
		mEntity->setCastShadows(false);

	
		mTrailNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mTrailNode->attachObject(mEntity);
		mTrailNode->setScale(mScaleFactor, 1, mScaleFactor);
	}



	Trail::OneTrail::~OneTrail()
	{
		mTrailNode->detachAllObjects();
		mSceneMgr->destroyEntity(mEntity);
		mSceneMgr->destroySceneNode(mTrailNode);
	}



	void Trail::OneTrail::setPosition(const Ogre::Vector3 &pos)
	{
		mTrailNode->setPosition(pos);
		mPos = pos;
	}



	
	bool Trail::OneTrail::updata(float timeSineLastFrame)
	{
		mPastTime += timeSineLastFrame;


		mColor.a -= timeSineLastFrame / mLifeTime;

		mMaterial->getTechnique(0)->getPass(0)->setAmbient(mColor);
		mMaterial->getTechnique(0)->getPass(0)->setDiffuse(mColor);

		return mPastTime < mLifeTime;
	}


}

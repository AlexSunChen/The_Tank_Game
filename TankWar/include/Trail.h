/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   Trail.h
			created:    6/28/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#pragma once

#include <Ogre.h>
#include <string>
#include <list>
#include "Util.h"

namespace Tank
{

	/**
	Wheels India to achieve, use mesh to simulate Wheels India.


	1. Before you start drawing cars India calls beginDraw () to advertise to draw a new car India.
	2. call addOneTrail().
	3. Over draw ,then callback endDraw().

	eg:

	bool BaseState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
	{ 
	if (id == OIS::MB_Left)	
	mTrail->beginDraw();
	return true;
	}

	bool BaseState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
	{ 
	if (id == OIS::MB_Left)
	mTrail->endDraw();
	return true; 
	}

	bool BaseState::mouseMoved(const OIS::MouseEvent &e)
	{

	if (mTrail->isDraw())
	{
	static Ogre::RaySceneQuery *query = mSceneMgr->createRayQuery(Ogre::Ray());

	float screenX = (float)e.state.X.abs / (float)e.state.width;
	float screenY = (float)e.state.Y.abs / (float)e.state.height;

	Ogre::Ray mouseRay = mCamera->getCameraToViewportRay(screenX , screenY);
	query->setRay(mouseRay);
	query->setSortByDistance(true);

	Ogre::RaySceneQueryResult &result = query->execute();
	Ogre::RaySceneQueryResult::iterator itr = result.begin();

	if (itr != result.end() && itr->worldFragment)
	{
	Ogre::Vector3 location = itr->worldFragment->singleIntersection;

	mTrail->addOneTrail(location.x, location.y + 0.5, location.z);
	}
	}
	}
	*/

	class Trail
	{
		class OneTrail;
	public:
		Trail(Ogre::SceneManager* sceneMgr, const std::string meshName, const std::string matName, float lifeTime = 10.0f);

		~Trail(void);

		
		void beginDraw() {	mFirstDraw = 1; }

		
		void endDraw() {	mFirstDraw = 0;	}

		
		bool isDraw() {	return mFirstDraw != 0;	}

	
		void addOneTrail(float x, float y, float z) { addOneTrail(Ogre::Vector3(x, y, z));	}


		void addOneTrail(const Ogre::Vector3 &pos, const Ogre::Vector3 &normal = Ogre::Vector3::UNIT_Y);


		Ogre::Vector3 getPosition(const Ogre::Vector3 &pos);

		void setDrawTrails(bool enable) { mDraw = enable; }
		bool isDrawTrails() const { return mDraw; }

		void update(float timeSinceLastFrame);

	private:
		Ogre::SceneManager	*mSceneMgr;
		Ogre::Entity					*mCloneEntity;

		bool								mDraw;

		std::list<OneTrail*>	mTrails;		

		std::string						mMeshName;		
		std::string						mMatName;

		int									mFirstDraw;		
		float								mLifeTime;

		class OneTrail
		{
		public:
			OneTrail(Ogre::SceneManager *sceMgr, const std::string &meshName, Ogre::Entity* clone = 0, float lifeTime = 5.0f, float scaleFactor = 6);

			~OneTrail();

			void setDirection(const Ogre::Vector3 &dir) { mTrailNode->setDirection(dir);	}
			void setPosition(const Ogre::Vector3 &pos);

			inline float getSize()
			{
				
				return mEntity->getBoundingBox().getSize().x * mScaleFactor;
			}

			
			bool updata(float timeSineLastFrame);

			Ogre::SceneManager		*mSceneMgr;

			Ogre::SceneNode			*mTrailNode;
			Ogre::Entity						*mEntity;
			Ogre::MaterialPtr			mMaterial;

			Ogre::Vector3					mPos;
			Ogre::ColourValue			mColor;			

			float									mScaleFactor;
			float									mPastTime;
			float									mLifeTime;
		};
	};
}

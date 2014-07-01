/*
	----------------------------------------------------------------------------
	***********************************************************************
				filename:   ChooseMapState.cpp
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "ChooseMapState.h"
#include "GameState.h"
#include "MenuState.h"
#include "AudioManager.h"
#include "GUIManager.h"
#include "StateManager.h"
#include "GameApp.h"
#include "MapManager.h"
#include "ContextApp.h"

namespace Tank
{


	// enter status
	void ChooseMapState::enter( void )
	{
		mSceneMgr = GameApp::getSingletonPtr()->getAideSceneMgr();
		mRenderWindow	= GameApp::getSingletonPtr()->getRenderWindow();

		GUIManager::loadWindow("ChooseMap");
		GUIManager::setCursorType(CURSOR_NORMAL);

		createGUIEvent();
		createCamera();

		mMaxMapNumber = MapManager::getSingletonPtr()->getMapPreSize();
		if (mMaxMapNumber>mCurrentMapIndex && mCurrentMapIndex>=0)
		{
			showMap(MapManager::getSingletonPtr()->getMapPreByIndex(mCurrentMapIndex));
		}
	}




	void ChooseMapState::exit( void )
	{
		
		mSceneMgr->destroyAllCameras();
		mRenderWindow->removeAllViewports();
		GUIManager::destoryWindow("ChooseMap");
	}



	void ChooseMapState::changeSize(int width, int height)
	{
		
		Ogre::Viewport *vp = mCamera->getViewport();
		mCamera->setAspectRatio((Ogre::Real)(vp->getActualWidth()) / (Ogre::Real)(vp->getActualHeight()));
	}



	void ChooseMapState::createCamera()
	{
		mCamera	= mSceneMgr->createCamera( "ChooseMapState" );
		mViewport = mRenderWindow->addViewport(mCamera);
		mCamera->setAspectRatio((float)mViewport->getActualWidth() / (float)mViewport->getActualHeight());
		mCamera->setNearClipDistance(0.1);
		
		mCamera->setPosition(Ogre::Vector3(56, 45, -180));
		mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
	}




	void ChooseMapState::createGUIEvent()
	{
		GUIManager::subscribeEvent("ChooseMap/Play", CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&ChooseMapState::onPlay, this));
		GUIManager::subscribeEvent("ChooseMap/Back", CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&ChooseMapState::onBack, this));
		GUIManager::subscribeEvent("ChooseMap/Pre",  CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&ChooseMapState::onPreMap, this));
		GUIManager::subscribeEvent("ChooseMap/Next", CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&ChooseMapState::onNextMap, this));

		//GUIManager::subscribeEvent("ChooseMap/Test", 
		//	CEGUI::Window::EventMouseClick,CEGUI::Event::Subscriber(&ChooseMapState::onTest, this));

	}



	void ChooseMapState::showMap(MapPre mapPre)
	{
		std::string propertys = "set:" + mapPre.name + " image:full_image";
		GUIManager::setProperty("ChooseMap/Show", "Image", propertys);
		GUIManager::getWindow("ChooseMap/Info")->setText("Name : " + mapPre.name);
		std::string number = Util::toString(mCurrentMapIndex + 1) + "/" + Util::toString(mMaxMapNumber);
		GUIManager::getWindow("ChooseMap/TotalInfo")->setText(number);
	}



	bool ChooseMapState::onPreMap(const CEGUI::EventArgs& e)
	{
		static MapManager *mapMgr = MapManager::getSingletonPtr();
		if (mCurrentMapIndex - 1 >= 0)
			mCurrentMapIndex--;

		AudioManager::play(AUDIO_MOUSE_CLICK, 0.5f, false, false);

		int maxsize = mapMgr->getMapPreSize();
		if (maxsize>mCurrentMapIndex)
		{
			showMap(mapMgr->getMapPreByIndex(mCurrentMapIndex));
		}

		return true;
	}



	bool ChooseMapState::onNextMap(const CEGUI::EventArgs& e)
	{
		static MapManager *mapMgr = MapManager::getSingletonPtr();
		if (mCurrentMapIndex + 1 < mMaxMapNumber)
			mCurrentMapIndex++;

		AudioManager::play(AUDIO_MOUSE_CLICK, 0.5f, false, false);
		int maxsize = mapMgr->getMapPreSize();
		if (maxsize>mCurrentMapIndex)
		{
			showMap(mapMgr->getMapPreByIndex(mCurrentMapIndex));
		}
		return true;
	}



	bool ChooseMapState::onPlay(const CEGUI::EventArgs& e)
	{
		AudioManager::play(AUDIO_MOUSE_CLICK, 0.5f, false, false);

		// Set map information
		MapPre mapPre = MapManager::getSingletonPtr()->getMapPreByIndex(mCurrentMapIndex);
		// Reset the player chooses the map information
		ContextApp::resetEnemyData(mapPre.maxEnemyInScene, mapPre.enemyNumber, mapPre.fileName);

		StateManager::getSingletonPtr()->changeState(GameState::getSingletonPtr());
		return true;
	}



	bool ChooseMapState::onBack(const CEGUI::EventArgs& e)
	{
		AudioManager::play(AUDIO_MOUSE_CLICK, 0.5f, false, false);
		StateManager::getSingletonPtr()->changeState(MenuState::getSingletonPtr());
		return true;
	}

	bool ChooseMapState::onTest(const CEGUI::EventArgs& e)
	{
	
		return true;
	}

}
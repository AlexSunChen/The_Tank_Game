/*
	----------------------------------------------------------------------------
	***********************************************************************
				filename:   MenuState.cpp
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "MenuState.h"
#include "GUIManager.h"
#include "Util.h"
#include "GameApp.h"
#include "GameState.h"
#include "ChooseMapState.h"
#include "StateManager.h"
#include "AudioManager.h"

namespace Tank{



	MenuState::MenuState() 
		: mTanslation(0,0,0), mInit(false), mLookAtNode(0), 
		mSnowParticle(0), mCamera(0), mViewport(0),
		mSceneMgr(0), mRenderWindow(0)
	{
		/**
			Load Game menu background scene here
			in fact, you can use the editor to generate custom particle or animation.
			Export OgreMax used here directly from 3Dmax scene.
		*/
		//Util::loadResouse(TANKWAR_RES_ROOT_PATH + "models/MenuScene/MenuScene.zip", ZIP_TYPE, "MenuScene", true, false);
	}



	MenuState::~MenuState()
	{
		
		//if (mInit)
		//	Util::unloadResouse("MenuScene");
	}



	void MenuState::enter( void )
	{

		AudioManager::play(AUDIO_INIT, 0.75f, true, false);

		mSceneMgr = GameApp::getSingletonPtr()->getAideSceneMgr();
		mRenderWindow	= GameApp::getSingletonPtr()->getRenderWindow();
		
		if (mInit)
		{
			GUIManager::loadWindow("MenuUI");
			GUIManager::setCursorType(CURSOR_NORMAL);

			createScene();
			createCamera();
			createLight();

	
			mSnowParticle->getEmitter(0)->setEnabled(true);
		}
		else
		{
			GUIManager::loadWindow("LogoUI");
			GUIManager::subscribeEvent("LogoUI/Enter" , CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&MenuState::onInit, this));
			GUIManager::subscribeEvent("LogoUI/Enter" , CEGUI::DefaultWindow::EventMouseEntersArea, CEGUI::Event::Subscriber(&MenuState::onHovor, this));
			createCamera();
			createLight();
		}
	}

	void MenuState::exit( void )
	{
		// Each time you exit or pause when you need to remove all cameras and viewports
		mSceneMgr->destroyAllCameras();
		mRenderWindow->removeAllViewports();
		mSceneMgr->destroyAllLights();

		if (mInit)
		{
			// Leave this scene, stop emitting particles 
			mSnowParticle->getEmitter(0)->setEnabled(false);
			GUIManager::destoryWindow("MenuUI");
		}

		AudioManager::stop(AUDIO_INIT , false);
	}



	void MenuState::createScene()
	{
		// MenuScene scene loaded only once
		static bool load = false;

		if (!load){
			//Util::parseDotScene("MenuScene.scene", mSceneMgr, "MenuScene");
			mLookAtNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();

			// Adding fog effects scene
			mSceneMgr->setFog(Ogre::FOG_EXP, Ogre::ColourValue(214.0f / 255.0f, 234.0f / 255.0f, 245.0f / 255.0f)
				, 0.0025f, 180.0f, 200.0f);
			createParticle();
			load = true;
		}

		createGUIEvent();
	}



	void MenuState::createParticle()
	{
	
		mSnowParticle = mSceneMgr->createParticleSystem("Snow", "TankWar/snow");
		Ogre::SceneNode* snow = mSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3(30.0f,  30.0f, 30.0f));
		snow->attachObject(mSnowParticle);
	
		Ogre::ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);
	}



	void MenuState::createCamera()
	{
		mSceneMgr->destroyAllCameras();
		mRenderWindow->removeAllViewports();

		mCamera	= mSceneMgr->createCamera( "MenuStateCamera" );
		mViewport = mRenderWindow->addViewport(mCamera);
		mCamera->setAspectRatio((float)mViewport->getActualWidth() / (float)mViewport->getActualHeight());
		mCamera->setNearClipDistance(0.1);

	
		mCamera->setPosition(Ogre::Vector3(56, 45, -180));
	
		if (mLookAtNode) mCamera->setAutoTracking(true, mLookAtNode);

		mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
	}



	void MenuState::createLight()
	{
		mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 0.0f, 0.0f));
		Ogre::Light *light = mSceneMgr->createLight("MainLight");
		light->setPosition(Ogre::Vector3(100, 100, 100));
	}



	void MenuState::update( float timeSinceLastFrame )
	{
		if (!mInit) return;
		updateCamera(timeSinceLastFrame);
	}

	void MenuState::createGUIEvent()
	{
		GUIManager::subscribeEvent("MenuUI/MenuFrame/SinglePlayer",CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MenuState::onSinglePlayer, this));
		GUIManager::subscribeEvent("MenuUI/MenuFrame/Exit",CEGUI::PushButton::EventClicked,CEGUI::Event::Subscriber(&MenuState::onExit, this));
	
		GUIManager::subscribeEvent("MenuUI/MenuFrame/SinglePlayer" , CEGUI::DefaultWindow::EventMouseEntersArea, CEGUI::Event::Subscriber(&MenuState::onHovor, this));
		GUIManager::subscribeEvent("MenuUI/MenuFrame/Exit" , CEGUI::DefaultWindow::EventMouseEntersArea, CEGUI::Event::Subscriber(&MenuState::onHovor, this));
	}

	bool MenuState::onSinglePlayer(const CEGUI::EventArgs& e)
	{
		AudioManager::play(AUDIO_MOUSE_CLICK, 0.5f, false, false);
		StateManager::getSingletonPtr()->changeState(ChooseMapState::getSingletonPtr());
		return true;
	}

	bool MenuState::onExit(const CEGUI::EventArgs& e)
	{
		GameApp::getSingletonPtr()->shutDown();
		return true;
	}

	bool MenuState::onHovor(const CEGUI::EventArgs& e)
	{
		AudioManager::play(AUDIO_MOUSE_OVER, 1.0f, false, false);
		return true;

	}


	bool MenuState::onInit(const CEGUI::EventArgs& e)
	{
		AudioManager::play(AUDIO_MOUSE_CLICK, 0.5f, false, false);

		
		GUIManager::destoryWindow("LogoUI");
		GUIManager::loadWindow("MenuUI");
		GUIManager::setCursorType(CURSOR_NORMAL);

		createScene();
		createCamera();

		
		mSnowParticle->getEmitter(0)->setEnabled(true);
		mInit = true;
		return true;
	}

	void MenuState::updateCamera(float timeSinceLastFrame)
	{
		static long delay = 0;
		static int dir = -1;

		/**
			Update camera angle, every move in the opposite direction to change the direction of movement of 8s.
			Since the camera is bound to the node, it will do circular motion around the node
		*/
		delay += timeSinceLastFrame;
		//if (delay > 8000)	// 
		//{
		//	delay = 0;
		//	dir *= -1;	
		//}

		// Set the camera moves according to its own axis relative distance
		// the effect is to follow the camera moves toward
		//move for the camera to move in accordance with the world axes, moveRelative own axis moves in the camera
		mCamera->moveRelative(Ogre::Vector3(1.0f, 0.0f, 0.0f) * dir);
	}



	void MenuState::changeSize(int width, int height)
	{
		
		Ogre::Viewport *vp = mCamera->getViewport();
		mCamera->setAspectRatio((Ogre::Real)(vp->getActualWidth()) / 
 					  (Ogre::Real)(vp->getActualHeight()));
	}


}
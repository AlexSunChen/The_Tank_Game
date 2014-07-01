/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   GameApp.cpp
			created:    6/30/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "GameApp.h"
#include "MenuState.h"
#include "StateManager.h"
#include "InputManager.h"
#include "GUIManager.h"
#include "GameState.h"
#include "ObjectManager.h"
#include "Util.h"

using namespace OgreBites;



namespace Tank
{
	using namespace Ogre;

	OgreBites::Label* GameApp::mDebugText = NULL;   


	GameApp::GameApp(void) 
		: mIsShutDown(false), mInputSystem(0), mGameSceneMgr(0), mGUIManager(0)
		, mAideSceneMgr(0), mRenderWindow(0), mRoot(0), mStateManager(0), mLostFouse(false)
		, mWindowWidth(1.0f), mWindowHeight(1.0f) , mTrayMgr(0)
		, mSysMouseShowFlag(false)
	{
	}



	GameApp::~GameApp(void)
	{
		OGRE_DELETE mRoot;

	}	



	void GameApp::log(const std::string &msg)
	{
		Ogre::LogManager::getSingletonPtr()->getDefaultLog()->logMessage(msg);
	}



	void GameApp::startGame()
	{
		if (!setup())
			return;

		// menu
		mStateManager->pushState(MenuState::getSingletonPtr());

		unsigned long timeLastFrame = 0;
		unsigned long timeCurrentFrame = 0;
		unsigned long timeSinceLastFrame = 0;

		Ogre::Timer *timer = mRoot->getTimer();

		// Main loop
		while( !mIsShutDown && (!mStateManager->empty()) )
		{
			timeCurrentFrame	= timer->getMilliseconds();
			timeSinceLastFrame	= timeCurrentFrame - timeLastFrame;
			timeLastFrame			= timeCurrentFrame;

			Ogre::RenderTarget::FrameStats stats = mRenderWindow->getStatistics();
			if (stats.lastFPS > 30)
			{
				Sleep(20);
			}
			// Event update is based on the millisecond level
			// OIS incident captured
			mInputSystem->capture();

			// Update the current status
			mStateManager->update(timeSinceLastFrame);
			// Rendering the next frame
			mRoot->renderOneFrame();
			// After the rendering process
			mStateManager->updateAfterRender(timeSinceLastFrame);
			mAudioManager->update(timeSinceLastFrame);

			// window event processing
			Ogre::WindowEventUtilities::messagePump();
		}

		release();
	}



	bool GameApp::frameRenderingQueued(const Ogre::FrameEvent& evt)
	{

		mTrayMgr->frameRenderingQueued(evt);

		return true;


	}
	void GameApp::release()
	{

		mStateManager->destroy();
		mInputSystem->destroy();
		mGUIManager->destroy();
		mAudioManager->destroy();

		mGameSceneMgr->clearScene();	
		mAideSceneMgr->clearScene();

		mRoot->destroySceneManager(mGameSceneMgr);	
		mRoot->destroySceneManager(mAideSceneMgr);

		mRenderWindow->destroy();
		mRoot->shutdown();

	}



	bool GameApp::setup()
	{
		mRoot = OGRE_NEW Ogre::Root();

		if (!configure()) 
			return false;

		// load Resources
		setupResources();

		// Create a scene manager, where the scene manager needs before initiating initManagers call
		// Because initManagers which will call to SceneManager
		createSceneMgr();

		// init Manger
		initManagers();

		// Create Listener
		createListener();

		return true;
	}



	void GameApp::createSceneMgr()
	{
		mAideSceneMgr = mRoot->createSceneManager(ST_GENERIC, "TankWar-Aide");
		mGameSceneMgr = mRoot->createSceneManager(ST_GENERIC, "TankWar");

		// Create a shadow
		mGameSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_MODULATIVE);
		mGameSceneMgr->setShadowTextureSize(512);
		mGameSceneMgr->setShadowColour(Ogre::ColourValue(0.0f, 0.0f, 0.7f));
		mGameSceneMgr->setShadowFarDistance(1000);

		mGameSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
	}



	void GameApp::initManagers()
	{
		// State Manager
		mStateManager = StateManager::getSingletonPtr();

		// init GUI
		mGUIManager = GUIManager::getSingletonPtr();
		mGUIManager->init();

		// init OIS
		mInputSystem = InputManager::getSingletonPtr();
		mInputSystem->init(mRenderWindow, false);

		// Get Object Manager
		mObjectFactory = ObjectManager::getSingletonPtr();

		// Audio Manager
		mAudioManager = AudioManager::getSingletonPtr();
		
	}



	void GameApp::createListener()
	{
		//Set callback function OIS
		mInputSystem->setKeyboardEventCallback(this);
		mInputSystem->setMouseEventCallback(this);

		mWindowWidth = mRenderWindow->getWidth();
		mWindowHeight = mRenderWindow->getHeight();

		// Event registration window
		Ogre::WindowEventUtilities::addWindowEventListener(mRenderWindow, this);


		mTrayMgr = new SdkTrayManager("InterfaceName", mRenderWindow, mInputSystem->getMouse(), this);
		mTrayMgr->showFrameStats(OgreBites::TL_TOP);
		mTrayMgr->hideCursor();
		mTrayMgr->hideAll();

		//! debug text
		mDebugText = mTrayMgr->createLabel(OgreBites::TL_BOTTOM , "debug_label" , "debug..."  , 800  );


		mRoot->addFrameListener(this);

	}




	bool GameApp::configure(void)
	{
		if(mRoot->restoreConfig() || mRoot->showConfigDialog())
		{
			mRenderWindow = mRoot->initialise(true, "TankWar");
			return true;
		}
		else
			return false;

		return true;
	}



	void GameApp::setupResources(void)
	{
		Ogre::ConfigFile cf;
		cf.load("resources.cfg");

		Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

		Ogre::String secName, typeName, archName;
		while (seci.hasMoreElements())
		{
			secName = seci.peekNextKey();
			Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
			Ogre::ConfigFile::SettingsMultiMap::iterator i;
			for (i = settings->begin(); i != settings->end(); ++i)
			{
				typeName = i->first;
				archName = i->second;

				Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
			}
		}

		
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
		Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	}



	bool GameApp::keyPressed( const OIS::KeyEvent &arg )
	{
		if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
		{
			mRenderWindow->writeContentsToTimestampedFile("screenshot", ".png");
			return true;
		}
		else if (arg.key == OIS::KC_2)
		{
			if (mTrayMgr->areTraysVisible())
			{
				mTrayMgr->hideAll();
			}
			else
			{
				mTrayMgr->showAll();
				mTrayMgr->hideCursor();
			}
		}

		return mStateManager->keyPressed(arg);
	}



	bool GameApp::keyReleased( const OIS::KeyEvent &arg )
	{
		return mStateManager->keyReleased(arg);
	}



	bool GameApp::mouseMoved( const OIS::MouseEvent &arg )
	{
		//if (!checkWindowBound(arg))
		//	return true;

		if(arg.state.X.abs < mRenderWindow->getViewport(0)->getActualLeft()+1
			||(arg.state.Y.abs < mRenderWindow->getViewport(0)->getActualTop()+1)
			||(arg.state.X.abs > mRenderWindow->getViewport(0)->getActualLeft()+mRenderWindow->getViewport(0)->getActualWidth())  
			||(arg.state.Y.abs > mRenderWindow->getViewport(0)->getActualTop() +mRenderWindow->getViewport(0)->getActualHeight()))        
		{
			if( !mSysMouseShowFlag)
			{
				ShowCursor(1);
				CEGUI::MouseCursor::getSingleton().hide();
				mSysMouseShowFlag = true;
			}

		}
		else
		{
			if( mSysMouseShowFlag)
			{
				ShowCursor(0);
				CEGUI::MouseCursor::getSingleton().show();
				mSysMouseShowFlag = false;
			}
		}

		// Constantly updated CEGUI's mouse position 
		OIS::MouseState state = arg.state;

		if (mGUIManager->injectMouseMove(state.X.rel, state.Y.rel, state.X.abs, state.Y.abs))
		{
			return true;
		}
		
		return mStateManager->mouseMoved(arg);
	}



	bool GameApp::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (!checkWindowBound(arg))
			return true;

		if (mGUIManager->injectMouseButtonDown(MB_OISToCEGUI(id)))
			return true;

		return mStateManager->mousePressed(arg, id);
	}



	bool GameApp::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
	{
		if (!checkWindowBound(arg))
			return true;

		if (mGUIManager->injectMouseButtonUp(MB_OISToCEGUI(id)))
			return true;

		return mStateManager->mouseReleased(arg, id);
	}

	
	void GameApp::windowResized(Ogre::RenderWindow* rw)
	{
		mRenderWindow->windowMovedOrResized(); 

		mWindowWidth = mRenderWindow->getWidth();
		mWindowHeight = mRenderWindow->getHeight();

		/* If you can change the window size, then you need to call 
		this function to change the size of the update window*/
		mInputSystem->updateMouseExtents(mWindowWidth, mWindowHeight);

		// Notify "CEGUI" redraw
		mGUIManager->changeSize(mWindowWidth, mWindowHeight);

		// Notify the "State" to change the view size
		mStateManager->changeSize(mWindowWidth, mWindowHeight);
	}


	void GameApp::OutputDebugMessage(std::string strMessage)
	{
		mDebugText->setCaption(strMessage );
	}




}
/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   GameApp.h
			created:    6/30/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#ifndef __GAME_H__
#define __GAME_H__

#include <Ogre.h>
#include <OIS/OIS.h>
#include "SingletonT.h"
#include "AudioManager.h"

#include <SdkTrays.h>


namespace Tank
{
	class GUIManager;
	class InputManager;
	class StateManager;
	class ObjectManager;


	class GameApp : public SingletonT<GameApp>, public Ogre::WindowEventListener, public OIS::KeyListener
		, public OIS::MouseListener , OgreBites::SdkTrayListener , public Ogre::FrameListener
	{
		SINGLETON_DEFINE(GameApp)

	public:
		~GameApp(void);

		/**
		GameLog FILE
		@msg record the game's log
		*/
		static void log(const std::string &msg);

		// Start the Game Loop;
		void startGame();

		// End the Game loop
		inline void shutDown() { mIsShutDown = true; }

		// init the Ogre's SceneManger
		inline Ogre::SceneManager *getGameSceneMgr() { return mGameSceneMgr; }
		// Auxiliary scene manager, such as Menu
		inline Ogre::SceneManager *getAideSceneMgr() 
		{
			return mAideSceneMgr; 

		}

		// Get the Ogre's main rendering window
		inline Ogre::RenderWindow* getRenderWindow() { return mRenderWindow; }

		// Get The Rendering Window's width and heigth
		inline float getWindowWidth() const { return mWindowWidth; }
		inline float getWindowHeight() const { return mWindowHeight; }

	protected:
		// Setup Ogre
		bool setup();

		// Load plugin
		bool configure(void);
		// Load Resource
		void setupResources(void);
		// Clean up resources
		void release();

	protected:	
		// OI Event
		inline bool keyPressed( const OIS::KeyEvent &arg );
		inline bool keyReleased( const OIS::KeyEvent &arg );

		inline bool mouseMoved( const OIS::MouseEvent &arg );
		inline bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
		inline bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

		// Window Event
		void windowResized(Ogre::RenderWindow* rw);
		void windowClosed(Ogre::RenderWindow* rw) { shutDown(); }
		void windowFocusChange(Ogre::RenderWindow* rw) { mLostFouse = !mLostFouse; }

		// Ogre::FrameListener
		virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);


	protected:
		GameApp(void);
		void createSceneMgr();
		void createListener();
		// init Manager
		void initManagers();

		/*Detect if the mouse is detected non-exclusive mode range of valid clicks the mouse
		 the mouse out of the window directly in return */
		inline bool checkWindowBound(const OIS::MouseEvent &arg)
		{
			if(	arg.state.X.abs<=0 || arg.state.X.abs >= mWindowWidth
				|| arg.state.Y.abs<=0 || arg.state.Y.abs>=mWindowHeight)
				return false;

			return true;
		}

	protected:
		Ogre::Root						*mRoot;
		Ogre::RenderWindow		*mRenderWindow;

		Ogre::SceneManager		*mGameSceneMgr;	// The scene manager which used by this game
		Ogre::SceneManager		*mAideSceneMgr; // Auxiliary scene manager for other purposes

		InputManager					*mInputSystem;		// OI system
		StateManager					*mStateManager;		// State Manager
		GUIManager						*mGUIManager;		// GUI Manager
		AudioManager					*mAudioManager;	// Audio Manager
		ObjectManager					*mObjectFactory;		// ObjectFactory

		bool									mIsShutDown;			// is ShutDown
		bool									mLostFouse;				// IS LostForce

		float									mWindowWidth;		// Window width
		float									mWindowHeight;		// Window height


		OgreBites::SdkTrayManager* mTrayMgr;
		//! a label for debug information
		static OgreBites::Label* mDebugText;   

		bool mSysMouseShowFlag;

	public:
		static void OutputDebugMessage(std::string strMessage);

	};

}

#endif

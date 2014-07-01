/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   GameState.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include <Ogre.h>
#include "State.h"
#include "SingletonT.h"
#include "GreenTank.h"
#include "ChaseCamera.h"

#include <CEGUI.h>


namespace Tank
{
	class MiniMap;

	class GameState : public State, public SingletonT<GameState>
	{
		SINGLETON_DEFINE(GameState)

		enum MOUSEBUTTONSTATE
		{
			LEFT_BUTTON_DOWN,
			RIGHT_BUTTON_DOWN,
			BUTTON_UP,
		};

	public:
		~GameState();

		// enter status
		virtual void enter( void );
		// exit status
		virtual void exit( void );

		// pause status
		virtual void pause( void );
		// resume status
		virtual void resume( void );

		// update status
		virtual void update( float timeSinceLastFrame );
		// resize the window
		void changeSize(int width, int height);

		bool keyPressed( const OIS::KeyEvent &e );
		bool keyReleased( const OIS::KeyEvent &e );

		bool mouseMoved(const OIS::MouseEvent &e);
		bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
		bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

	protected:
		GameState();
		void createScene();
		void createCamera();
		void createLight();
		void createUI();

		//void updateCamera(float timeSinceLastFrame);

	protected:
		Ogre::Root							*mRoot;
		Ogre::RenderWindow		*mRenderWindow;

		Ogre::SceneManager			*mSceneMgr;

		Ogre::Camera						*mCamera;
		Ogre::Viewport					*mViewport;

		Ogre::Vector3						mMove;
		float										mMoveSpeed;
		float										mRotateSpeed;

		Ogre::Degree						mRotX;
		Ogre::Degree						mRotY;

		GreenTank							*mPlayerTank;
		ChaseCamera						*mChaseCamera;
		MiniMap								*mMiniMap;

		MOUSEBUTTONSTATE		mMouseState;

		bool mRightButtonDown;
		
	public:
		//! winner window
		bool onWinnerWindowClick(const CEGUI::EventArgs& e);
		bool onLoserWindowClick(const CEGUI::EventArgs& e);


	};
}

#endif
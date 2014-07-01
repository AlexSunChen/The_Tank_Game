/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   MenuState.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#ifndef __MENUSTATE_H__
#define __MENUSTATE_H__

#include <CEGUI.h>
#include "State.h"
#include "SingletonT.h"

namespace Tank
{
	/**
		Main menu state
	*/
	class MenuState : public State, public SingletonT<MenuState>
	{
		SINGLETON_DEFINE(MenuState)

	public:
		~MenuState();

		// Enter status 
		void enter( void );
		// exit status
		void exit( void );

		// pause status
		void pause( void ) {}
		// resume state
		void resume( void ) {}

		// Update status
		void update( float timeSinceLastFrame );
		//  rendering update status
		void updateAfterRender( float timeSinceLastFrame){}

		// After the call to change the window size, 
		//the state is responsible for the handling of the window changes
		void changeSize(int width, int height);

	private:
		MenuState();

		// Create a scene called when entering state
		void createScene();
		// Create Camera
		void createCamera();
		// Create light
		void createLight();
		// Create particle effects
		void createParticle();

		// create GUI
		void createGUIEvent();
		// Single-player event listener callback function
		bool onSinglePlayer(const CEGUI::EventArgs& e);
		bool onExit(const CEGUI::EventArgs& e);
		bool onInit(const CEGUI::EventArgs& e);
		bool onHovor(const CEGUI::EventArgs& e);
		
		// Update Camera
		void updateCamera(float timeSinceLastFrame);

	private:
		Ogre::SceneManager			*mSceneMgr;
		Ogre::RenderWindow		*mRenderWindow;

		Ogre::ParticleSystem			*mSnowParticle;

		Ogre::SceneNode				*mLookAtNode;
		Ogre::Vector3						mTanslation;

		Ogre::Camera						*mCamera;
		Ogre::Viewport					*mViewport;

		bool										mInit;
	};
}
#endif
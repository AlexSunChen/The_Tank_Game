/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   ChooseMapState.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include <CEGUI.h>
#include "State.h"
#include "SingletonT.h"

namespace Tank
{
	/**
		choose map state
	*/
	struct MapPre;
	class ChooseMapState : public State, public SingletonT<ChooseMapState>
	{
		SINGLETON_DEFINE(ChooseMapState)

	public:
		~ChooseMapState(){}

		
		void enter( void );

		
		void exit( void );

		void update( float timeSinceLastFrame ){}

		
		void pause( void ) {}
		
		void resume( void ) {}

		void changeSize(int width, int height);

	private:
		ChooseMapState() : mCurrentMapIndex(0){}

		//Show preview map
		void showMap(MapPre mapPre);

		void createCamera();


		void createGUIEvent();

		bool onPreMap(const CEGUI::EventArgs& e);
		bool onNextMap(const CEGUI::EventArgs& e);
		bool onPlay(const CEGUI::EventArgs& e);
		bool onBack(const CEGUI::EventArgs& e);
		bool onTest(const CEGUI::EventArgs& e);

	private:
		Ogre::SceneManager			*mSceneMgr;
		Ogre::RenderWindow		*mRenderWindow;

		Ogre::Camera						*mCamera;
		Ogre::Viewport					*mViewport;

		int											mCurrentMapIndex;	// The current map index
		int											mMaxMapNumber;	// The current maximum number of maps
	};
}
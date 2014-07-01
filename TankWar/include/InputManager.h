/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   InputManager.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#ifndef __INPUTSYSTEM__
#define __INPUTSYSTEM__

#include <Ogre.h>
#include <OIS/OIS.h>
#include "SingletonT.h"

namespace Tank
{
	/**
		OIS
	*/
	class InputManager : public SingletonT<InputManager>
	{
		SINGLETON_DEFINE(InputManager)

	public:
		~InputManager();

		void destroy();

		/**
			Initialization, only the first call to the effective
			@renderWindow	
			@isNexclusive		
		*/
		void init(Ogre::RenderWindow* renderWindow, bool isNexclusive = true);


		void setMouseEventCallback(OIS::MouseListener *mouse);
		void setKeyboardEventCallback(OIS::KeyListener *keyboard);

		/**
			Updated mouse movement range
			@width	
			@height	
		*/
		void updateMouseExtents(float width, float height);


		inline void capture()
		{
			mMouse->capture();
			mKeyboard->capture();
		}

	
		inline bool isNexclusive() const { return mIsNexclusive; }

		inline OIS::Mouse* getMouse() { return mMouse; }

		inline OIS::Keyboard* getKeyboard() { return mKeyboard; }

	protected:
		InputManager();

	protected:
		OIS::InputManager			*mInputManager;
		OIS::Mouse						*mMouse;
		OIS::Keyboard					*mKeyboard;

		Ogre::RenderWindow		*mRenderWindow;

		bool									mIsNexclusive;

	};
}

#endif
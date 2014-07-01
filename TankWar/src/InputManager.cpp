/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   InputManager.cpp
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "InputManager.h"

namespace Tank
{


	InputManager::InputManager() : 
		mInputManager(0), mMouse(0), mRenderWindow(0),
		mKeyboard(0), mIsNexclusive(false) {}



	InputManager::~InputManager() { }



	void InputManager::init(Ogre::RenderWindow* renderWindow, bool isNexclusive)
	{
		if (!mInputManager)
		{
			mRenderWindow = renderWindow;
			mIsNexclusive = isNexclusive;

			OIS::ParamList pl;
			size_t windowHnd = 0;
			std::ostringstream windowHndStr;

			renderWindow->getCustomAttribute("WINDOW", &windowHnd);
			windowHndStr << windowHnd;
			pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

			if (!isNexclusive)
			{	
				// Under the non-exclusive mode mouse out of the window outside
				pl.insert(std::make_pair(std::string("w32_mouse"), "DISCL_FOREGROUND"));
				pl.insert(std::make_pair(std::string("w32_mouse"), "DISCL_NONEXCLUSIVE"));
				
				//::ShowCursor(false);
			}

			
			mInputManager = OIS::InputManager::createInputSystem( pl );

			mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
			mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

			
			updateMouseExtents(mRenderWindow->getWidth(), mRenderWindow->getHeight());
		}		
	}



	// Set mouse movement range
	void InputManager::updateMouseExtents(float width, float height)
	{
		
		const OIS::MouseState &mouseState = mMouse->getMouseState();
		mouseState.width  = width;
		mouseState.height = height;
	}



	void InputManager::setMouseEventCallback(OIS::MouseListener *mouse)
	{
		mMouse->setEventCallback(mouse);
	}



	void InputManager::setKeyboardEventCallback(OIS::KeyListener *keyboard)
	{
		mKeyboard->setEventCallback(keyboard);
	}



	void InputManager::destroy()
	{
		if (mInputManager)
		{
			mInputManager->destroyInputObject(mMouse);
			mInputManager->destroyInputObject(mKeyboard);
			OIS::InputManager::destroyInputSystem(mInputManager);

			mInputManager = 0;
		}
	}


}
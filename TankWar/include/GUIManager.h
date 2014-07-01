/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   GUIManager.h
			created:    6/29/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include <CEGUIOgreRenderer.h>
#include <CEGUI.h>
#include <Ogre.h>

#include "SingletonT.h"
#include "Common.h"


namespace Tank{

	/**
	The OIS mouse converted into CEGUI mouse
	@buttonID	OISMouse enumeration
	*/
	CEGUI::MouseButton MB_OISToCEGUI(int buttonID);

	// Mouse type
	enum CURSOR_TYPE
	{
		CURSOR_ON				= 0,		// Finger
		CURSOR_FIRE				= 1,		// Arrack
		CURSOR_NORMAL		= 2,		// Normal
	};

	/**
	GUI management system, is responsible for loading the GUI interface, events and management
	*/
	class GUIManager : public SingletonT<GUIManager>, public IFrameListener
	{
		SINGLETON_DEFINE(GUIManager)

	public:
		virtual ~GUIManager();

		
		void destroy();

		/**
		Initialize the GUI, only the first call effective
		@rendweWindow		rendering window
		@sceneManager		GUI display scene
		*/
		void init();

		
		inline CEGUI::OgreRenderer* getGUIRender() { return mpGUIRenderer; }
		// Get the current root Window GUI
		inline CEGUI::Window* getGUISheet() 
		{
			//assert(0&&"CEGUI::Window* getGUISheet");
			CEGUI::System &sys = CEGUI::System::getSingleton();

			return sys.getGUISheet(); 
		}

		// Injected into mouse events CEGUI
		inline bool injectMouseButtonDown(CEGUI::MouseButton id) 
		{ 
			CEGUI::System &sys = CEGUI::System::getSingleton();
			return sys.injectMouseButtonDown(id);
		}
		inline bool injectMouseButtonUp(CEGUI::MouseButton id) 
		{
			CEGUI::System &sys = CEGUI::System::getSingleton();
			return sys.injectMouseButtonUp(id); 
		}
		// CEGUI injected mouse move events,
		// because constantly updated mouse position, so the inline
		inline bool injectMouseMove(float xRel, float yRel, float xAbs, float yAbs)
		{
			//static CEGUI::MouseCursor* mouse = CEGUI::MouseCursor::getSingletonPtr();
			//mouse->setPosition(CEGUI::Point(xAbs, yAbs));
			CEGUI::System &sys = CEGUI::System::getSingleton();

			sys.injectMouseMove(xRel, yRel);
			return sys.injectMousePosition(xAbs, yAbs);
		}

		/**
		When the window size of the update call
		@width	
		@height	
		*/
		void changeSize(float width, float height);

		void update( float timeSinceLastFrame );

		/**
		Register CEGUI control events
		@windowName	    control event name
		@eventName		event
		@subscriber		The callback function is triggered when an event
		*/
		static void subscribeEvent(const CEGUI::String& windowName, 
			const CEGUI::String &eventName, 
			const CEGUI::Event::Subscriber &subscriber);

		// Get GUI window manager
		static CEGUI::WindowManager* getWindowManager() { return CEGUI::WindowManager::getSingletonPtr(); }

		/**
		Create a Window Controls
		@type		window type
		@name	Window name. It must be globally unique
		*/
		static CEGUI::Window* createWindow(const std::string &type, const std::string &name);

		/**
		get window
		@name 
		*/
		static CEGUI::Window* getWindow(const std::string &windowName);

		
		static void setProperty(const std::string &windowName, const std::string& name, 
			const std::string& value);

		/**
		According to the picture constructed Imageset
		@name		Imageset name created. globally unique
		@filename	Imageset's picture name 
		*/
		static void createImageset(const std::string& name, const std::string& filename);

		/**
		Load UI layout, after loading the settings for the current UI
		Note: to ensure that the same layout file name and Root:Root name = Window, Window.layout
		If the layout is not loaded returns true, if the layout has been loaded or false
		@windowName	the name of layout
		*/
		static bool loadWindow(const std::string &windowName);

		/**
		destroy window
		@windowName	
		*/
		static void destoryWindow(const std::string &windowName);

		// Show / Hide GUI mouse
		static void showGUICursor(bool enable);
		// Set mouse style
		static void setCursorType(CURSOR_TYPE cursor);

	private:
		GUIManager();

	private:
		CEGUI::OgreRenderer			*mpGUIRenderer;
		CEGUI::System				*mpGUISystem;

	};
}

#endif
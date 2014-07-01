/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   GUIManager.cpp
			created:    6/29/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "GUIManager.h"
#include "Util.h"

namespace Tank{

	CEGUI::MouseButton MB_OISToCEGUI(int buttonID)
	{
		switch (buttonID)
		{
		case 0: return CEGUI::LeftButton;
		case 1: return CEGUI::RightButton;
		case 2:	return CEGUI::MiddleButton;
		case 3: return CEGUI::X1Button;
		default: return CEGUI::LeftButton;
		}
	}

	
	const std::string MOUSE_CURSOR[3] = {"CursorOn", "CursorFire", "CursorNormal"};

	GUIManager::GUIManager() : mpGUISystem(0),  mpGUIRenderer(0){}


	GUIManager::~GUIManager()
	{

	}

	void GUIManager::destroy()
	{
		//SAFE_DELETE(mpGUISystem);
		//SAFE_DELETE(mpGUIRenderer);
	}

	//void GUIManager::setTargetSceneManager(Ogre::SceneManager* mgr)
	//{
	//	//mpGUIRenderer->setRenderingEnabled(true);
	//}

	void GUIManager::init()
	{
		if (!mpGUIRenderer)
		{
			mpGUIRenderer = &CEGUI::OgreRenderer::bootstrapSystem();

#ifdef _DEBUG
			CEGUI::Logger::getSingleton().setLoggingLevel(CEGUI::Informative);
#endif

			// set the default resource groups to be used
			CEGUI::Imageset::setDefaultResourceGroup("imagesets");
			CEGUI::Font::setDefaultResourceGroup("fonts");
			CEGUI::Scheme::setDefaultResourceGroup("schemes");
			CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
			CEGUI::WindowManager::setDefaultResourceGroup("layouts");
			CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

			// setup default group for validation schemas
			CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
			if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
				parser->setProperty("SchemaDefaultResourceGroup", "schemas");


			CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");

			// Load GUI theme and set default parameters
			CEGUI::SchemeManager::getSingleton().create("VanillaSkin.scheme");
			CEGUI::SchemeManager::getSingleton().create("WindowsLook.scheme");
			CEGUI::SchemeManager::getSingleton().create("TankWar.scheme");
			CEGUI::SchemeManager::getSingleton().create("OgreTray.scheme");

			

			CEGUI::System &sys = CEGUI::System::getSingleton();
			sys.setDefaultFont("BlueHighway-10");
			sys.setDefaultTooltip("WindowsLook/Tooltip");

			setCursorType(CURSOR_NORMAL);
			showGUICursor(true);
		}
	}

	CEGUI::Window* GUIManager::getWindow(const std::string &windowName)
	{
		return getWindowManager()->getWindow(windowName);
	}

	CEGUI::Window* GUIManager::createWindow(const std::string &type, const std::string &name)
	{
		return getWindowManager()->createWindow(type, name);
	}

	bool GUIManager::loadWindow(const std::string &windowName)
	{
		bool flag = false;
		// Detect files with the given layout are loaded, not loaded then loaded
		if(!getWindowManager()->isWindowPresent(windowName))
		{
			// Read a UI layout from. Layout script file and place it into the GUI resource group.
			CEGUI::Window *editorGuiSheet = getWindowManager()->loadWindowLayout(windowName + ".layout");
			// Next we tell which parts of CEGUI UI layout display. 
			//Of course, we can always replace the UI layout display.
			CEGUI::System &sys = CEGUI::System::getSingleton();
			sys.setGUISheet(editorGuiSheet);
			//getSingletonPtr()->getGUISystem()->setGUISheet(editorGuiSheet);
			editorGuiSheet->setVisible(true);
			editorGuiSheet->setMousePassThroughEnabled(true);

			flag = true;
		}
		else
		{	
			assert(0);
			
		}

		return flag;
	}

	void GUIManager::changeSize(float width, float height) 
	{ 
		// Update CEGUI window size
		//display range synchronous mouse to solve CEGUI mouse range is consistent with the scope of the system mouse.
		mpGUIRenderer->setDisplaySize(CEGUI::Size(width, height)); 

		// Ensure UI and display is redrawn
		// Described, when the window size is changed, UI maybe disappear, so we must call mpGUISystem-> signalRedraw ();
		// To repaint and display the UI
		CEGUI::System &guiSys = CEGUI::System::getSingleton();
		guiSys.signalRedraw();
	}

	void GUIManager::update( float timeSinceLastFrame )
	{
		CEGUI::System &sys = CEGUI::System::getSingleton();
		sys.injectTimePulse(timeSinceLastFrame / 1000);
	}

	void GUIManager::showGUICursor(bool enable)
	{
		if (enable)
			CEGUI::MouseCursor::getSingletonPtr()->show();
		else
			CEGUI::MouseCursor::getSingletonPtr()->hide();

		::ShowCursor(!enable);
	}

	void GUIManager::setCursorType(CURSOR_TYPE cursor)
	{
		CEGUI::System &guiSys = CEGUI::System::getSingleton();
		guiSys.setDefaultMouseCursor("NormalImageSet", MOUSE_CURSOR[cursor]);
	}

	void GUIManager::destoryWindow(const std::string &windowName)
	{
		getWindowManager()->destroyWindow(windowName);
	}

	void GUIManager::createImageset(const std::string& name, const std::string& filename)
	{
		CEGUI::ImagesetManager::getSingleton().createFromImageFile(name, filename);
	}



	void GUIManager::subscribeEvent(const CEGUI::String& windowName, const CEGUI::String &eventName, 
		const CEGUI::Event::Subscriber &subscriber)
	{
		CEGUI::WindowManager::getSingleton().getWindow(CEGUI::String(windowName))->subscribeEvent(eventName,subscriber);
	}

	void GUIManager::setProperty(const std::string &windowName, const std::string& PropName, const std::string& PropValue)
	{
		getWindow(windowName)->setProperty(PropName, PropValue);
	}

}
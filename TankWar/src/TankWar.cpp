/*
	-----------------------------------------------------------------------------
    ***********************************************************************
			filename:   TankWar.cpp
			created:    6/30/2014
			author:     Sun Chen
    ************************************************************************
    ************************************************************************
	-----------------------------------------------------------------------------
*/

#include "GameApp.h"
#include "windows.h"

using namespace Tank;

INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
{
	try {
		// Set up to support Chinese path
		setlocale(LC_ALL,"Chinese-simplified"); 
		GameApp *app = GameApp::getSingletonPtr();
		app->startGame();

	} catch( Ogre::Exception& e ) {
		MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", 
							 MB_OK | MB_ICONERROR | MB_TASKMODAL);
	}

	return 0;
}


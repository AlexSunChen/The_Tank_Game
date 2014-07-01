/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   GameState.cpp
			created:    6/29/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "GameState.h"
#include "GameApp.h"
#include "Util.h"
#include "GUIManager.h"
#include "ObjectManager.h"
#include "AudioManager.h"
#include "Item.h"
#include "MiniMap.h"
#include "MapManager.h"
#include "ContextApp.h"
#include "ChooseMapState.h"
#include "StateManager.h"

namespace Tank
{
	GameState::GameState() 
	 : mMove(0.0f, 0.0f, 0.0f), mMoveSpeed(0.025f), mRotateSpeed(0.0025f), mMouseState(BUTTON_UP)
		, mRotX(0), mRotY(0) , mRightButtonDown(false)
	{
		mMiniMap = NULL;
	}

	GameState::~GameState()
	{
	}

	
	void GameState::enter( void )
	{
		mRoot = Ogre::Root::getSingletonPtr();
		mSceneMgr = GameApp::getSingletonPtr()->getGameSceneMgr();
		mRenderWindow = GameApp::getSingletonPtr()->getRenderWindow();


		
		AudioManager::play(AUDIO_BACKGROUND, 0.5f, true, false);

		AudioManager::play(AUDIO_BEGIN);

		createCamera();
		createScene();
		createLight();
		createUI();
	}

	
	void GameState::exit( void )
	{
		AudioManager::stop(AUDIO_BACKGROUND , false);

		SAFE_DELETE(mChaseCamera);
		SAFE_DELETE(mMiniMap);
		ObjectManager::getSingletonPtr()->clear();

		mSceneMgr->clearScene();
		mSceneMgr->destroyAllCameras();
		mSceneMgr->destroyAllLights();
		mRenderWindow->removeAllViewports();

		GUIManager::destoryWindow("GameUI");
		GUIManager::destoryWindow("WinnerUI");
		GUIManager::destoryWindow("LosterUI");

	}


	void GameState::pause( void )
	{
	}


	void GameState::resume( void )
	{
	}

	void GameState::createUI()
	{
		GUIManager::loadWindow("GameUI");

		GUIManager::setCursorType(CURSOR_FIRE);

		mMiniMap = new MiniMap(mSceneMgr, mPlayerTank->getBodyNode(), Ogre::Vector3(0, 30, 100));
		ContextApp::getSingletonPtr()->mMiniMap = mMiniMap;
		ContextApp::updateScores();
	}

	void GameState::createScene()
	{
		mSceneMgr->setSkyBox(true, "Examples/MorningSkyBox");

		ObjectManager *objectMgr = ObjectManager::getSingletonPtr();
		MapManager* mapMgr = MapManager::getSingletonPtr();

		mapMgr->loadMap(ContextApp::getCurrentMapName());

		Ogre::Vector3 pos = mapMgr->getStartPosition(TANK_GREEN, 0);
		mPlayerTank = static_cast<GreenTank*>(objectMgr->createTank(TANK_GREEN, "Star", "Green", pos, 0.2f, 1.0f));

		Vector3 chaseCamPos(0, 33, -48);

		//! Models direction correction
		if (!mPlayerTank->isFrontZ())
		{
			chaseCamPos.z = -chaseCamPos.z ;
		}
		mChaseCamera = new ChaseCamera(mPlayerTank, mCamera, chaseCamPos );


		ContextApp::getSingletonPtr()->mChaseCamera = mChaseCamera;
		ContextApp::getSingletonPtr()->mPlayerA = mPlayerTank;

#if 0
		//! test
		Entity* ent = mSceneMgr->createEntity("axes.mesh");
		ent->setQueryFlags( 0  );
		SceneNode* axesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("center_axes");
		axesNode->attachObject(ent);
#endif

	}

	void GameState::createLight()
	{
		mSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
		Ogre::Light *light = mSceneMgr->createLight("MainLight");
		light->setPosition(Ogre::Vector3(50, 100, 50));
	}

	void GameState::createCamera()
	{
		mCamera	= mSceneMgr->createCamera( "GameState" );
		mViewport = mRenderWindow->addViewport(mCamera);
		mCamera->setAspectRatio((float)mViewport->getActualWidth() / (float)mViewport->getActualHeight());
		mCamera->setNearClipDistance(0.1);

		mViewport->setBackgroundColour(Ogre::ColourValue(0,0,0));
	}

	
	void GameState::update( float timeSinceLastFrame )
	{
		static ObjectManager *objectMgr = ObjectManager::getSingletonPtr();

		if (mChaseCamera)
			mChaseCamera->update(timeSinceLastFrame);

		if (mMiniMap) 
			mMiniMap->update(timeSinceLastFrame);

		// Update winning or losing information, updates various objects
		if (!ContextApp::updateWinner(timeSinceLastFrame))
		{
			objectMgr->update(timeSinceLastFrame);
		}
	}

	void GameState::changeSize(int width, int height)
	{
		// change Viewport
		Ogre::Viewport *vp = mCamera->getViewport();
		mCamera->setAspectRatio((Ogre::Real)(vp->getActualWidth()) / (Ogre::Real)(vp->getActualHeight()));
	}

	bool GameState::keyPressed( const OIS::KeyEvent &e )
	{ 
		if (e.key == OIS::KC_ESCAPE)
		{
			StateManager::getSingletonPtr()->changeState(ChooseMapState::getSingletonPtr());
			return true;
		}
		else if( e.key == OIS::KC_1 )
			mChaseCamera->shake(10, 20, 10);

		if (mPlayerTank)
			return mPlayerTank->keyPressed(e); 
		else
			return true;
	}

	bool GameState::keyReleased( const OIS::KeyEvent &e )
	{ 
		if (mPlayerTank)
			return mPlayerTank->keyReleased(e); 
		else
			return true;
	}

	bool GameState::mouseMoved(const OIS::MouseEvent &e)
	{ 
		if (mMouseState == RIGHT_BUTTON_DOWN)
		{
			mRotY = Ogre::Degree(e.state.Y.rel * mRotateSpeed);
			mChaseCamera->pitchGoal(mRotY);
		}

		if (e.state.Z.rel)
		{

			Real deta = -0.001f * e.state.Z.rel;
			mChaseCamera->wheelGoal( deta );
		}

		assert(mPlayerTank);
		mPlayerTank->mouseMoved(e); 

		return true;
	}


	bool GameState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
	{ 
		if (id == OIS::MB_Right)
			mMouseState = RIGHT_BUTTON_DOWN;

		if (mPlayerTank)
			return mPlayerTank->mousePressed(e, id); 
		else
			return true;
	}


	bool GameState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
	{ 
		mMouseState = BUTTON_UP;

		if (mPlayerTank)
			return mPlayerTank->mouseReleased(e, id); 
		else
			return true;
	}

	bool GameState::onWinnerWindowClick(const CEGUI::EventArgs& e)
	{
		StateManager::getSingletonPtr()->changeState(ChooseMapState::getSingletonPtr());
		return true;

	}

	bool GameState::onLoserWindowClick(const CEGUI::EventArgs& e)
	{
		StateManager::getSingletonPtr()->changeState(ChooseMapState::getSingletonPtr());
		return true;
	}


}
/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   MiniMap.cpp
			created:    6/28/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "MiniMap.h"
#include "ContextApp.h"
#include "Util.h"

#include <assert.h>

namespace Tank
{
	MiniMap::MiniMap(Ogre::SceneManager	*sceneMgr, Ogre::SceneNode *baseNode, const Ogre::Vector3 &heightOffset)
		: mSceneMgr(sceneMgr), mZoomFactor(1.0f), mBaseNode(baseNode),	mHeightOffset(heightOffset)
	{
		mMode = MINIMAP_OVERLOOK;

		createRadarMode();
		createOverLookMode("GameUI/SmallMap");

		setBaseNode(baseNode, heightOffset);

		onChangeMode(CEGUI::EventArgs());
		onChangeMode(CEGUI::EventArgs());
	}

	MiniMap::~MiniMap()
	{
		mSceneMgr->destroySceneNode(mCameraNode);
		mSceneMgr->destroyCamera(mCamera);

		CEGUI::ImagesetManager::getSingleton().destroy("RttImageset");
		Ogre::OverlayManager* overlays = Ogre::OverlayManager::getSingletonPtr();
		overlays->destroyOverlayElement(mOverlayRadar);
		overlays->destroy(mOverlays);
	}

	void MiniMap::update(float timeSinceLastFrame)
	{
		if (mMode == MINIMAP_OVERLOOK)
			updateOverLook(timeSinceLastFrame);
	}

	void MiniMap::updateOverLook(float timeSinceLastFrame)
	{
		mCameraNode->setPosition(mBaseNode->_getDerivedPosition() + mZoomFactor * mHeightOffset);
	}

	void MiniMap::addPoint(const std::string &name)
	{
		static Ogre::OverlayManager *overlayMgr = Ogre::OverlayManager::getSingletonPtr();
		Ogre::OverlayElement* tank = overlayMgr->createOverlayElement("Panel", name);
		{
			tank->setMetricsMode(Ogre::GMM_PIXELS);	//
			tank->setDimensions(RADAR_ENMEY_SIZE, RADAR_ENMEY_SIZE);
			tank->setMaterialName("TankWar/Radar/Enemy");

			mOverlayRadar->addChild(tank);
			mEnemys.insert(std::make_pair(tank->getName(), tank));
		}
	}

	void MiniMap::removePoint(const std::string &name)
	{
		Ogre::OverlayManager::getSingletonPtr()->destroyOverlayElement(mEnemys[name]);
		mEnemys.erase(name);
	}

	void MiniMap::updatePoint(const std::string &name, const Ogre::Vector3& pos)
	{
		static float X = RADAR_POS_X + RADAR_SIZE / 2 - 19;
		static float Y = RADAR_POS_Y + RADAR_SIZE / 2 - 19;
		static float RADIAN_270 = 3.1415926f * 1.5f;

		if (mMode == MINIMAP_OVERLOOK) return;

		BaseTank* player = ContextApp::getPlayerA();
		if (player)
		{
			Ogre::Vector3 fireDirection = player->getFireDirection();
			Ogre::Vector3 enemyDireciont = pos - player->getPosition();
			float length = enemyDireciont.length();

			if (length < RADAR_MAX_RADIUS)
			{
				mEnemys[name]->show();
				
				float radius = RADAR_RADIUS * length / RADAR_MAX_RADIUS;
				
				Ogre::Radian radian = fireDirection.angleBetween(enemyDireciont);
				if (fireDirection.crossProduct(enemyDireciont).y > 0)
					radian = -radian;

				
				float rad = radian.valueRadians() + RADIAN_270;
				mEnemys[name]->setPosition(X + radius * cos(rad), Y + radius * sin(rad));
			}
			else
				mEnemys[name]->hide();
		}
	}

	
	bool MiniMap::onZoomIn(const CEGUI::EventArgs& e) 
	{ 
		if (mZoomFactor > 0.5f) 
			mZoomFactor -= 0.1f;
		return true;
	}

	bool MiniMap::onZoomOut(const CEGUI::EventArgs& e) 
	{
		if (mZoomFactor < 2.5f) 
			mZoomFactor += 0.1f; 
		return true;
	}

	
	bool MiniMap::onChangeMode(const CEGUI::EventArgs& e) 
	{ 
		mMode = (mMode == MINIMAP_OVERLOOK) ? MINIMAP_RADAR : MINIMAP_OVERLOOK;

		showOverLookMode(mMode == MINIMAP_OVERLOOK);
		showRadarMode(mMode == MINIMAP_RADAR);

		return true;
	}

	void MiniMap::createOverLookMode(const std::string &windowName)
	{
		Ogre::TexturePtr tex = Ogre::TextureManager::getSingleton().createManual("RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME
			, Ogre::TEX_TYPE_2D, 512, 512, 1, 0, Ogre::PF_R8G8B8, Ogre::TU_RENDERTARGET);
		
		mCamera = mSceneMgr->createCamera("RttCam");
		mCamera->setNearClipDistance(0.1f);

		Ogre::Viewport *v = tex->getBuffer()->getRenderTarget()->addViewport( mCamera );
		v->setOverlaysEnabled(false);
		v->setClearEveryFrame( true );
		v->setBackgroundColour( Ogre::ColourValue::Black );
		

		CEGUI::Texture &guiTex = GUIManager::getSingletonPtr()->getGUIRender()->createTexture(tex);

		//! CEGUI works with image sets instead of individual images. 
		CEGUI::Imageset &imageSet =	CEGUI::ImagesetManager::getSingleton().create("RttImageset", guiTex);
		imageSet.defineImage("RttImage", CEGUI::Point(0.0f, 0.0f),CEGUI::Size(guiTex.getSize().d_width, guiTex.getSize().d_height)
			,CEGUI::Point(0.0f, 0.0f));

		GUIManager::setProperty(windowName, "Image", "set:RttImageset image:RttImage");


		subscribeEvents();
	}

	void MiniMap::createRadarMode()
	{
		Ogre::OverlayManager *overlayMgr = Ogre::OverlayManager::getSingletonPtr();

		mOverlayRadar = static_cast<Ogre::OverlayContainer*>(overlayMgr->createOverlayElement("Panel", "TankRadar"));
		mOverlayRadar->setMetricsMode(Ogre::GMM_PIXELS);	
		mOverlayRadar->setPosition(RADAR_POS_X, RADAR_POS_Y);
		mOverlayRadar->setDimensions(RADAR_SIZE, RADAR_SIZE);
		mOverlayRadar->setMaterialName("TankWar/Radar"); 

		mOverlays = Ogre::OverlayManager::getSingletonPtr()->create("TankRadarOverlay");
		mOverlays->setZOrder(650);
		mOverlays->add2D(mOverlayRadar);
		mOverlays->show();
	}

	void MiniMap::subscribeEvents()
	{
		GUIManager::subscribeEvent("GameUI/Add", CEGUI::DefaultWindow::EventMouseButtonDown , CEGUI::Event::Subscriber(&MiniMap::onZoomIn, this));
		GUIManager::subscribeEvent("GameUI/Sub", CEGUI::DefaultWindow::EventMouseButtonDown , CEGUI::Event::Subscriber(&MiniMap::onZoomOut, this));
		GUIManager::subscribeEvent("GameUI/Mode",CEGUI::DefaultWindow::EventMouseButtonDown , CEGUI::Event::Subscriber(&MiniMap::onChangeMode, this));

		GUIManager::getWindow("GameUI/Add")->setWantsMultiClickEvents(false);
		GUIManager::getWindow("GameUI/Sub")->setWantsMultiClickEvents(false);
		GUIManager::getWindow("GameUI/Mode")->setWantsMultiClickEvents(false);
	}


	void MiniMap::setBaseNode(Ogre::SceneNode *baseNode, const Ogre::Vector3 &heightOffset)
	{
		mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
		mCameraNode->setPosition(mBaseNode->_getDerivedPosition() + heightOffset);

		mCameraNode->attachObject(mCamera);
		mCamera->setPosition(Ogre::Vector3(0, 0, 0));
		mCameraNode->lookAt(mBaseNode->_getDerivedPosition(), Ogre::Node::TS_WORLD);
	}


	void MiniMap::setVisible(bool visible) 
	{ 
		mOverlays->hide(); 
		GUIManager::getWindow("GameUI/SmallMap")->setVisible(visible);
		GUIManager::getWindow("GameUI/Add")->setVisible(visible);
		GUIManager::getWindow("GameUI/Sub")->setVisible(visible);
	}


	void MiniMap::showOverLookMode(bool show)
	{
		mCameraNode->setVisible(show);
		mCamera->setVisible(show);
		GUIManager::getWindow("GameUI/SmallMap")->setVisible(show);
		GUIManager::getWindow("GameUI/Add")->setVisible(show);
		GUIManager::getWindow("GameUI/Sub")->setVisible(show);
	}


	void MiniMap::showRadarMode(bool show)
	{
		if (show) 
		{
			mOverlays->setZOrder(650);
			mOverlays->show();
		}
		else
			mOverlays->hide();
	}
	
}
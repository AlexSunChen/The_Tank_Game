/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   MiniMap.h
			created:    6/28/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#pragma once

#include <Ogre.h>
#include "GUIManager.h"

namespace Tank
{
	// Rader size MINI_MAP_SIZE * MINI_MAP_SIZE
	const static float RADAR_SIZE = 145;

	// The initial X coordinate pixel radar screen
	const static float RADAR_POS_X = 10;

	// The initial Y coordinate pixel radar screen
	const static float RADAR_POS_Y = 10;

	// Size enemy in radar
	const static float RADAR_ENMEY_SIZE = 15;

	// The maximum range of the radar can detect
	const static float RADAR_MAX_RADIUS = 50;

	// Radar display radius
	const static float RADAR_RADIUS = RADAR_SIZE / 2;

	// Radar display radius squared
	const static float RADAR_RADIUS_POW_2 = RADAR_RADIUS * RADAR_RADIUS;

	// Rader mode
	enum MINIMAP_MODE
	{
		MINIMAP_OVERLOOK,		// Bird view display
		MINIMAP_RADAR,			// Radar Display
	};

	class MiniMap
	{
	public:
		MiniMap(Ogre::SceneManager	*sceneMgr, Ogre::SceneNode *baseNode, const Ogre::Vector3 &heightOffset);

		~MiniMap();

		void update(float timeSinceLastFrame);

		// Add the enemy's radar node
		void addPoint(const std::string &name);
		// Remove the enemy's radar node
		void removePoint(const std::string &name);
		// With a new enemy in the radar coordinates
		void updatePoint(const std::string &name, const Ogre::Vector3& pos);

		// Small map is visible
		void setVisible(bool visible);

	private:
		// Small Scale Map
		bool onZoomIn(const CEGUI::EventArgs& e) ;

		// Enlarge map
		bool onZoomOut(const CEGUI::EventArgs& e) ;

		// Small map display mode change
		bool onChangeMode(const CEGUI::EventArgs& e) ;

		// Overlook mode
		void createOverLookMode(const std::string &windowName);
		// Rader mode
		void createRadarMode();

		void updateOverLook(float timeSinceLastFrame);

		void subscribeEvents();

		void setBaseNode(Ogre::SceneNode *baseNode, const Ogre::Vector3 &heightOffset);
		void showOverLookMode(bool show);
		void showRadarMode(bool show);

	private:
		Ogre::SceneManager			*mSceneMgr;
		Ogre::SceneNode				*mBaseNode;
		Ogre::SceneNode				*mCameraNode;

		Ogre::Camera						*mCamera;
		Ogre::Vector3						mHeightOffset;

		float										mZoomFactor;

		Ogre::Overlay						*mOverlays;

		Ogre::OverlayContainer	*mOverlayRadar;
		std::map<std::string, Ogre::OverlayElement*>	mEnemys;

		MINIMAP_MODE				mMode;
	};
}
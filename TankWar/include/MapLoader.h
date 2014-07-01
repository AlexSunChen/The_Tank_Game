/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   MapLoader.h
				created:    5/28/2012
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include <string>
#include "tinyxml.h"
#include "ObjectManager.h"
#include "MapManager.h"

namespace Tank
{
	using namespace TinyXml;
	/**
		Map loading classes, to achieve load the terrain file. Map 
	*/
	class MapLoader  
	{
	public:
		/**
			@filename	Terrain file name(*.map)
			@objectFactory	Scene Object Manager
		*/
		MapLoader(const std::string &filename, ObjectManager* objectFactory);
		virtual ~MapLoader();

		// Start loading maps, loaded successfully returns true, and false failed to load
		bool loadMap();
		// Get maps file name
		std::string getFileName() const { return mFilename; }
		// Obtain basic information topography
		MapInfo	getMapInfo() { return mMapInfo; }

		// Whether an error occurred while reading
		bool hasError() const { return mHasError; }
		std::string getErrorMessage() const { return mErrorMessage; }

	private:
		// Load map configuration
		bool loadConfig(TiXmlElement* config);
	
		bool loadTank(TiXmlElement* tank);
		
		bool loadNode(TiXmlElement* node);

	private:
		ObjectManager	*mObjectFactory;
		MapInfo			mMapInfo;

		bool					mHasError;
		
		std::string			mFilename;
		std::string			mErrorMessage;
	};
}
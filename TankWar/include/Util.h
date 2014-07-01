/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   Util.h
				created:    5/19/2012
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#ifndef __UTIL_H__
#define __UTIL_H__

#include <Ogre.h>
#include "Config.h"

namespace Tank
{

	// Safely Remove pointer macro definition
	#define SAFE_DELETE(point) \
	{											\
		if (point)							\
			delete point;					\
		point = 0;							\
	}

	// Load Resource Type
	enum ResType
	{
		FILE_TYPE,	// Load ordinary directory of resources
		ZIP_TYPE,		// Zip file to load the resource
	};

	// The root of resource folder
	const static std::string TANKWAR_RES_ROOT_PATH = "../media/";

	/**
		Auxiliary math
	*/
	class Util
	{
	public:

		/**
			The string is converted into a floating-point type digital
			@num Numeric string to convert
		*/
		inline static float toFloat(const std::string&num) 
		{ 
			return atof(num.c_str());
		}

		/**
		    Converted into a string in the form of a floating-point type
			@num Value to be converted
		*/
		inline static std::string toString(float num)
		{
			return Ogre::StringConverter::toString(num);
		}

		// Random truth value
		inline static bool randomBool() { return ((int)Ogre::Math::RangeRandom(0, 2)) == 1; }
		
		// Random number range
		inline static float randomRange(float low, float heigh) { return Ogre::Math::RangeRandom(low, heigh); }

		// record log
		static void log(const std::string &msg)
		{
			Ogre::LogManager::getSingletonPtr()->getDefaultLog()->logMessage(msg);
		}

		/**
			For a given digital format
			exp: format(1234, 6) = 001234
			@num	To format numbers
			@digit	To format the median
		*/
		static std::string format(int num, int digit);

		// Eliminating gaps on both sides of the string
		static std::string trim(const std::string &line);

		// Compare strings are equal without regard to whether the case
		static bool equal(const std::string &left, const std::string &right);

		/**
			Manually: load resources
			@path: Resoure's path
			@type: Type of Resource
			@groupName:	Resource group resource is located
			@load:	Is immediately loaded
			@recursive:	Whether subfolders recursively search
		*/
		static void loadResouse(const std::string &path, ResType type, 
													 const std::string &groupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, 
													 bool load = true, bool recursive = false);

		/**
			Unloaded: Resoure
			@groupName:	Resource group you want to uninstall
		*/
		static void unloadResouse(const std::string &GroupName);

		/**
			Parsing. Scene file and generate the corresponding scene
			before calling the need to ensure the use of loadResouse () to load. Scene resources
			@sceneName		.sceneFile Name
			@groupName		.sceneResource group resource is located
			@yourSceneMgr	sceneManager
			@pAttachNode		Create a root node of the scene, if it is NULL places to build root SceneNode
		*/
		static void parseDotScene(const std::string &sceneName, 	Ogre::SceneManager *sceneMgr,  
												const std::string &groupName = Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
												Ogre::SceneNode *rootNode = NULL);
	};
}

#endif
/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   Util.cpp
				created:    5/19/2012
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "Util.h"
#include "OgreMaxScene.hpp"

namespace Tank
{
	
	const static std::string BLOCK = " \t";



	std::string Util::format(int num, int digit)
	{
		std::string str = toString(num);
		if (digit > (int)str.length())
		{
			int length = digit - (int)str.length();
			for (int i = 0; i < length; i++)
				str = "0" + str;
		}

		return str;
	}



	std::string Util::trim(const std::string &line)
	{
		std::string result;
		std::size_t index = line.find_first_not_of(BLOCK);
		if (index != std::string::npos)
			result = line.substr(index);

		index = result.find_last_not_of(BLOCK);
		if (index != std::string::npos && index != result.size() - 1)
			result = result.substr(0, index + 1);

		return result;
	}



	// Determine whether the string is equal, ignoring case
	bool Util::equal(const std::string &left, const std::string &right)
	{
		if (left.size() != right.size())
			return false;

		for (unsigned int i = 0; i < left.size(); i++)
			if (tolower(left[i]) != tolower(right[i]))
				return false;

		return true;
	}



	void Util::loadResouse(const std::string &path, ResType type, const std::string &groupName, bool load, bool recursive)
	{
		std::string ty = (type == ZIP_TYPE ? "Zip" : "FileSystem");

		//Manually load resources
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(path,ty,groupName,recursive);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(groupName);

		if(load)	// All need to be loaded directly
			Ogre::ResourceGroupManager::getSingleton().loadResourceGroup(groupName);
	}



	//unload resourse
	void Util::unloadResouse(const std::string &GroupName)
	{
		Ogre::ResourceGroupManager::getSingleton().destroyResourceGroup(GroupName);
	}



	// Parsing. Scene file and generate the corresponding scene
	//before calling the need to ensure the use of loadResouse () to load. Scene resources
	// sceneName .scene: File Name, groupName:Resource Group, pAttachNode = NULL:create by root 
	void Util::parseDotScene(const std::string &sceneName, Ogre::SceneManager *sceneMgr,  
											const std::string &groupName, Ogre::SceneNode *rootNode)
	{
		OgreMax::OgreMaxScene scene;
		scene.Load(sceneName, 0, 0, sceneMgr, rootNode, 0, groupName);
	}
}
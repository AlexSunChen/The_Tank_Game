/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   StaticObject.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include <Ogre.h>
#include <string>
#include "Object.h"

namespace Tank
{
	class StaticObject : public Object
	{
	public:
		StaticObject(Ogre::SceneManager* sceneMgr, const std::string &name, const std::string &meshName);
		StaticObject();

		bool onHit(Attacker *attacker);
	};
}
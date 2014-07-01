
/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   IFactory.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include "Object.h"

namespace Tank
{
	/**
		factory interface
	*/
	class IFactory : public IFrameListener
	{
	
		virtual void update( float timeSinceLastFrame ) {}

	
		virtual Object* get(const std::string &name) = 0;

	
		virtual bool destroy(const std::string &name) = 0;
	};
}
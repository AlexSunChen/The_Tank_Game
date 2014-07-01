/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   State.cpp
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "State.h"
#include "StateManager.h"

namespace Tank
{


	void State::changeState( State *newState )
	{
		StateManager::getSingletonPtr()->changeState(newState);
	}



	void State::pushState( State *newState )
	{
		StateManager::getSingletonPtr()->pushState(newState);
	}



	void State::popState( void )
	{
		StateManager::getSingletonPtr()->popState();
	}


}
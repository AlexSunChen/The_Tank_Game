/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   StateManager.cpp
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#include "StateManager.h"
#include "Util.h"

namespace Tank
{


	StateManager::~StateManager()
	{

	}



	void StateManager::destroy()
	{
		/**
		Note: For vector for content changes can cause invalid iterators may cause crashes
		*/
		while (!empty())
			popState();

		mStates.clear();
	}



	void StateManager::changeState( State *newState )
	{

		if (!mStates.empty())
		{
			mStates.back()->exit();
			mStates.pop_back();
		}

		mStates.push_back(newState);
		newState->enter();
	}



	void StateManager::pushState( State *newState )
	{
		
		if (!mStates.empty())
			mStates.back()->pause();

		mStates.push_back(newState);
		newState->enter();
	}



	void StateManager::popState( void )
	{
		
		if (!mStates.empty())
		{
			mStates.back()->exit();
			mStates.pop_back();
		}

		if (!mStates.empty())
			mStates.back()->resume();
	}

	State* StateManager::getCurrentState()
	{
		if (!mStates.empty())
		{
			return mStates.back();;
		}
		else
		{
			return NULL;
		}


	}



}
/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   State.h
				created:    6/10/2012
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include <Ogre.h>
#include "Common.h"

namespace Tank
{
	/*
		Gameplay base class
		Other states, such as menu state game, the game state etc. 
		which need to inherit and implement specific operations.
	*/
	class State : public IFrameListener, public IKeyMouseListener
	{
	public:
		virtual ~State(){}

		// enter status
		virtual void enter( void )  = 0;
		// exit status
		virtual void exit( void )   = 0;

		// pause status
		virtual void pause( void ) {}
		// resume status
		virtual void resume( void ) {}
		
		/**
			After the call to change the window size, 
			the state is responsible for the handling of the window changes
			@width	Render window width After change the window size
			@height	Render window height After change the window size
		*/
		virtual void changeSize(int width, int height) = 0;

		/** 
			Replace the state, the state will pop the stack, push this state
			@newState 
		*/
		void changeState( State *newState );

		/**
			the state of push stack, firstly pause() the stack,then push
			@newState	
		*/
		void pushState( State *newState );

		void popState( void );

	protected:
		State(){}
		State(const State&);
		State& operator=(const State&);
	};
}
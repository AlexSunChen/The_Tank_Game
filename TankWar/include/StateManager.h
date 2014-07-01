/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   StateManager.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#ifndef __STATESYSTEM_H__
#define __STATESYSTEM_H__

#include <vector>
#include <assert.h>

#include "State.h"
#include "SingletonT.h"

namespace Tank
{
	/**
	Status Manager for managing and updating all the states.
	*/
	class StateManager : public State, public SingletonT<StateManager>
	{
		SINGLETON_DEFINE(StateManager)

	public:
		virtual ~StateManager();

		
		void destroy();
		// Replace the state, the state will pop the stack, push this state
		void changeState( State *newState );
		// the state of push stack, firstly pause() the stack,then push
		void pushState( State *newState );
		
		void popState( void );

		// Get Top state
		inline State* peekTop() 
		{ 
			assert(!mStates.empty()); 
			return mStates.back(); 
		}

		// The current stack size
		inline int size() const { return (int)mStates.size(); }
		inline bool empty() const { return mStates.empty(); }

	public:
		// State may be self-contained
		inline void update( float timeSinceLastFrame ) { peekTop()->update(timeSinceLastFrame); }
		// After rendering update status
		inline void updateAfterRender( float timeSinceLastFrame) { peekTop()->updateAfterRender(timeSinceLastFrame); }

		inline bool keyPressed( const OIS::KeyEvent &e ){ return peekTop()->keyPressed(e); }
		inline bool keyReleased( const OIS::KeyEvent &e ){ return peekTop()->keyReleased(e); }

		inline bool mouseMoved(const OIS::MouseEvent &e){ return peekTop()->mouseMoved(e); }
		inline bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {return peekTop()->mousePressed(e, id); 	}
		inline bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ){ return peekTop()->mouseReleased(e, id);}

		// After the call to change the window size, the state is responsible for the handling of the window changes
		inline void changeSize(int width, int height) { peekTop()->changeSize(width, height); }

		virtual void enter( void ){}
		
		virtual void exit( void ) {}

		State* getCurrentState();



	protected:
		StateManager(){}

		std::vector<State*> mStates;
	};
}

#endif
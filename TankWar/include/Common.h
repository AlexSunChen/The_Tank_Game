/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   Common.h
				created:    6/28/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include <Ogre.h>
#include <OIS/OIS.h>

namespace Tank
{
	class IFrameListener
	{
	public:
		/**
			Status updates, calling each frame
			@timeSinceLastFrame	:Since the last time (based s)
		*/
		virtual void update( float timeSinceLastFrame ) = 0;

		/**
			Update status after rendering each frame call
			@timeSinceLastFrame	
		*/
		virtual void updateAfterRender( float timeSinceLastFrame) {}
	};

	
	class IKeyMouseListener : public OIS::MouseListener, public OIS::KeyListener
	{
	public:
	
		virtual bool keyPressed( const OIS::KeyEvent &e ){ return true; }
		virtual bool keyReleased( const OIS::KeyEvent &e ){ return true; }

		virtual bool mouseMoved(const OIS::MouseEvent &e){ return true; }
		virtual bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ){ return true; }
		virtual bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ){ return true; }
	};
}
/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   GreenTank.h
			created:    6/29/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#pragma once

#include "Trail.h"
#include "BaseTank.h"
#include "Common.h"
#include <OIS/OIS.h>

using namespace Ogre;

namespace Tank
{

	class GreenTank : public BaseTank, public IKeyMouseListener
	{
	public:
		
		GreenTank(const std::string &name, const std::string &meshName, const Ogre::Vector3 &pos = Ogre::Vector3::ZERO
			, float speed = 2, float damage = 20, float heightOffset = 0.3f, float fireHeightOffset = 1.0f);

		virtual ~GreenTank();

		void update(float timeSinceLastFrame);

		void fire();

		bool onHit(Attacker *attacker);

	
		void setDrawTrails(bool enable);
		bool isDrawTrails() const ;

	
		void resetPosition();

		bool keyPressed( const OIS::KeyEvent &e );
		bool keyReleased( const OIS::KeyEvent &e );

		bool mouseMoved(const OIS::MouseEvent &e);
		bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
		bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) { return true; }

	protected:
		
		bool checkCollision(const Ogre::Vector3 &basePos, const Ogre::Vector3 &destPositon);


		void updateFireDirection(float timeSinceLastFrame);


		void beginDrawTrails();

		void endDrawTrails();
	
		void updateTrails(float timeSinceLastFrame);

	protected:
		Ogre::Vector2 mMousePos;	

		Trail *mLeftTrails;				
		Trail *mRightTrails;		

		Ogre::Quaternion mResetOrientation;
		Ogre::Vector3 mResetPosition;

		Vector3 mKeyDirection;      // player's local intended direction based on WASD keys

		bool getScreenspaceCoords(Ogre::MovableObject* object, Ogre::Camera* camera, Ogre::Vector2&result);

	public:
		const Vector3& getTankFront();


	};
}
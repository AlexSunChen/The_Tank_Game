/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   ChaseCamera.h
			created:    6/28/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#pragma once

#include <Ogre.h>
#include "GreenTank.h"
#include "Common.h"

namespace Tank
{
	enum CAMERATYPE
	{
		CAMERA_FIX,
		CAMERA_AUTO,
	};

	/**
	Implement a simple buffer camera package, is used to synchronize Ogre :: Camera and BaseTank position
	And the effect of the buffer with the camera.
	*/
	class ChaseCamera : public IFrameListener
	{
	public:
		/**	
		@tank		The camera needs to bind to Tank
		@camera	
		@offset	
		*/
		ChaseCamera(GreenTank* tank, Ogre::Camera *camera, const Ogre::Vector3 &offset = Ogre::Vector3(0, 15, 25));

		~ChaseCamera();
		/**
		Status updates, calling each frame
		@timeSinceLastFrame	
		*/
		void update(float timeSinceLastFrame);

		// Set the camera buffer coefficient, the smaller the coefficient  the slower moving smoother
		void setMoveFactor(float moveFactor) { mMoveFactor = moveFactor; }

		/** 
		Jitter camera
		@shakeTime 
		@shakeRight	The maximum distance shake the camera moves to the right, to the left as-shakeRight
		@shakeUp		The maximum distance the camera moves shake up, if it is -1, it means not to shake down
		*/
		void shake(float shakTime = 10.0f, float shakeRight = 10.0f, float shakeUp = 10.0f);

		void pitchGoal(Ogre::Degree degree);

		void wheelGoal(Ogre::Real deltaZoom);

	private:
		
		void updateShake(float timeSinceLastFrame);

	private:
		GreenTank						*mOwner;
		Ogre::SceneManager		*mSceneManager;

		Ogre::Camera					*mCamera;		

		Ogre::SceneNode			*mPivotNode;	
		Ogre::SceneNode			*mGoalNode;		
		Ogre::SceneNode			*mCameraNode;	

		float									mMoveFactor;		

		float									mShakeTime;		
		float									mCurrentShakeTime;	
		float									mShakeRight;		
		float									mShakeUp;			
		Ogre::Vector3					mShakeOffset;		
	};
}
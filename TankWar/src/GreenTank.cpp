/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   GreenTank.cpp
			created:    6/29/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "GreenTank.h"
#include "GameApp.h"
#include "MapManager.h"
#include "ObjectManager.h"
#include "ContextApp.h"
#include <iostream>
using namespace std;

namespace Tank
{
	GreenTank::GreenTank(const std::string &name, const std::string &meshName, const Ogre::Vector3 &pos, 
		float speed, float damage, float heightOffset, float fireHeightOffset)
		: BaseTank(TANK_GREEN, name, meshName, pos, damage, heightOffset, fireHeightOffset), 
		mMousePos(0, 0)
	{
		mLife = 400;
		mDefense = 2;
		mSpeed = 1.25f;

		mLeftTrails = new Trail(mSceneManager, "tiretrial.mesh", "TireTrialMaterial", 5.0f);
		mRightTrails = new Trail(mSceneManager, "tiretrial.mesh", "TireTrialMaterial", 5.0f);

	
		mFireNode->setFixedYawAxis(true);

	
		mBodyEntity->setQueryFlags(COLLSION_PASS);
		mFireEntity->setQueryFlags(COLLSION_COLLISION);

		mResetPosition = getPosition();
		mResetOrientation = mBodyNode->getOrientation();

		mKeyDirection = Vector3::ZERO;

		//Quaternion qua(Degree(180) , Vector3::UNIT_Y );
		//mBodyNode->rotate(qua);


		//! 
		mFrontZ = false;
	}

	GreenTank::~GreenTank()
	{
		SAFE_DELETE(mLeftTrails);
		SAFE_DELETE(mRightTrails);
	}

	void GreenTank::resetPosition()
	{
		setPosition(mResetPosition);
		setOrientation(mResetOrientation);

		mPreMove = MOVE_UP;
		mMove = Ogre::Vector3(0,0,0);
	}

	bool GreenTank::checkCollision(const Ogre::Vector3 &basePos, const Ogre::Vector3 &destPositon )
	{
	
		static Ogre::Vector3 size = getBoundingBox().getHalfSize();
		static int COLLISION_MASK = COLLSION_COLLISION | COLLSION_ITEM;
		static ObjectManager* objectMgr = ObjectManager::getSingletonPtr();


		Vector3 direction = destPositon - basePos;
		Ogre::Vector3 offset = direction.crossProduct(Ogre::Vector3::UNIT_Y) * size;

		direction.normalise();
		Ogre::Vector3 toPos= destPositon + direction * size;

		Object* object = objectMgr->collision( basePos ,toPos, 0.5f, COLLISION_MASK);
		if (!object) object = objectMgr->collision(basePos + offset , toPos + offset, 0.5f, COLLISION_MASK);
		if (!object) object = objectMgr->collision(basePos - offset , toPos - offset, 0.5f, COLLISION_MASK);

		if (object && object != this)
		{

			if (object->getObjectItemType() == OBJECT_ITEM)
			{
				object->onHit(this);
			}

			return true;
		}
		else
		{

			object = objectMgr->collisionAABB(basePos , destPositon, COLLSION_ITEM);
			if (object) object->onHit(this);
			return false;
		}
	}

	//! second
	void GreenTank::update(float deltaTime)
	{

		//deltaTime *= 0.001f;
		mFireCD += deltaTime * 0.01f;

		Vector3 tankFront = mBodyNode->getOrientation().zAxis();
		Vector3 tankLeft = mBodyNode->getOrientation().xAxis();


		if (!mFrontZ)
		{
			tankFront*=-1.0f;
			tankLeft*=-1.0f;
		}

		Vector3 mGoalDirection;     // actual intended direction in world-space
		mGoalDirection = Vector3::ZERO;
		mGoalDirection += mKeyDirection.z * tankFront ;
		mGoalDirection += mKeyDirection.x * tankLeft ;
		mGoalDirection.y = 0;
		mGoalDirection.normalise();


		if( mKeyDirection.x != 0)
		{

			Quaternion toGoal = tankFront.getRotationTo(mGoalDirection);
			// calculate how much the character has to turn to face goal direction
			Real yawToGoal = toGoal.getYaw().valueDegrees();
			// this is how much the character CAN turn this frame
			Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * deltaTime * 50.0f* 0.003f;
			// reduce "turnability" if we're in midair

			// turn as much as we can, but not more than we need to
			if (yawToGoal < 0) yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, yawAtSpeed, 0);
			else if (yawToGoal > 0) yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, 0, yawAtSpeed);

			mBodyNode->yaw(Degree(yawToGoal));
		}

		if (mKeyDirection.z != 0)
		{
			Ogre::Vector3 oldPos = getPosition();

			mMove = Vector3::UNIT_Z * mKeyDirection.z*0.01;
			////! 模型修正
			if (!mFrontZ)
			{
				mMove *= Vector3::NEGATIVE_UNIT_Z ;
			}

			Ogre::Vector3 move = mSpeed * mMove * deltaTime;

			mBodyNode->translate(move ,Node::TS_LOCAL);

			Vector3 newPostion =  mBodyNode->getPosition();


			if (checkCollision(oldPos, newPostion))
			{
				mBodyNode->translate(-move ,Node::TS_LOCAL);
			}
			else
			{
				mFireNode->setPosition(newPostion);
			}

		}

	
		if (mDamaged && mCurrentLife < mLife * 0.5f)
		{
			mDamaged = false;
			mSmokeNode->detachObject(mSmokeParticle);
		}

		updateFireDirection(deltaTime *10);
	
		updateTrails(deltaTime *10);
	}

	void GreenTank::updateTrails(float timeSinceLastFrame)
	{
	
		static bool drawTrails = true;		
		static float flushTime = 0;	
		static float flushTotalTime = 50.0f;

		//if(!mMoveUp) return;

		flushTime += timeSinceLastFrame;
		if (flushTime > flushTotalTime)
		{
			flushTime = 0;
			flushTotalTime = Util::randomRange(50, 100);
			if (drawTrails) 
				endDrawTrails();
			else
			{
				//if (mMoveUp)	
				//	beginDrawTrails();
			}
			drawTrails = !drawTrails;
		}

	
		static Ogre::Vector3 size = getBoundingBox().getHalfSize() * 0.5f;
		static float heightOffset = size.y * 1.75f;
		if (drawTrails && mLeftTrails->isDraw() && mRotateBody == 0)
		{
			Ogre::Vector3 offset = mMove.crossProduct(Ogre::Vector3::UNIT_Y) * size;
			Ogre::Vector3 back = mMove * size;
			Ogre::Vector3 pos = getPosition();
			pos.y -= heightOffset;	


			mLeftTrails->addOneTrail(pos - offset);
			mRightTrails->addOneTrail(pos + offset);
		}

		mRightTrails->update(timeSinceLastFrame);
		mLeftTrails->update(timeSinceLastFrame);
	}

	void GreenTank::updateFireDirection(float deltaTime)
	{

		static ObjectManager* objectMgr = ObjectManager::getSingletonPtr();

		Ogre::Vector3 pick = objectMgr->collisionFromScreen(mMousePos);
		Ogre::Vector3 firePostion = getFirePosition();

		Ogre::Vector3 goalDirection = pick - firePostion;
		goalDirection.y = 0;

		ostringstream oss;
		//oss<<std::fixed << std::setprecision(1);
		//oss<<" pick: "<<pick;
		//oss<<" firePostion:"<<firePostion;
		//oss<<" goaldirection: "<<goalDirection;


		if (goalDirection.length() > 4.0f) 
		{
			Vector3 fireFront = mFireNode->getOrientation().zAxis() ;
			//! 模型修正
			if (!mFrontZ) fireFront*= -1.0f;

			Quaternion toGoal = fireFront.getRotationTo(goalDirection);

			// calculate how much the character has to turn to face goal direction
			Real yawToGoal = toGoal.getYaw().valueDegrees();
			// this is how much the character CAN turn this frame
			Real yawAtSpeed = yawToGoal / Math::Abs(yawToGoal) * deltaTime * 50.0f* 0.001f;
			// reduce "turnability" if we're in midair
			// turn as much as we can, but not more than we need to
			if (yawToGoal < 0) yawToGoal = std::min<Real>(0, std::max<Real>(yawToGoal, yawAtSpeed));
			else if (yawToGoal > 0) yawToGoal = std::max<Real>(0, std::min<Real>(yawToGoal, yawAtSpeed));

			mFireNode->yaw(Degree(yawToGoal));

		}

		GameApp::OutputDebugMessage( oss.str() );

	}

	bool GreenTank::onHit(Attacker *attacker)
	{
		float attack = attacker->getDamageValue() ;
		float damage = attack * 5 - mDefense * 5;

		if (damage<1)
		{
			damage = 1;
		}

		mCurrentLife +=  damage;
		if (!mDamaged && mCurrentLife > mLife * 0.5f)
		{
			mSmokeNode->attachObject(mSmokeParticle);
			mDamaged = true;
		}

		ContextApp::updateScores(this);
		ContextApp::getChaseCamera()->shake(8, 25, 15 );
		AudioManager::play(AUDIO_DEST_TANK, 0.5f);

		return true;
	}

	void GreenTank::setDrawTrails(bool enable)
	{ 
		mLeftTrails->setDrawTrails(enable); 
		mRightTrails->setDrawTrails(enable);
	}

	bool GreenTank::isDrawTrails() const 
	{ 
		return mLeftTrails->isDrawTrails() && mRightTrails->isDrawTrails(); 
	}

	void GreenTank::beginDrawTrails()
	{
		mLeftTrails->beginDraw();
		mRightTrails->beginDraw();
	}

	// 停止绘制车印
	void GreenTank::endDrawTrails()
	{
		mLeftTrails->endDraw();
		mRightTrails->endDraw();
	}



	void GreenTank::fire()
	{
		if (mFireCD > 8.0f)	
		{
			ObjectManager::getSingletonPtr()->createBullet(this);
			mFireCD = 0;
		}
	}

	bool GreenTank::keyPressed( const OIS::KeyEvent &evt )
	{
		if (evt.key == OIS::KC_W || evt.key == OIS::KC_UP ) mKeyDirection.z = 1;
		else if (evt.key == OIS::KC_S || evt.key == OIS::KC_DOWN ) mKeyDirection.z = -1;
		else if (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT ) mKeyDirection.x = 1;
		else if (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT ) mKeyDirection.x = -1;

		if (evt.key == OIS::KC_F1) { mBulletType = BULLET_ROCKET; }
		else if (evt.key == OIS::KC_F2) { mBulletType = BULLET_IMPULSE; }

		if (evt.key == OIS::KC_SPACE)
		{
			fire();
		}

		return true;
	}

	bool GreenTank::keyReleased( const OIS::KeyEvent &evt )
	{
		// keep track of the player's intended direction
		if ( (evt.key == OIS::KC_W || evt.key == OIS::KC_UP) && mKeyDirection.z == 1) mKeyDirection.z = 0;
		else if ( (evt.key == OIS::KC_S ||  evt.key == OIS::KC_DOWN)  && mKeyDirection.z == -1) mKeyDirection.z = 0;
		else if ( (evt.key == OIS::KC_A || evt.key == OIS::KC_LEFT) && mKeyDirection.x == 1) mKeyDirection.x = 0;
		else if ( (evt.key == OIS::KC_D || evt.key == OIS::KC_RIGHT) && mKeyDirection.x == -1) mKeyDirection.x = 0;

		return true;
	}

	bool GreenTank::mouseMoved(const OIS::MouseEvent &e)
	{
		mMousePos.x = e.state.X.abs;
		mMousePos.y = e.state.Y.abs;

		return true;
	}

	bool GreenTank::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
	{
		if (id == OIS::MB_Left)
		{
			//mMousePos.x = e.state.X.abs;
			//mMousePos.y = e.state.Y.abs;
			fire();

		}
		return true;
	}

	/**
	* This little snippet gets the screenspace coordinates for a MovableObject
	*
	* @param   object   The object to retrieve the coordidnates of.
	* @param   camera   The active camera
	* @param   result   The Vector2 to store the result in
	*
	* @return   Returns true if the object is visible and the coordinates were
	*         retrieved, false otherwise.
	*/
	bool GreenTank::getScreenspaceCoords(Ogre::MovableObject* object, Ogre::Camera* camera, Ogre::Vector2&result)
	{

		if(!object->isInScene())
			return false;

		const Ogre::AxisAlignedBox &AABB = object->getWorldBoundingBox(true);
		/**
		* If you need the point above the object instead of the center point:
		* This snippet derives the average point between the top-most corners of the bounding box
		* Ogre::Vector3 point = (AABB.getCorner(AxisAlignedBox::FAR_LEFT_TOP)
		*    + AABB.getCorner(AxisAlignedBox::FAR_RIGHT_TOP)
		*    + AABB.getCorner(AxisAlignedBox::NEAR_LEFT_TOP)
		*    + AABB.getCorner(AxisAlignedBox::NEAR_RIGHT_TOP)) / 4;
		*/


		// Get the center point of the object's bounding box
		Ogre::Vector3 point = AABB.getCenter();

		// Is the camera facing that point? If not, return false
		Ogre::Plane cameraPlane = Plane(Vector3(camera->getDerivedOrientation().zAxis()), camera->getDerivedPosition());

		if(cameraPlane.getSide(point) != Plane::NEGATIVE_SIDE)
			return false;

		// Transform the 3D point into screen space
		point = camera->getProjectionMatrix() * (camera->getViewMatrix() * point);

		// Transform from coordinate space [-1, 1] to [0, 1] and update in-value
		result.x = (point.x / 2) + 0.5f;
		result.y = 1 - ((point.y / 2) + 0.5f);

		return true;
	}

	const Vector3& GreenTank::getTankFront()
	{
		Vector3 front = mBodyNode->_getDerivedOrientation().zAxis();
		if (!mFrontZ)
		{
			front*=-1.0f;
		}
		return front;
	}

}
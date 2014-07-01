/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   ChaseCamera.cpp
			created:    6/28/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "ChaseCamera.h"

using namespace Ogre;

namespace Tank
{


	ChaseCamera::ChaseCamera(GreenTank* tank, Ogre::Camera *camera, const Ogre::Vector3 &offset)
		: mOwner(tank), mSceneManager(tank->getSceneManager()), mCamera(camera), 
		mMoveFactor(0.002f), mShakeTime(-1), mCurrentShakeTime(0), mShakeOffset(0,0,0)
	{
		// Three nodes used here to simulate a camera with a buffer
		// mPivotNode is a node in world coordinates, time to update the current position of the tank for the role
		// mGoalNode is a child node mPivotNode, and with a certain offset, this node is the camera to follow the ultimate goal
		// mCameraNode camera is actually bound node, this node is bound to the camera, and to keep moving mGoalNod
		// This will form a buffer Camera System
		mPivotNode = mSceneManager->getRootSceneNode()->createChildSceneNode();

		//Ogre::Entity* ent = mSceneManager->createEntity("axes.mesh");
		mGoalNode = mOwner->getBodyNode()->createChildSceneNode(offset);
		//Ogre::SceneNode* child = mGoalNode->createChildSceneNode("gola" , Ogre::Vector3(0 , -25 , -25 ) );
		//child->scale(0.2 , 0.2 , 0.2);
		//child->attachObject(ent);

		mCameraNode = mSceneManager->getRootSceneNode()->createChildSceneNode();
		mCameraNode->setPosition(mPivotNode->getPosition() + mGoalNode->getPosition());

		
		mPivotNode->setFixedYawAxis(true);
		mGoalNode->setFixedYawAxis(true);
		mCameraNode->setFixedYawAxis(true);

		//mGoalNode->lookAt(mOwner->getBodyNode()->_getDerivedPosition() , Node::TS_LOCAL );//mOwner->getBodyNode()
		mCameraNode->attachObject(mCamera);
		mCameraNode->lookAt(mPivotNode->_getDerivedPosition(), Ogre::Node::TS_WORLD);
	}



	ChaseCamera::~ChaseCamera()
	{
		mSceneManager->destroySceneNode(mPivotNode);
		mSceneManager->destroySceneNode(mGoalNode);
		mSceneManager->destroySceneNode(mCameraNode);
	}
	//Ogre::SceneNode			*mPivotNode;	// ��������ڵ�
	//Ogre::SceneNode			*mGoalNode;		// ������˶�Ŀ��ڵ�
	//Ogre::SceneNode			*mCameraNode;	// ������󶨽ڵ�

	void ChaseCamera::wheelGoal(Ogre::Real deltaZoom)
	{

		Vector3 local =  mGoalNode->getPosition();

		Real dist = mGoalNode->_getDerivedPosition().distance(mPivotNode->_getDerivedPosition());

		Real distChange = deltaZoom * dist;

		mGoalNode->translate(0, 0, distChange, Node::TS_LOCAL);

		//dist = mGoalNode->_getDerivedPosition().distance(mPivotNode->_getDerivedPosition());
		////! goal��pivot��ˮƽ����
		//Vector3  direction = mPivotNode->_getDerivedPosition() - mGoalNode->_getDerivedPosition();
		//Real verticalDistance = Math::Abs(direction.y);

		//if(dist  < verticalDistance + 2.0f   )
		//{
		//	mGoalNode->translate(0, 0, -distChange, Node::TS_LOCAL);
		//}

		Vector3 localGoal = mGoalNode->getPosition();
		if (localGoal.z < 0 )
		{
			mGoalNode->translate(0, 0, -distChange, Node::TS_LOCAL);

		}

	}

	void ChaseCamera::pitchGoal(Ogre::Degree degree)
	{
		//Ogre::Radian rad(degree);
		Ogre::Vector3 vec(0 , degree.valueDegrees()*100 , 0);
		Ogre::Vector3 trans = mGoalNode->getOrientation() * vec;
		Ogre::Vector3 pos = mGoalNode->getPosition();


		pos = pos + trans;

		mGoalNode->setPosition(pos);

	}



	/**
	״̬����, ÿһ֡����
	@timeSinceLastFrame	����һ֡��ʱ��(����s)
	*/
	void ChaseCamera::update(float timeSinceLastFrame)
	{
		updateShake(timeSinceLastFrame);

		// ʱ�̸����ɫλ��
		mPivotNode->setPosition(mOwner->getPosition());
		// ��ȡ�����Ŀ��ڵ㵽��ǰ�����λ�õ�ƫ����
		// ע : ע�������ķ���
		Ogre::Vector3 offset = mGoalNode->_getDerivedPosition() - mCameraNode->_getDerivedPosition();

		mCameraNode->translate((offset + mShakeOffset) * mMoveFactor * timeSinceLastFrame);
		// mCameraNode->translate((offset ) * mMoveFactor * 2 * timeSinceLastFrame);
		// ���ڵ㳯�Ž�ɫ̹�˿�
		mCameraNode->lookAt(mPivotNode->_getDerivedPosition(), Ogre::Node::TS_WORLD);
	}



	void ChaseCamera::shake(float shakTime, float shakeRight, float shakeUp) 
	{ 
		if (mShakeTime > 0) 
			return;

		mShakeTime = shakTime; 
		mShakeRight = shakeRight;
		mShakeUp = shakeUp;
		mCurrentShakeTime = 0;
		mShakeOffset = Ogre::Vector3::ZERO;
	}

	void ChaseCamera::updateShake(float timeSinceLastFrame)
	{
		static int direction = 1;

		if (mShakeTime > 0)
		{
			mCurrentShakeTime += timeSinceLastFrame / 100;		
			if (mCurrentShakeTime > mShakeTime)
			{	// ֹͣ����, ��������
				mShakeTime = -1;
				mShakeOffset = Ogre::Vector3::ZERO;
			}
			else
			{
				// ��ȡ��ǰ̹�˵�������
				Ogre::Vector3 up = mOwner->getTankFront();
				// ���򶶶�
				if (direction == 1)
				{
					// ZΪ������
					if (!Ogre::Math::RealEqual(up.z, 0.0f))
					{
						if (mShakeOffset.x < mShakeRight)
							mShakeOffset.x += 1;
						else
							direction = -1;
					}
					else	// XΪ������
					{
						if (mShakeOffset.z < mShakeRight)
							mShakeOffset.z += 1;
						else
							direction = -1;
					}		
				}
				else
				{
					// ZΪ������
					if(!Ogre::Math::RealEqual(up.z, 0.0f))
					{
						if (mShakeOffset.x > -mShakeRight)
							mShakeOffset.x -= 1;
						else
							direction = 1;
					}
					else	// XΪ������
					{
						if (mShakeOffset.z > -mShakeRight)
							mShakeOffset.z -= 1;
						else
							direction = 1;
					}
				}

				// ���·����������
				if (mShakeUp > 0)
					mShakeOffset.y = Util::randomRange(-mShakeUp,mShakeUp);
			}
		}
	}


}
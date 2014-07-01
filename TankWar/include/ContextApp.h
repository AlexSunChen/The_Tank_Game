/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   ContextApp.h
			created:    6/29/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "SingletonT.h"
#include "GUIManager.h"
#include "Util.h"
#include "GreenTank.h"
#include "ChaseCamera.h"
#include "Object.h"
#include "MiniMap.h"
#include <string>

namespace Tank
{
	/**
	Context management 
	*/
	class ObjectManager;

	class ContextApp : public SingletonT<ContextApp>
	{
		SINGLETON_DEFINE(ContextApp)

		friend class GameState;
		friend class ChooseMapState;
	public:
		// Update players GUI displays information such as scores, the value of life, etc.
		static void updateScores(BaseTank *tank);
		static void updateScores() { updateScores(getPlayerA());}

		static Ogre::SceneManager* getGameSceneManager();
		static Ogre::SceneManager* getAideSceneMgr();

		static Ogre::Camera* getGameCamera();
		static Ogre::Camera* getAideCamera();

		static Ogre::RenderWindow* getRenderWindow();

		// Show victory message
		static void showWinnerInfo(bool winner);

		// Get the current single-player game
		static GreenTank*	getPlayerA() { return getSingletonPtr()->mPlayerA; }
		static ChaseCamera* getChaseCamera() { return getSingletonPtr()->mChaseCamera; }
		static MiniMap* getMiniMap() { return getSingletonPtr()->mMiniMap; }

		// Get the remaining number of tanks
		static int getLastEnemyNumber() ;
		// Whether to eliminate all enemies
		static bool isEnemyDieOut() { return getLastEnemyNumber() == 0; }
		// Destroy an enemy tank
		static void destroyOneEnemy();
		// Produce an enemy tank
		static void createOneEnemy();
		// Can also produce tanks?
		static bool canBirthEnemy() ;

		// Get the max number of the enemy on one map 
		static int getMaxEnemyNumberInScene(){ return getSingletonPtr()->mMaxEnemyNumberInScene; }

		// Reset date
		static void resetEnemyData(int maxEnemyNumber, int totalEnemyNumber, std::string mapName);

		// Update winning or losing information, the game is over returns true, the game did not end return false
		static bool updateWinner(float timeSinceLastFrame);
		// Get the current name of the map
		static std::string getCurrentMapName() { return getSingletonPtr()->mCurrentMapName;}

		// The number of current remaining players 
		static int getPlayerLifeNumber() { return getSingletonPtr()->mLifeNumber; }

	protected:
		ContextApp();

	protected:
		GreenTank *mPlayerA;				// Current player Tank
		ChaseCamera *mChaseCamera;	// Buffer camera

		MiniMap *mMiniMap;				// Mini map - Radar

		std::string mCurrentMapName;				// The name of the currently loaded map
		int mDeathEnemyNumber; // The number of enemies to be killed
		int mTotalEnemyNumber; // Total number of enemy
		int mBirthEnemyNumber; // The current number of the enemy has been generated

		int mMaxEnemyNumberInScene;			// Maximum number of enemy presence in the scene at the same time
		int mLifeNumber; // The number of current remaining players

		int mFinish; // Game Over


	};
}
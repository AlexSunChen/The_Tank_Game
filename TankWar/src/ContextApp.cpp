/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   ContextApp.cpp
			created:    6/29/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "ContextApp.h"
#include "GameApp.h"
#include "GUIManager.h"
#include "StateManager.h"
#include "ChooseMapState.h"
#include "MapManager.h"

#include "GameState.h"


namespace Tank
{
	//----------------------------------------------------------------------------------------

	ContextApp::ContextApp() : mPlayerA(0), mChaseCamera(0), mFinish(false),
		mDeathEnemyNumber(0), mTotalEnemyNumber(20), mMaxEnemyNumberInScene(4)
	{

	}

	//----------------------------------------------------------------------------------------

	void ContextApp::updateScores(BaseTank *tank)
	{
		int money = tank->getTotalMoney();
		std::string strMoney = Util::format((int)money, 7);
		GUIManager::getWindow("GameUI/Scores")->setText(strMoney);

		std::string life = Util::toString(tank->getCurrentLifeValue()) + "/" + Util::toString(tank->getLifeValue());
		GUIManager::getWindow("GameUI/Life")->setText(life);

		std::string enemyNumber = Util::toString(getLastEnemyNumber());
		GUIManager::getWindow("GameUI/LastTanks")->setText(enemyNumber);

		std::string lifeNumber = Util::toString(getPlayerLifeNumber());
		GUIManager::getWindow("GameUI/TankNumber")->setText(lifeNumber);
	}

	//----------------------------------------------------------------------------------------

	Ogre::SceneManager* ContextApp::getGameSceneManager() 
	{ 
		return GameApp::getSingletonPtr()->getGameSceneMgr(); 
	}

	//----------------------------------------------------------------------------------------

	Ogre::SceneManager* ContextApp::getAideSceneMgr() 
	{ 
		assert(0&&"ContextApp::getAideSceneMgr");
		return GameApp::getSingletonPtr()->getAideSceneMgr(); 
	}

	//----------------------------------------------------------------------------------------

	Ogre::Camera* ContextApp::getGameCamera() 
	{ 
		return GameApp::getSingletonPtr()->getGameSceneMgr()->getCameraIterator().getNext(); 
	}

	//----------------------------------------------------------------------------------------

	Ogre::Camera* ContextApp::getAideCamera() 
	{ 
		return GameApp::getSingletonPtr()->getAideSceneMgr()->getCameraIterator().getNext(); 
	}

	//----------------------------------------------------------------------------------------

	Ogre::RenderWindow* ContextApp::getRenderWindow() 
	{ 
		return GameApp::getSingletonPtr()->getRenderWindow(); 
	}

	//----------------------------------------------------------------------------------------

	
	int ContextApp::getLastEnemyNumber()  
	{ 
		return getSingletonPtr()->mTotalEnemyNumber - getSingletonPtr()->mDeathEnemyNumber; 
	}

	//----------------------------------------------------------------------------------------

	
	void ContextApp::destroyOneEnemy() 
	{ 
		if (!isEnemyDieOut()) 
			getSingletonPtr()->mDeathEnemyNumber++; 
	}

	//----------------------------------------------------------------------------------------

	void ContextApp::createOneEnemy() 
	{
		if (!isEnemyDieOut()) getSingletonPtr()->mBirthEnemyNumber++; 
	}

	//----------------------------------------------------------------------------------------

	
	bool ContextApp::canBirthEnemy()
	{ 
		return getSingletonPtr()->mBirthEnemyNumber < getSingletonPtr()->mTotalEnemyNumber;
	}

	//----------------------------------------------------------------------------------------

	void ContextApp::resetEnemyData(int maxEnemyNumber, int totalEnemyNumber, std::string mapName)
	{
		getSingletonPtr()->mMaxEnemyNumberInScene = maxEnemyNumber;
		getSingletonPtr()->mTotalEnemyNumber = totalEnemyNumber;
		getSingletonPtr()->mCurrentMapName = mapName;
		getSingletonPtr()->mDeathEnemyNumber = 0;
		getSingletonPtr()->mBirthEnemyNumber = 0;
		getSingletonPtr()->mLifeNumber = 3;
		getSingletonPtr()->mFinish = false;
	}

	//----------------------------------------------------------------------------------------

	void ContextApp::showWinnerInfo(bool winner)
	{
		GameState* gameState = static_cast<GameState*>( StateManager::getSingletonPtr()->getCurrentState() );

		if (winner)
		{
			GUIManager::loadWindow("WinnerUI");

			int money = getPlayerA()->getTotalMoney();
			std::string strMoney = Util::format((int)money, 7);
			strMoney = "Scores     :     " + strMoney;
			GUIManager::getWindow("WinnerUI/Info/Scores")->setText(strMoney);

			if (gameState)
			{
				GUIManager::subscribeEvent("WinnerUI/Info" , CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&GameState::onWinnerWindowClick, gameState));
			}



		}
		else
		{
			GUIManager::loadWindow("LosterUI");
			int money = getPlayerA()->getTotalMoney();
			std::string strMoney = Util::format((int)money, 7);
			strMoney = "Scores     :     " + strMoney;
			GUIManager::getWindow("LosterUI/Info/Scores")->setText(strMoney);

			if (gameState)
			{
				GUIManager::subscribeEvent("LosterUI/Info" , CEGUI::DefaultWindow::EventMouseClick, CEGUI::Event::Subscriber(&GameState::onLoserWindowClick, gameState));
			}

		}
	}



	bool ContextApp::updateWinner(float timeSinceLastFrame)
	{
		static float flushTime = 0;

		if (getPlayerLifeNumber() > 0)
		{
			if (!getPlayerA()->isAlive())
			{
				getSingletonPtr()->mLifeNumber--;
				GreenTank* player = getPlayerA();
				player->resetFullLifeValue();
				player->resetPosition();

				updateScores(player);

				if (getPlayerLifeNumber() == 0)
				{
					player->setVisible(false);
					return true;
				}

				AudioManager::play(AUDIO_BEGIN);

				return false;
			}

			// Gameover
			if(isEnemyDieOut())
			{
				flushTime += timeSinceLastFrame * 0.1f;

				if (!getSingletonPtr()->mFinish)
				{
					AudioManager::stop(AUDIO_BACKGROUND , false);
					AudioManager::play(AUDIO_SCORE_ADD);
					// hide minimap
					getSingletonPtr()->mMiniMap->setVisible(false);
					// show win
					showWinnerInfo(true);
					getSingletonPtr()->mFinish = true;

				}
				else
				{
					// Show some time, and then transfer to  the selection map interface
					if (flushTime > 1000)
					{
						flushTime = 0;
						StateManager::getSingletonPtr()->changeState(ChooseMapState::getSingletonPtr());
					}
				}
				return true;
			}
			else
				return false;
		}
		else
		{
			flushTime += timeSinceLastFrame * 0.1f;

			if (!getSingletonPtr()->mFinish)
			{
				AudioManager::stop(AUDIO_BACKGROUND , false);
				AudioManager::play(AUDIO_SCORE_ADD);
				// hide minimap
				getSingletonPtr()->mMiniMap->setVisible(false);
				// show win
				showWinnerInfo(false);
				getSingletonPtr()->mFinish = true;
			}
			else
			{
				// Show some time, and then transfer to  the selection map interface
				if (flushTime > 1000)
				{
					flushTime = 0;
					StateManager::getSingletonPtr()->changeState(ChooseMapState::getSingletonPtr());
				}
			}
			return true;
		}
	}

	//----------------------------------------------------------------------------------------
}
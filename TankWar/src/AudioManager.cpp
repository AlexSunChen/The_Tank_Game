/*
-----------------------------------------------------------------------------
***********************************************************************
			filename:   AudioManager.cpp
			created:    6/29/2014
			author:     Sun Chen
************************************************************************
************************************************************************
-----------------------------------------------------------------------------
*/

#include "AudioManager.h"
#include "Config.h"

namespace Tank
{
	AudioManager::AudioManager() : mAudios(AUDIO_SIZE)
	{
		FMOD::System_Create(&mSystem);
		mSystem->init(32, FMOD_INIT_NORMAL, 0);			

		loadAudios();
	}

	AudioManager::~AudioManager()
	{

	}

	void AudioManager::destroy()
	{
		mFadeIns.clear();
		mFadeOuts.clear();

		for (unsigned int i = 0; i < mAudios.size(); i++)
			SAFE_DELETE( mAudios[i] );

		mAudios.clear();
		mSystem->close();
		mSystem->release();
	}

	void AudioManager::play(AUDIO_TYPE audio, float volume, bool loop, bool fadeIn)
	{
		AudioManager* audioMgr = getSingletonPtr();
		audioMgr->mAudios[audio]->setLoop(loop);
		audioMgr->mAudios[audio]->play();
		audioMgr->mAudios[audio]->setFadeInVolume(volume);

		if (fadeIn) 
		{
			audioMgr->mAudios[audio]->setVolume(0);
			audioMgr->mFadeIns.push_back(audioMgr->mAudios[audio]);
		}
		else
			audioMgr->mAudios[audio]->setVolume(volume);
	}

	void AudioManager::stop(AUDIO_TYPE audio, bool fadeOut)
	{
		AudioManager* audioMgr = getSingletonPtr();
		if (fadeOut) 
			audioMgr->mFadeOuts.push_back(audioMgr->mAudios[audio]);
		else
			audioMgr->mAudios[audio]->stop();
	}

	void AudioManager::updateFade()
	{
		for (std::list<AudioElem*>::iterator itr = mFadeIns.begin(); itr != mFadeIns.end(); )
		{
			// Fade to perform the operation, getting into the end it is deleted from getting into the queue
			float volume = (*itr)->getVolume();
			if (volume + FADE_VOLUME < (*itr)->getFadeInVolume())
			{
				(*itr)->setVolume(volume + FADE_VOLUME);
				++itr;
			}
			else
				mFadeIns.erase(itr++);
		}

		for (std::list<AudioElem*>::iterator itr = mFadeOuts.begin(); itr != mFadeOuts.end(); )
		{
			// Fade to perform the operation, getting into the end it is deleted from getting into the queue
			float volume = (*itr)->getVolume();
			if (volume - FADE_VOLUME > 0.0f)
			{
				(*itr)->setVolume(volume - FADE_VOLUME);
				++itr;
			}
			else
			{
				(*itr)->stop();
				mFadeIns.erase(itr++);
			}
		}
	}

	void AudioManager::loadAudios()
	{
		Config soundConfig(TANKWAR_RES_ROOT_PATH + "music/Sounds.cfg");
		std::string path = TANKWAR_RES_ROOT_PATH + "music/";
//
		mAudios[AUDIO_INIT] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "INITMUSIC"));
		mAudios[AUDIO_BACKGROUND] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "BACKGROUND"));
		mAudios[AUDIO_CRASH_WALL] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "CRASH_WALL"));
		mAudios[AUDIO_DEST_TANK] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "DEST"));
		mAudios[AUDIO_EXPLOSION] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "EXPLOSION"));
		mAudios[AUDIO_FIRE] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "FIRE"));
		mAudios[AUDIO_IMPULSE] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "IMPULSE"));
		mAudios[AUDIO_ITEM_TAKE] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "ITEM_TAKE"));
		mAudios[AUDIO_MOUSE_CLICK] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "MOUSE_CLICK"));
		mAudios[AUDIO_MOUSE_OVER] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "MOUSE_OVER"));
		mAudios[AUDIO_SCORE_ADD] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "SCORE_ADD"));
		mAudios[AUDIO_BEGIN] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "BEGIN"));
		mAudios[AUDIO_TRUBA] = new AudioElem(mSystem, path + soundConfig.getString("SOUNDS", "TRUBA"));
	}


}
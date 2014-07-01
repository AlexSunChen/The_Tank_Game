/*
	-----------------------------------------------------------------------------
	***********************************************************************
				filename:   AudioManager.h
				created:    6/29/2014
				author:     Sun Chen
	************************************************************************
	************************************************************************
	-----------------------------------------------------------------------------
*/

#pragma once

#include "SingletonT.h"
#include "Common.h"
#include "fmod.hpp"
#include "fmod.h"
#include <vector>
#include <list>

namespace Tank
{
	// Audio enum
	enum AUDIO_TYPE
	{
		AUDIO_INIT = 0,
		AUDIO_BACKGROUND,	     // background music
		AUDIO_BEGIN,			// Start game music
		AUDIO_CRASH_WALL,		// Destroyed wall
		AUDIO_DEST_TANK,		// Destroy tanks
		AUDIO_EXPLOSION,		// Explosion
		AUDIO_FIRE,				// FIRE
		AUDIO_IMPULSE,			// Pulse Gun
		AUDIO_ITEM_TAKE,		// Pick up items
		AUDIO_MOUSE_CLICK,		// Mouse click
		AUDIO_MOUSE_OVER,		// Move the mouse over the control
		AUDIO_SCORE_ADD,		// Add scores
		AUDIO_TRUBA,			// Victory
		AUDIO_SIZE
	};

	const static float FADE_VOLUME = 0.1f;

	class AudioManager : public SingletonT<AudioManager>, public IFrameListener
	{
		SINGLETON_DEFINE(AudioManager)
		
		/** Audio elements simple package */
		class AudioElem
		{
		public:
			FMOD::Sound		*mSound;
			FMOD::Channel	*mChannel;
			FMOD::System		*mSystem;
			float						mFadeInVolume;

			AudioElem(FMOD::System *system, const std::string &musicName) 
				: mSystem(system), mChannel(0), mFadeInVolume(1)
			{
				system->createSound(musicName.c_str(), FMOD_HARDWARE, 0, &mSound);
			}

			~AudioElem() { mSound->release(); }
			
			// Playing the current music
			void play() { mSystem->playSound(FMOD_CHANNEL_FREE, mSound, false, &mChannel); }
			// Stop the current music
			void stop() { if (mChannel) mChannel->stop(); }
			/**
				Set music loops
				@loop Whether the cycle
			*/
			void setLoop(bool loop) 
			{ 
				setMode(loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF);
				mChannel->setLoopCount(loop ? -1 : 1); 
			}
			// Set mode
			void setMode(FMOD_MODE mode){	mSound->setMode(mode); }
			// Set volume [0~1]
			void setVolume(float volume){ if (mChannel) mChannel->setVolume(volume); }
			// Set the volume limit(0~1]
			void setFadeInVolume(float fadeInVolume) { mFadeInVolume = fadeInVolume; }
			// Get the current ceiling audio
			float getFadeInVolume() { return mFadeInVolume; }
			// Get the current audio
			float getVolume() 
			{ 
				float volume = 0; 
				mChannel->getVolume(&volume); 
				return volume; 
			}
		};

	public:
		~AudioManager();
		
		// Free resources
		void destroy();
		
		// Updated audio system
		inline void update( float timeSinceLastFrame )
		{
			updateFade();
			mSystem->update(); 
		}

		/**
			Plays the specified music
			@audio  play the type of music
			@volume	Playback volume
			@loop		Whether the loop
			@fadeIn	Whether to perform gradually into effect
		*/
		static void play(AUDIO_TYPE audio, float volume = 0.75f, bool loop = false, bool fadeIn = true);
		
		/**
			Stop specifies the music 
			@audio		Play the type of music
			@fadeOut	Want to perform a fade effect ?
		*/
		static void stop(AUDIO_TYPE audio, bool fadeOut = true);

	protected:
		AudioManager();
		
		// Updated audio gradient
		void updateFade();
		// Load audio files
		void loadAudios();

	protected:
		std::vector<AudioElem*>	mAudios;		// Audio collection
		FMOD::System					*mSystem;

		std::list<AudioElem*>		mFadeIns;		// Getting into the queue
		std::list<AudioElem*>		mFadeOuts;	// Getting out of the queue
	};
}
#ifndef SOUND_H
#define SOUND_H

#include "3DLibs.h"
#include <Dbt.h>
#include "Audio.h"

class Sound
{
	
	private:
		static const int SIZE=10;
		std::unique_ptr<AudioEngine> audEng;
		std::unique_ptr<SoundEffect> m_soundEffects[SIZE];
		std::string names[SIZE];
		int nrOfEffects;

	public:
		Sound();
		Sound(const Sound& other);
		~Sound();
		bool addSoundEffect(std::wstring filepath, std::string effectName);
		void init();
		void playSound(std::string effectname);
		void shutDown();
	

};
#endif
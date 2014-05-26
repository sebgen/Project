#include "Sound.h"

Sound::Sound()
{
	
}
Sound::Sound(const Sound& other)
{
}
Sound::~Sound()
{
}

void Sound::init()
{
	nrOfEffects=0;
	audEng.reset(new AudioEngine(AudioEngine_Default));
	for(int i=0; i<SIZE; i++)
	{
		names[i]="";
	}
}
void Sound::playSound(std::string effectname)
{	
	int found=-1;
	for(int i=0; i<nrOfEffects; i++)
	{
		if(names[i]==effectname)
		{
			found=i;
		}
	}
	if(found!=-1)
	{
		if(!m_soundEffects[found]->IsInUse())
		{
			m_soundEffects[found]->Play();
		}
		
	}
	else OutputDebugString("dident find the soundeffect\n");
	
}

bool Sound::addSoundEffect(std::wstring filepath,std::string effectName)
{
	if(nrOfEffects==SIZE-1)
	{
		OutputDebugString("array is full set SIZE to a greater value\n");
		return false;
	}
	for(int i=0; i< nrOfEffects; i++)
	{
		if(effectName==names[i])
		{
			OutputDebugString("that effect allready exist\n");
			return false;
		}
	}
	names[nrOfEffects]=effectName;
	m_soundEffects[nrOfEffects].reset(new SoundEffect(audEng.get(),filepath.c_str()));
	nrOfEffects++;
	return true;
}
void Sound::shutDown()
{
	audEng.release();
	for(int i=0; i<SIZE; i++)
	{
		m_soundEffects[i].release();
	}
	m_soundEffects->release();
}

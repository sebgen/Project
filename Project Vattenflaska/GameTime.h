/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: GameTime.h
  Get delta time
  From Frank Luna's Introduction to 3D Game Programming with DirectX 11
-------------------------------------------------------------------------
  History:
  2014-04-26
  Created by Simon Johanson

*************************************************************************/

#include <Windows.h>

class GameTime
{
	private:
		double	m_secondsPerCount;
		double	m_deltaTime;

		__int64	m_baseTime;   // Holds the count from application start
		__int64 m_pausedTime; // Sum of all paused time
		__int64 m_stopTime;   // The time stamp when timer is stopped
		__int64 m_prevTime;
		__int64 m_currTime;

		bool	m_stopped;

	public:
		GameTime();
		~GameTime();

		void Reset();
		void Start();
		void Stop();
		void Tick();

		float GetGameTime() const; 
		float GetDeltaTime() const;
};
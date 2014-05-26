/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
-------------------------------------------------------------------------
  History:
  2014-04-26
  Created by Simon Johanson

*************************************************************************/

#include "GameTime.h"

GameTime::GameTime()
{
	m_secondsPerCount	= 0.0f;
	m_deltaTime			= 0.0f;

	m_baseTime			= 0;
	m_pausedTime		= 0;
	m_stopTime			= 0;
	m_prevTime			= 0;
	m_currTime			= 0;

	m_stopped			= false;

	__int64 countsPerSec;
	QueryPerformanceFrequency( (LARGE_INTEGER*)&countsPerSec );
	m_secondsPerCount = 1.0f / (double)countsPerSec;
}

GameTime::~GameTime()
{
}

void GameTime::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter( (LARGE_INTEGER*)&currTime );

	m_baseTime = currTime;
	m_prevTime = currTime;
	m_stopTime = 0;
	m_stopped  = false;
}

void GameTime::Start()
{
	__int64 startTime;
	QueryPerformanceCounter( (LARGE_INTEGER*)&startTime );

	// If we are resuming the timer from a stopped state
	if( m_stopped )
	{
		// Accumulate the paused time
		m_pausedTime += (startTime - m_stopTime );

		// Since we're starting the timer back up, the current
		// previous time is not valid, as it occurred while paused.
		// So reset it to the current time
		m_prevTime = startTime;

		// No longer stopped
		m_stopTime = 0;
		m_stopped  = false;
	}
}

void GameTime::Stop()
{
	// If we're already stopped, then don't do anything
	if( !m_stopped )
	{
		__int64 currTime;
		QueryPerformanceCounter( (LARGE_INTEGER*)&currTime );

		// Otherwise, save the time we stopped at and flag at stopped
		m_stopTime = currTime;
		m_stopped  = true;
	}
}

//===================================================
// Called every frame to calculate time difference ||
// between previous and current frame and store it ||
//                  as delta time				   ||
//===================================================
void GameTime::Tick()
{
	if( m_stopped )
	{
		m_deltaTime = 0.0f;
		return;
	}

	// Get the time this frame
	__int64 currTime;
	QueryPerformanceCounter( (LARGE_INTEGER*)&currTime );
	m_currTime = currTime;

	// Time difference between this frame and the previous
	m_deltaTime = ( m_currTime - m_prevTime ) * m_secondsPerCount;

	// Prepare for next frame
	m_prevTime = m_currTime;

	// Force nonnegative, iow delta time is not allowed to be negative
	if( m_deltaTime < 0.0f )
		m_deltaTime = 0.0f;
}

//=======================================
// Returns the amount of time that has ||
// elapsed since the application start ||
//=======================================
float GameTime::GetGameTime() const
{
	if( m_stopped )
		return (float)( ( ( m_stopTime - m_pausedTime ) - m_baseTime ) * m_secondsPerCount );
	else
	{
		return (float)( ( ( m_currTime - m_pausedTime ) - m_baseTime ) * m_secondsPerCount );
	}

}

float GameTime::GetDeltaTime() const
{
	return (float)m_deltaTime;
}
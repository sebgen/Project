/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: LuaWrapper.h
  Singleton. Handle all in and out going communication with lua

-------------------------------------------------------------------------
  History:
  2014-05-26
  Created by Sebastian Genlund

*************************************************************************/

#ifndef _LUAWRAPPER_H_
#define _LUAWRAPPER_H_

#include "EventSys/EventManagerImpl.h"
#include "EventSys/Events.h"
#include "LuaEngine.h"
#include "Lever.h"

class LuaWrapper
{
private: // Members
	LuaEngine*				m_le;
	lua_State*				m_L;
	EventManager*			m_em;

private: // Singleton private
	static LuaWrapper*		m_instance;
	LuaWrapper();

public: // Singleton public
	void Initialize( LuaEngine* le, EventManager* em );
	static LuaWrapper* Instance();
	~LuaWrapper();

public: // Lua wrapper
	void CreateLever( IEventDataPtr pEventData );
	void PullLever( IEventDataPtr pEventData );
	void CreateEvtOpenDoor();

public: // Init metadata for scripts
	void InitMazeMeta();
	void InitDungeonMeta();
	void InitCaveMeta();
};
#endif
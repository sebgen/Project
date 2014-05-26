/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: LuaEngine.h
  Creates and handles the Lua State

-------------------------------------------------------------------------
  History:
  2014-05-26
  Created by Sebastian Genlund

*************************************************************************/

#ifndef _LUAENGINE_H_
#define _LUAENGINE_H_

#include <lua.hpp>
#include <string>
#include <Windows.h>

class LuaEngine
{
private:
	// Games Lua state
	lua_State* m_state;

private:
	// Handle errors
	void ReportErrors( int failure );

public:
	LuaEngine() : m_state( luaL_newstate() ) { luaL_openlibs( m_state ); }
	~LuaEngine() { lua_close( m_state ); }

	// Returns lua state
	lua_State* GetLuaState() { return m_state; }

	// Execute Lua file
	void ExecuteFile( const char* file );

	// Delete old state, create and return a new state
	lua_State* ResetSate();
};

#endif
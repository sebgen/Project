#include "LuaEngine.h"

void LuaEngine::ReportErrors( int failure )
{
	if( failure != 0 )
	{
		OutputDebugString( lua_tostring( m_state, -1 ) );
		lua_pop( m_state, 1 ); // Remove error
	}
}

void LuaEngine::ExecuteFile( const char* file )
{
	if( file == NULL )
		return;
	int failure = luaL_dofile( m_state, file );
	ReportErrors( failure );
}

lua_State* LuaEngine::ResetSate()
{
	if( m_state )
		lua_close( m_state );
	m_state = luaL_newstate();
	luaL_openlibs( m_state );
	return m_state;
}
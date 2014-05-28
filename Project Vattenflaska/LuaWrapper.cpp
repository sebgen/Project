#include "LuaWrapper.h"

//================================================================================================
//										LUA WRAPPER
//================================================================================================

extern "C"
{
	static int LuaOpenDoor( lua_State* L )
	{
		LuaWrapper::Instance()->CreateEvtOpenDoor();
		return 0;
	}

	static int ResetLever( lua_State* L )
	{
		Lever** leverPtr = static_cast<Lever**>( luaL_testudata( L, 1, "LeverMeta" ) );
		if( !leverPtr )
		{
			OutputDebugString( "Error! Getting lever pointer from ResetLever() call in lua" );
			return 0;
		}

		(**leverPtr).ResetLever();

		return 0;
	}
}

void LuaWrapper::CreateLever( IEventDataPtr pEventData )
{
	if( EvtData_Lever_Created::sk_EventType == pEventData->VGetEventType() )
	{
		shared_ptr<EvtData_Lever_Created> lever = std::static_pointer_cast<EvtData_Lever_Created>( pEventData );
		lua_getglobal( m_L, "SetLever" );
		Lever** leverPtr = reinterpret_cast<Lever**>( lua_newuserdata( m_L, sizeof( Lever* ) ) );
		*leverPtr = lever->GetLever();
			luaL_getmetatable( m_L, "LeverMeta" );
			lua_setmetatable( m_L, -2 );
		int err = lua_pcall( m_L, 1, 0, 0 );
		if( err ) // Error if != 0
		{
			OutputDebugString( "\nError creating lever: ");
			OutputDebugString( lua_tostring( m_L, -1 ) );
		}
			
	}
}

void LuaWrapper::CreateWheel( IEventDataPtr pEventData )
{
	if( EvtData_Wheel_Created::sk_EventType == pEventData->VGetEventType() )
	{
		shared_ptr<EvtData_Wheel_Created> wheel = std::static_pointer_cast<EvtData_Wheel_Created>( pEventData );

		if( wheel->GetWheel()->GetWheelType() == MusicWheel )
			lua_getglobal( m_L, "SetMusicWheel" );
		else if( wheel->GetWheel()->GetWheelType() == BoilerWheel )
			lua_getglobal( m_L, "SetBoilerWheel" );
		else if( wheel->GetWheel()->GetWheelType() == ButtonWheel )
			lua_getglobal( m_L, "SetButtonWheel" );
		Wheel** wheelPtr = reinterpret_cast<Wheel**>( lua_newuserdata( m_L, sizeof( Wheel* ) ) );
		*wheelPtr = wheel->GetWheel();
			luaL_getmetatable( m_L, "WheelMeta" );
			lua_setmetatable( m_L, -2 );
		int err = lua_pcall( m_L, 1, 0, 0 );
		if( err )
		{
			OutputDebugString( "\nError creating wheel: ");
			OutputDebugString( lua_tostring( m_L, -1 ) );
		}
	}
}

void LuaWrapper::PullLever( IEventDataPtr pEventData )
{
	if( EvtData_Lever_Pull::sk_EventType == pEventData->VGetEventType() )
	{
		shared_ptr<EvtData_Lever_Pull> lever = std::static_pointer_cast<EvtData_Lever_Pull>( pEventData );
		lua_getglobal( m_L, "PullLever" );
		lua_pushstring( m_L, lever->GetLever()->GetName().c_str() );
		lua_pcall( m_L, 1, 0, 0 );
	}
}

void LuaWrapper::CreateEvtOpenDoor()
{
	IEventDataPtr e(GCC_NEW EvtData_Unlock_Door() );
	m_em->VQueueEvent( e );
}

//================================================================================================
//										Singleton
//================================================================================================

LuaWrapper* LuaWrapper::m_instance = nullptr;

LuaWrapper* LuaWrapper::Instance()
{
	if( !m_instance )
		m_instance = new LuaWrapper();
	return m_instance;
}

LuaWrapper::LuaWrapper()
{
	m_le	= nullptr;
	m_L		= nullptr;
	m_em	= nullptr;
}

LuaWrapper::~LuaWrapper()
{
	m_instance = nullptr;
}

void LuaWrapper::Initialize( LuaEngine* le, EventManager* em )
{
	m_le	= le;
	m_L		= m_le->GetLuaState();
	m_em	= em;

	//=====================================
	//			Event Listeners
	//===================================== 
	// Create Lever
	m_em->VAddListener(
		   std::bind(	&LuaWrapper::CreateLever,
						this,
						std::placeholders::_1),
						EvtData_Lever_Created::sk_EventType);
	// Pull Lever
	m_em->VAddListener(
		   std::bind(	&LuaWrapper::PullLever,
						this,
						std::placeholders::_1),
						EvtData_Lever_Created::sk_EventType);
}

//================================================================================================
//									Init scripts and meta data
//================================================================================================
void LuaWrapper::InitMazeMeta()
{
	m_L = m_le->ResetSate();
	m_le->ExecuteFile( "maze.lua" );
}

void LuaWrapper::InitDungeonMeta()
{
	m_L = m_le->ResetSate();

	//=====================================
	//			Lua Meta Tables
	//=====================================
	//Game
	luaL_newmetatable( m_L, "GameMeta" );

	luaL_Reg GameRegs[] = 
	{
		{ "OpenDoor", LuaOpenDoor },
		{ NULL, NULL },
	};

	luaL_setfuncs( m_L, GameRegs, 0 );

	lua_pushvalue( m_L, -1 );

	lua_setfield( m_L, -2, "__index" );

	lua_setglobal( m_L, "Game" );

	//Lever
	luaL_newmetatable( m_L, "LeverMeta" );
	luaL_Reg LeverRegs[] =
	{
		{ "Reset", ResetLever },
		{ NULL, NULL },
	};
	luaL_setfuncs( m_L, LeverRegs, 0 );
	lua_pushvalue( m_L, -1 );
	lua_setfield( m_L, -2, "__index" );
	lua_setglobal( m_L, "Lever" );

	m_le->ExecuteFile( "Dungeon.lua" );
}

void LuaWrapper::InitCaveMeta()
{
	m_L = m_le->ResetSate();
	//=====================================
	//			Lua Meta Tables
	//=====================================
	//Game
	luaL_newmetatable( m_L, "GameMeta" );

	luaL_Reg GameRegs[] = 
	{
		{ "OpenDoor", LuaOpenDoor },
		{ NULL, NULL },
	};

	luaL_setfuncs( m_L, GameRegs, 0 );

	lua_pushvalue( m_L, -1 );

	lua_setfield( m_L, -2, "__index" );

	lua_setglobal( m_L, "Game" );

	// Wheel
	luaL_newmetatable( m_L, "WheelMeta" );

	luaL_Reg WheelRegs[] = 
	{
		//{ "IsOn", LeverIsOn },
		//{ "WheelGetName", WheelGetName },
		//{ "WheelGetValue", WheelGetValue },
		//{ "Reset", ResetLever },
		{ NULL, NULL },
	};

	luaL_setfuncs( m_L, WheelRegs, 0 );

	lua_pushvalue( m_L, -1 );

	lua_setfield( m_L, -2, "__index" );

	lua_setglobal( m_L, "Wheel" );

	m_le->ExecuteFile( "Cave.lua" );
}
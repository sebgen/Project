#include "LuaWrapper.h"

//================================================================================================
//										LUA WRAPPER
//================================================================================================

extern "C"
{
	static int LuaResetMaze( lua_State* L )
	{
		LuaWrapper::Instance()->CreateEvtResetMaze();
		return 0;
	}

	static int LuaOpenDoor( lua_State* L )
	{
		LuaWrapper::Instance()->CreateEvtOpenDoor();
		return 0;
	}

	static int LuaOpenMazeDoor( lua_State* L )
	{
		const char* doorName = lua_tostring( L, 1, nullptr );
		if( doorName != nullptr )
		{
			LuaWrapper::Instance()->CreateEvtOpenMazeDoor( doorName );
		}
		return 0;
	}

	static int LuaCloseMazeDoor( lua_State* L )
	{
		const char* doorName = lua_tostring( L, 1, nullptr );
		if( doorName != nullptr )
		{
			LuaWrapper::Instance()->CreateEvtCloseMazeDoor( doorName );
		}
		return 0;
	}

	static int ResetLever( lua_State* L )
	{
		Lever** leverPtr = static_cast<Lever**>( luaL_testudata( L, 1, "LeverMeta" ) );
		if( !leverPtr )
		{
			OutputDebugString( "\nError! Getting lever pointer from ResetLever() call in lua" );
			return 0;
		}

		(**leverPtr).ResetLever();

		return 0;
	}

	static int WheelIsOn( lua_State* L )
	{
		Wheel** wheelPtr = static_cast<Wheel**>( luaL_testudata( L, 1, "WheelMeta" ) );
		if( !wheelPtr )
		{
			OutputDebugString( "\nError getting wheel pointer from IsOn() call in lua\n" );
			return 0;
		}
		lua_pushboolean( L, (**wheelPtr).IsOn() );

		return 1;
	}

	static int WheelGetName( lua_State* L )
	{
		Wheel** wheelPtr = static_cast<Wheel**>( luaL_testudata( L, 1, "WheelMeta" ) );
		if( !wheelPtr )
		{
			OutputDebugString( "\nError getting wheel pointer from WheelGetName() call in lua\n" );
			return 0;
		}
		lua_pushstring( L, (**wheelPtr).GetName().c_str() );

		return 1;
	}

	static int WheelGetValue( lua_State* L )
	{
		Wheel** wheelPtr = static_cast<Wheel**>( luaL_testudata( L, 1, "WheelMeta" ) );
		if( !wheelPtr )
		{
			OutputDebugString( "\nError getting wheel pointer from WheelGetValue() call in lua\n" );
			return 0;
		}

		int val = (**wheelPtr).GetValue();
		if( val == 0 )
		{
			OutputDebugString( "\nError wheel type is not a MusicWheel\n" );
			return 0;
		}

		lua_pushinteger( L, val );

		return 1;
	}

	static int StartMusicPanel( lua_State* L )
	{
		LuaWrapper::Instance()->CreateEvtStartMusicPanel();
		return 0;
	}

	static int PlayMusicSequence( lua_State* L )
	{
		LuaWrapper::Instance()->CreateEvtPlayMusicSequence();
	}

	static int LuaChangeNavMesh( lua_State* L )
	{
		const char* navMesh = lua_tostring( L, 1, nullptr );
		if( navMesh != nullptr )
		{
			LuaWrapper::Instance()->CreateEvtChangeNavMesh( navMesh );
		}
		return 0;
	}
}

void LuaWrapper::CreateEvtResetMaze()
{
	IEventDataPtr e(GCC_NEW EvtData_Reset_Maze() );
	m_em->VQueueEvent( e );
}

void LuaWrapper::CreateEvtChangeNavMesh( const char* meshName )
{
	IEventDataPtr e(GCC_NEW EvtData_Change_NavMesh( meshName ) );
	m_em->VQueueEvent( e );
}

void LuaWrapper::CreateEvtOpenMazeDoor( const char* doorName )
{
	IEventDataPtr e(GCC_NEW EvtData_Unlock_Maze_Door( doorName ) );
	m_em->VQueueEvent( e );
}

void LuaWrapper::CreateEvtCloseMazeDoor( const char* doorName )
{
	IEventDataPtr e(GCC_NEW EvtData_Lock_Maze_Door( doorName ) );
	m_em->VQueueEvent( e );
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

void LuaWrapper::RotateWheel( IEventDataPtr pEventData )
{
	if( EvtData_Rotate_Wheel::sk_EventType == pEventData->VGetEventType() )
	{
		int err;
		shared_ptr<EvtData_Rotate_Wheel> wheel = std::static_pointer_cast<EvtData_Rotate_Wheel>( pEventData );
		if( wheel->GetWheel()->GetWheelType() == MusicWheel )
		{
			/*lua_getglobal( m_L, "CheckMusickWheels" );
			err = lua_pcall( m_L, 0, 0, 0 );
			if( err )
			{
				OutputDebugString( "\nError Rotating music wheel: ");
				OutputDebugString( lua_tostring( m_L, -1 ) );
			}*/
		}
		else if( wheel->GetWheel()->GetWheelType() == BoilerWheel )
		{
			lua_getglobal( m_L, "CheckBoilerWheels" );
			err = lua_pcall( m_L, 0, 0, 0 );
			if( err )
			{
				OutputDebugString( "\nError Rotating boiler wheel: ");
				OutputDebugString( lua_tostring( m_L, -1 ) );
			}
		}
		else if( wheel->GetWheel()->GetWheelType() == ButtonWheel )
		{
			/*lua_getglobal( m_L, "CheckButtonWheels" );
			err = lua_pcall( m_L, 0, 0, 0 );
			if( err )
			{
				OutputDebugString( "\nError Rotating Button wheel: ");
				OutputDebugString( lua_tostring( m_L, -1 ) );
			}*/

			if( wheel->GetWheel()->GetName() == "lastMusicWheelShape" )
			{
				lua_getglobal( m_L, "CheckMusickWheels" );
				err = lua_pcall( m_L, 0, 0, 0 );
				if( err )
				{
					OutputDebugString( "\nError Rotating music wheel: ");
					OutputDebugString( lua_tostring( m_L, -1 ) );
				}
			}
		}
	}
}

void LuaWrapper::CreateEvtOpenDoor()
{
	IEventDataPtr e(GCC_NEW EvtData_Unlock_Door() );
	m_em->VQueueEvent( e );
}

void LuaWrapper::CreateEvtPlayMusicSequence()
{
	IEventDataPtr e(GCC_NEW EvtData_Play_Music_Sequence() );
	m_em->VQueueEvent( e );
}

void LuaWrapper::CreateEvtStartMusicPanel()
{
	IEventDataPtr e(GCC_NEW EvtData_Start_MusicPanel() );
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

	// Create Wheel
	m_em->VAddListener(
		   std::bind(	&LuaWrapper::CreateWheel,
						this,
						std::placeholders::_1),
						EvtData_Wheel_Created::sk_EventType);

	// Pull Lever
	m_em->VAddListener(
		   std::bind(	&LuaWrapper::PullLever,
						this,
						std::placeholders::_1),
						EvtData_Lever_Pull::sk_EventType);

	// Rotate Wheel
	m_em->VAddListener(
		   std::bind(	&LuaWrapper::RotateWheel,
						this,
						std::placeholders::_1),
						EvtData_Rotate_Wheel::sk_EventType);
}

//================================================================================================
//									Init scripts and meta data
//================================================================================================
void LuaWrapper::InitMazeMeta()
{
	m_L = m_le->ResetSate();

	//=====================================
	//		Lua Meta Tables Maze
	//=====================================
	//Game
	luaL_newmetatable( m_L, "GameMeta" );

	luaL_Reg GameRegs[] = 
	{
		{ "ResetMaze",	   LuaResetMaze },
		{ "OpenMazeDoor",  LuaOpenMazeDoor },
		{ "CloseMazeDoor", LuaCloseMazeDoor },
		{ "ChangeNavMesh", LuaChangeNavMesh },
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

	m_le->ExecuteFile( "maze.lua" );
}

void LuaWrapper::InitDungeonMeta()
{
	m_L = m_le->ResetSate();

	//=====================================
	//		Lua Meta Tables Dungeon
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
	//		Lua Meta Tables Cave
	//=====================================
	//Game
	luaL_newmetatable( m_L, "GameMeta" );

	luaL_Reg GameRegs[] = 
	{
		{ "OpenDoor", LuaOpenDoor },
		{ "StartMusicPanel", StartMusicPanel },
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
		{ "IsOn", WheelIsOn },
		{ "WheelGetName", WheelGetName },
		{ "WheelGetValue", WheelGetValue },
		//{ "Reset", ResetLever },
		{ NULL, NULL },
	};

	luaL_setfuncs( m_L, WheelRegs, 0 );

	lua_pushvalue( m_L, -1 );

	lua_setfield( m_L, -2, "__index" );

	lua_setglobal( m_L, "Wheel" );

	m_le->ExecuteFile( "Cave.lua" );
}
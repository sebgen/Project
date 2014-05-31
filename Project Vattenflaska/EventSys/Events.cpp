/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: LuaEngine.h
  Defines common game events

-------------------------------------------------------------------------
  History:
  2014-05-25
  Created by Sebastian Genlund

*************************************************************************/

#include "Events.h"

//=============================================================================================
//										Lever Created
//=============================================================================================
const EventType EvtData_Lever_Created::sk_EventType( 0xa66483fc );

//=============================================================================================
//										Wheel Created
//=============================================================================================
const EventType EvtData_Wheel_Created::sk_EventType( 0xd4e2fc16 );

//=============================================================================================
//										Lever Pull
//=============================================================================================
const EventType EvtData_Lever_Pull::sk_EventType( 0xa66483fc );

//=============================================================================================
//										Rotate Wheel
//=============================================================================================
const EventType EvtData_Rotate_Wheel::sk_EventType( 0x4191b33c );

//=============================================================================================
//										Unlock Door
//=============================================================================================
const EventType EvtData_Unlock_Door::sk_EventType( 0xe841f4d6 );

//=============================================================================================
//										Start Music Panel
//=============================================================================================
const EventType EvtData_Start_MusicPanel::sk_EventType( 0xefa287fd );

//=============================================================================================
//										Play Music Sequence
//=============================================================================================
const EventType EvtData_Play_Music_Sequence::sk_EventType( 0xc953e9ba );

//=============================================================================================
//										Change NavMesh
//=============================================================================================
const EventType EvtData_Change_NavMesh::sk_EventType( 0xabdd5913 );

//=============================================================================================
//										Unlock Maze Door
//=============================================================================================
const EventType EvtData_Unlock_Maze_Door::sk_EventType( 0x1d99db7b );

//=============================================================================================
//										Lock Maze Door
//=============================================================================================
const EventType EvtData_Lock_Maze_Door::sk_EventType( 0xa082a891 );
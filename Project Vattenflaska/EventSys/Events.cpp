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
// EvtData_Pull_Lever - Sent from Lever::PullLever when picking is detecting a collision with
//=============================================================================================
const EventType EvtData_Lever_Created::sk_EventType( 0xa66483fc );

//=============================================================================================
// EvtData_Wheel_Created - Sent from Wheels's constructor with a pointer to it self
//=============================================================================================
const EventType EvtData_Wheel_Created::sk_EventType( 0xd4e2fc16 );

//=============================================================================================
// EvtData_Pull_Lever - Sent from Lever::PullLever when picking is detecting a collision with
//=============================================================================================
const EventType EvtData_Lever_Pull::sk_EventType( 0xa66483fc );

//=============================================================================================
// EvtData_Rotate_Wheel - Sent from Wheel::RotateWheel when picking is detecting a collision with
//=============================================================================================
const EventType EvtData_Rotate_Wheel::sk_EventType( 0x4191b33c );

//=============================================================================================
// EvtData_Unlock_Door - Sent from LuaWrapper when lua calls LuaOpenDoor()
//=============================================================================================
const EventType EvtData_Unlock_Door::sk_EventType( 0xe841f4d6 );

//=============================================================================================
// EvtData_Start_MusicPanel - Sent from LuaWrapper when lua calls LuaStartWrapper()
//=============================================================================================
const EventType EvtData_Start_MusicPanel::sk_EventType( 0xefa287fd );

//=============================================================================================
// EvtData_Play_Music_Sequence - Sent from LuaWrapper when lua calls PlayMusicSequence()
//=============================================================================================
const EventType EvtData_Play_Music_Sequence::sk_EventType( 0xc953e9ba );

//=============================================================================================
// EvtData_Change_NavMesh - Sent from lua wrapper to change current navmesh
//=============================================================================================
const EventType EvtData_Change_NavMesh::sk_EventType( 0xabdd5913 );

//=============================================================================================
// EvtData_Unlock_Maze_Door - Sent from LuaWrapper when lua calls LuaOpenDoor()
//=============================================================================================
const EventType EvtData_Unlock_Maze_Door::sk_EventType( 0x1d99db7b );

//=============================================================================================
//				EvtData_Lock_Maze_Door - Sent from LuaWrapper
//=============================================================================================
const EventType EvtData_Lock_Maze_Door::sk_EventType( 0xa082a891 );
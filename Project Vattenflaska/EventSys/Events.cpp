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
// EvtData_Pull_Lever - Sent from Lever::PullLever when picking is detecting a collision with
//=============================================================================================
const EventType EvtData_Lever_Pull::sk_EventType( 0xa66483fc );

//=============================================================================================
// EvtData_Unlock_Door - Sent from LuaWrapper when lua calls LuaOpenDoor()
//=============================================================================================
const EventType EvtData_Unlock_Door::sk_EventType( 0xe841f4d6 );
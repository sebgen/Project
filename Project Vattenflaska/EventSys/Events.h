#pragma once

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


#include "EventManager.h"
class Lever;
class Wheel;

//=============================================================================================
// EvtData_Create_Lever - Sent from Lever's constructor with a pointer to it self
//=============================================================================================
class EvtData_Lever_Created : public BaseEventData
{
    Lever* m_lever;

public:
	static const EventType sk_EventType;

    explicit EvtData_Lever_Created( Lever* lever )
        : m_lever( lever )
    {
    }

	Lever* GetLever() const
	{
		return m_lever;
	}

	virtual const EventType& VGetEventType( void ) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr (GCC_NEW EvtData_Lever_Created ( m_lever ) );
	}

	virtual void VSerialize( std::ostrstream & out )
	{
	}

    virtual const char* GetName(void) const
    {
        return "EvtData_Lever_Created";
    }
};

//=============================================================================================
// EvtData_Wheel_Created - Sent from Wheels's constructor with a pointer to it self
//=============================================================================================
class EvtData_Wheel_Created : public BaseEventData
{
    Wheel* m_wheel;

public:
	static const EventType sk_EventType;

    explicit EvtData_Wheel_Created( Wheel* wheel )
        : m_wheel( wheel )
    {
    }

	Wheel* GetWheel() const
	{
		return m_wheel;
	}

	virtual const EventType& VGetEventType( void ) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr (GCC_NEW EvtData_Wheel_Created ( m_wheel ) );
	}

	virtual void VSerialize( std::ostrstream & out )
	{
	}

    virtual const char* GetName(void) const
    {
        return "EvtData_Wheel_Created";
    }
};

//=============================================================================================
// EvtData_Pull_Lever - Sent from Lever::PullLever when picking is detecting a collision with
//=============================================================================================
class EvtData_Lever_Pull : public BaseEventData
{
    Lever* m_lever;

public:
	static const EventType sk_EventType;

    explicit EvtData_Lever_Pull( Lever* lever )
        : m_lever( lever )
    {
    }

	Lever* GetLever() const
	{
		return m_lever;
	}

	virtual const EventType& VGetEventType( void ) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr (GCC_NEW EvtData_Lever_Pull ( m_lever ) );
	}

	virtual void VSerialize( std::ostrstream & out )
	{
	}

    virtual const char* GetName(void) const
    {
        return "EvtData_Lever_Pull";
    }
};

//=============================================================================================
// EvtData_Rotate_Wheel - Sent from Wheel::RotateWheel when picking is detecting a collision with
//=============================================================================================
class EvtData_Rotate_Wheel : public BaseEventData
{
    Wheel* m_wheel;

public:
	static const EventType sk_EventType;

    explicit EvtData_Rotate_Wheel( Wheel* wheel )
        : m_wheel( wheel )
    {
    }

	Wheel* GetWheel() const
	{
		return m_wheel;
	}

	virtual const EventType& VGetEventType( void ) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr (GCC_NEW EvtData_Rotate_Wheel ( m_wheel ) );
	}

	virtual void VSerialize( std::ostrstream & out )
	{
	}

    virtual const char* GetName(void) const
    {
        return "EvtData_Rotate_Wheel";
    }
};

//=============================================================================================
// EvtData_Unlock_Door - Sent from LuaWrapper when lua calls LuaOpenDoor()
//=============================================================================================
class EvtData_Unlock_Door : public BaseEventData
{
public:
	static const EventType sk_EventType;

	virtual const EventType& VGetEventType( void ) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr (GCC_NEW EvtData_Unlock_Door ( ) );
	}

	virtual void VSerialize( std::ostrstream & out )
	{
	}

    virtual const char* GetName(void) const
    {
        return "EvtData_Unlock_Door";
    }
};
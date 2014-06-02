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
//										Lever Created
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
//										Wheel Created
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
//										Lever Pull
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
//										Rotate Wheel
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
//										Unlock Door
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

//=============================================================================================
//										Start Music Panel
//=============================================================================================
class EvtData_Start_MusicPanel : public BaseEventData
{
public:
	static const EventType sk_EventType;

	virtual const EventType& VGetEventType( void ) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr (GCC_NEW EvtData_Start_MusicPanel ( ) );
	}

	virtual void VSerialize( std::ostrstream & out )
	{
	}

    virtual const char* GetName(void) const
    {
        return "EvtData_Start_MusicPanel";
    }
};

//=============================================================================================
//										Play Music Sequence
//=============================================================================================
class EvtData_Play_Music_Sequence : public BaseEventData
{
public:
	static const EventType sk_EventType;

	virtual const EventType& VGetEventType( void ) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr (GCC_NEW EvtData_Play_Music_Sequence ( ) );
	}

	virtual void VSerialize( std::ostrstream & out )
	{
	}

    virtual const char* GetName(void) const
    {
        return "EvtData_Play_Music_Sequence";
    }
};

//=============================================================================================
//										Change NavMesh
//=============================================================================================
class EvtData_Change_NavMesh : public BaseEventData
{
    int m_index;

public:
	static const EventType sk_EventType;

    explicit EvtData_Change_NavMesh( int index )
        : m_index( index )
    {
    }

	int GetIndex() const
	{
		return m_index;
	}

	virtual const EventType& VGetEventType( void ) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr (GCC_NEW EvtData_Change_NavMesh ( m_index ) );
	}

	virtual void VSerialize( std::ostrstream & out )
	{
	}

    virtual const char* GetName(void) const
    {
        return "EvtData_Change_NavMesh";
    }
};

//=============================================================================================
//										Unlock Maze Door
//=============================================================================================
class EvtData_Unlock_Maze_Door : public BaseEventData
{
    const char* m_doorName;

public:
	static const EventType sk_EventType;

    explicit EvtData_Unlock_Maze_Door( const char* doorName )
        : m_doorName( doorName )
    {
    }

	const char* GetDoorName() const
	{
		return m_doorName;
	}

	virtual const EventType& VGetEventType( void ) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr (GCC_NEW EvtData_Unlock_Maze_Door ( m_doorName ) );
	}

	virtual void VSerialize( std::ostrstream & out )
	{
	}

    virtual const char* GetName(void) const
    {
        return "EvtData_Unlock_Maze_Door";
    }
};

//=============================================================================================
//										Lock Maze Door
//=============================================================================================
class EvtData_Lock_Maze_Door : public BaseEventData
{
    const char* m_doorName;

public:
	static const EventType sk_EventType;

    explicit EvtData_Lock_Maze_Door( const char* doorName )
        : m_doorName( doorName )
    {
    }

	const char* GetDoorName() const
	{
		return m_doorName;
	}

	virtual const EventType& VGetEventType( void ) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr (GCC_NEW EvtData_Lock_Maze_Door ( m_doorName ) );
	}

	virtual void VSerialize( std::ostrstream & out )
	{
	}

    virtual const char* GetName(void) const
    {
        return "EvtData_Lock_Maze_Door";
    }
};

//=============================================================================================
//										Reset Maze
//=============================================================================================
class EvtData_Reset_Maze : public BaseEventData
{
public:
	static const EventType sk_EventType;

	virtual const EventType& VGetEventType( void ) const
	{
		return sk_EventType;
	}

	virtual IEventDataPtr VCopy() const
	{
		return IEventDataPtr (GCC_NEW EvtData_Reset_Maze ( ) );
	}

	virtual void VSerialize( std::ostrstream & out )
	{
	}

    virtual const char* GetName(void) const
    {
        return "EvtData_Reset_Maze";
    }
};

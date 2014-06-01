#include "Room.h"

Room::Room()
{
	m_roomName = "";
}

Room::Room( std::string roomName, EventManager* em )
{
	m_roomName = roomName;
	m_em = em;

	// Open Door
	m_em->VAddListener(
		   std::bind(	&Room::OpenMazeDoor,
						this,
						std::placeholders::_1),
						EvtData_Unlock_Maze_Door::sk_EventType);

	// Close Door
	m_em->VAddListener(
		   std::bind(	&Room::CloseMazeDoor,
						this,
						std::placeholders::_1),
						EvtData_Lock_Maze_Door::sk_EventType);

	// Reset Maze
	m_em->VAddListener(
		   std::bind(	&Room::ResetMaze,
						this,
						std::placeholders::_1),
						EvtData_Reset_Maze::sk_EventType);
}

Room::~Room()
{}

void Room::AddDoor( Door* door )
{
	m_doors.push_back( door );
}

void Room::AddLever( Lever* lever )
{
	m_levers.push_back( lever );
}

void Room::AddWheel( Wheel* wheel )
{
	m_wheels.push_back( wheel );
}

void Room::AddTorch( Torch* torch )
{
	m_torches.push_back( torch );
}

void Room::AddLight( const GlobalLight& light )
{
	m_lights.push_back( light );
}

void Room::AddContent( TestObject* object )
{
	m_roomContent.push_back( object );
}

std::string Room::GetRoomName() const
{
	return m_roomName;
}
std::vector<MeshInfo> Room::getTorchMesh()
{
	std::vector<MeshInfo> temp;
	for(int i=0; i< m_torches.size(); i++)
	{
		temp.push_back(m_torches.at(i)->getInfo());
	}
	return temp;
}
std::vector<MeshInfo> Room::getDoorMesh()
{
	std::vector<MeshInfo> temp;
	for(int i=0; i< m_roomContent.size(); i++)
	{
		if(m_roomContent.at(i)->GetGroupName()=="door_exitShape" || m_roomContent.at(i)->GetGroupName()=="door_ExitShape")
		{
			temp.push_back(m_roomContent.at(i)->getInfo());
		}
	}
	return temp;
}

std::vector<MeshInfo> Room::GetLever() 
{
	std::vector<MeshInfo> temp;
	for(int i=0; i< m_levers.size(); i++)
	{
		temp.push_back(m_levers.at(i)->getInfo());
	}
	return temp;
}

std::vector<MeshInfo> Room::GetWheel()
{
	std::vector<MeshInfo> temp;
	for(int i=0; i< m_wheels.size(); i++)
	{
		temp.push_back(m_wheels.at(i)->getInfo());
	}
	return temp;
}


Door* Room::GetADoor( int index ) const
{
	return m_doors.at( index );
}

const std::vector<GlobalLight>& Room::GetLights() const
{
	return m_lights;
}

void Room::OpenMazeDoor( IEventDataPtr pEventData )
{
	if( EvtData_Unlock_Maze_Door::sk_EventType == pEventData->VGetEventType() )
	{
		shared_ptr<EvtData_Unlock_Maze_Door> door = std::static_pointer_cast<EvtData_Unlock_Maze_Door>( pEventData );
		for( int i = 0; i < m_doors.size(); i++ )
		{
			if( strcmp(m_doors.at( i )->GetName().c_str(), door->GetDoorName() ) == 0 )
			{
				m_doors.at( i )->OpenDoor( pEventData );
			}
		}
	}
}

void Room::CloseMazeDoor( IEventDataPtr pEventData )
{
	if( EvtData_Lock_Maze_Door::sk_EventType == pEventData->VGetEventType() )
	{
		shared_ptr<EvtData_Lock_Maze_Door> door = std::static_pointer_cast<EvtData_Lock_Maze_Door>( pEventData );
		for( int i = 0; i < m_doors.size(); i++ )
		{
			if( strcmp(m_doors.at( i )->GetName().c_str(), door->GetDoorName() ) == 0 )
			{
				m_doors.at( i )->CloseDoor( pEventData );
			}
		}
	}
}

void Room::ResetMaze( IEventDataPtr pEventData )
{
	for( int i = 0; i < m_doors.size(); i++ )
		m_doors.at( i )->CloseDoor( pEventData );
}

Lever* Room::GetALever( int index ) const
{
	return m_levers.at( index );
}

Wheel* Room::GetAWheel( int index ) const
{
	return m_wheels.at( index );
}

std::vector<Lever*> Room::GetLevers() const
{
	return m_levers;
}

std::vector<Door*> Room::GetDoors()
{
	return m_doors;
}

HRESULT Room::Update( float deltaTime, Camera* camera )
{	
	HRESULT hr = S_OK;

	// Update content
	for (int i = 0; i < m_roomContent.size(); i++)
		m_roomContent.at(i)->Update( deltaTime, camera );

	for (int i = 0; i < m_torches.size(); i++)
		m_torches.at(i)->Update( deltaTime, camera );

	for (int i = 0; i < m_levers.size(); i++)
		m_levers.at(i)->Update( deltaTime, camera );

	for (int i = 0; i < m_doors.size(); i++)
		m_doors.at(i)->Update( deltaTime, camera );

	for (int i = 0; i < m_wheels.size(); i++)
		m_wheels.at(i)->Update( deltaTime, camera );

	return hr;
}

HRESULT Room::Draw( float deltaTime )
{
	HRESULT hr = S_OK;

	// Draw content
	for (int i = 0; i < m_roomContent.size(); i++)
		m_roomContent.at(i)->Draw( deltaTime );

	for (int i = 0; i < m_torches.size(); i++)
		m_torches.at(i)->Draw( deltaTime );

	for (int i = 0; i < m_levers.size(); i++)
		m_levers.at(i)->Draw( deltaTime );

	for (int i = 0; i < m_doors.size(); i++)
		m_doors.at(i)->Draw( deltaTime );

	for (int i = 0; i < m_wheels.size(); i++)
		m_wheels.at(i)->Draw( deltaTime );

	return hr;
}

void Room::Shutdown()
{

}
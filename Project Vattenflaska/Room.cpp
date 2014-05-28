#include "Room.h"

Room::Room()
{
	m_roomName = "";
}

Room::Room( std::string roomName )
{
	m_roomName = roomName;
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

std::vector<MeshInfo> Room::getDoorMesh()
{
	std::vector<MeshInfo> temp;
	for(int i=0; i< m_roomContent.size(); i++)
	{
		if(m_roomContent.at(i)->GetGroupName()=="door_exitShape")
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


Door* Room::GetDoor() const
{
	return m_doors.front();
}

const std::vector<GlobalLight>& Room::GetLights() const
{
	return m_lights;
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
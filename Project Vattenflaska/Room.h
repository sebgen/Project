/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: Room.h
  
-------------------------------------------------------------------------
  History:
  2014-05-13
  Created by Simon Johanson

*************************************************************************/

#ifndef _ROOM_H_
#define _ROOM_H_

#include "EventSys/EventManagerImpl.h"
#include "EventSys/Events.h"
#include "Lever.h"
#include "Door.h"
#include "Torch.h"
#include "GlobalLight.h"
#include "Wheel.h"

#include "TestObject.h"


class Room
{
	private:
		std::string			m_roomName;
		EventManager*		m_em;

		std::vector<TestObject*>	m_roomContent;
		std::vector<Lever*>			m_levers;
		std::vector<Wheel*>			m_wheels;
		std::vector<Door*>			m_doors;
		std::vector<Torch*>			m_torches;
		std::vector<GlobalLight>	m_lights;

	public:
		Room();
		Room( std::string roomName, EventManager* em );
		~Room();

		void AddDoor( Door* door ); // Adds a door to the room
		void AddLever( Lever* lever ); // Adds a lever to the room
		void AddWheel( Wheel* wheel );
		void AddTorch( Torch* torch ); // Adds a torch to the room
		void AddLight( const GlobalLight& light ); // Adds a pointlight to the room
		void AddContent( TestObject* object ); // Adds content to the room

		std::string GetRoomName() const;
		std::vector<MeshInfo> GetLever();
		std::vector<MeshInfo> GetWheel();
		std::vector<MeshInfo> getTorchMesh();
		//std::vector<MeshInfo> getExitDoor();
		std::vector<MeshInfo> getDoorMesh();
		const std::vector<GlobalLight>& GetLights() const;
		
		void OpenMazeDoor( IEventDataPtr pEventData ); 
		void CloseMazeDoor( IEventDataPtr pEventData ); 
		void ResetMaze( IEventDataPtr pEventData );

		Lever* GetALever( int index ) const;
		Wheel* GetAWheel( int index ) const;
		Door* GetADoor( int index ) const;

		std::vector<Lever*> GetLevers() const;
		std::vector< Door*> GetDoors();
		

		HRESULT Update( float deltaTime, Camera* camera );
		HRESULT Draw( float deltaTime );

		void Shutdown();
};
#endif
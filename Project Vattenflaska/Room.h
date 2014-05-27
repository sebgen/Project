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

#include "Lever.h"
#include "Door.h"
#include "Torch.h"
#include "GlobalLight.h"

#include "TestObject.h"


class Room
{
	private:
		std::string			m_roomName;

		std::vector<TestObject*> m_roomContent;
		std::vector<Lever*> m_levers;
		std::vector<Wheel*> m_wheels;
		std::vector<Door*>  m_doors;
		std::vector<Torch*> m_torches;
		std::vector<GlobalLight> m_lights;

	public:
		Room();
		Room( std::string roomName );
		~Room();

		void AddDoor( Door* door ); // Adds a door to the room
		void AddLever( Lever* lever ); // Adds a lever to the room
		void AddWheel( Wheel* wheel );
		void AddTorch( Torch* torch ); // Adds a torch to the room
		void AddLight( const GlobalLight& light ); // Adds a pointlight to the room
		void AddContent( TestObject* object ); // Adds content to the room

		std::vector<MeshInfo> GetLever();
		std::vector<MeshInfo> GetWheel();
		//std::vector<MeshInfo> getExitDoor();
		std::vector<MeshInfo> getDoorMesh();
		Door* GetDoor() const;
		const std::vector<GlobalLight>& GetLights() const;

		Lever* GetALever( int index ) const;
		std::vector<Lever*> GetLevers() const;
		

		HRESULT Update( float deltaTime, Camera* camera );
		HRESULT Draw( float deltaTime );

		void Shutdown();
};
#endif
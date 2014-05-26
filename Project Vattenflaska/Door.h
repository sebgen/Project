/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: Door.h
  
-------------------------------------------------------------------------
  History:
  2014-05-13
  Created by Simon Johanson

*************************************************************************/

#ifndef _DOOR_H_
#define _DOOR_H_

#include "EventSys/EventManagerImpl.h"
#include "EventSys/Events.h"
#include "DrawableObject.h"

class Door : public DrawableObject
{
	private:
		bool  m_isOpening; // True when door is moving
		bool  m_isOpen;    // True when door is fully open
		float m_animationTimer; // Makes sure that animation is equal on all systems
		int   m_moveUnits; // Number of units the door will move

	public:
		Door();
		Door( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo );
		~Door();

		void OpenDoor(); // Tell Door to open
		void RaiseDoor();
		void UpdateAndSetVertexBuffer();

		bool IsOpen() const;

		virtual HRESULT Update( float deltaTime, Camera* camera );
		virtual HRESULT Draw( float deltaTime );

		void Shutdown();
};
#endif
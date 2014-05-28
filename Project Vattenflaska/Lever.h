/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: Lever.h
  
-------------------------------------------------------------------------
  History:
  2014-05-13
  Created by Simon Johanson

  2014-05-26
  Edited by Sebastian Genlund
  Added events

*************************************************************************/

#ifndef _LEVER_H_
#define _LEVER_H_

#include "EventSys/EventManagerImpl.h"
#include "EventSys/Events.h"
#include "DrawableObject.h"
#include "Sound.h"

class Lever : public DrawableObject
{
	private: // Lua / Event
		bool			m_isOn;
		std::string		m_name;
		EventManager*	m_em;
		float			m_rotationX;
		Sound*			m_sound;

	public:
		Lever();
		Lever( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo, EventManager* em );
		~Lever();
		MeshInfo getInfo();
		virtual HRESULT Update( float deltaTime, Camera* camera );
		virtual HRESULT Draw( float deltaTime );
		
		void Shutdown();

	public: // Lua / Event
		void PullLever();
		bool IsOn() const;
		std::string GetName() const;
		void ResetLever();
};

#endif
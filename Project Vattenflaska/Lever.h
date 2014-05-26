/*************************************************************************
  Project Vattenflaska Source File.
-------------------------------------------------------------------------
  Description: Lever.h
  
-------------------------------------------------------------------------
  History:
  2014-05-13
  Created by Simon Johanson

*************************************************************************/

#ifndef _LEVER_H_
#define _LEVER_H_

#include "DrawableObject.h"

class Lever : public DrawableObject
{
	public:
		Lever();
		Lever( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo );
		~Lever();

		virtual HRESULT Update( float deltaTime, Camera* camera );
		virtual HRESULT Draw( float deltaTime );
		
		void Shutdown();
};

#endif
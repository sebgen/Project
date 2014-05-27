#ifndef _TESTOBJECT_H_
#define _TESTOBJECT_H_

#include "DrawableObject.h"

class TestObject : public DrawableObject
{
	public:
		TestObject();
		TestObject( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo );
		~TestObject();

		float rotationY;

		virtual HRESULT Update( float deltaTime, Camera* camera );
		virtual HRESULT Draw( float deltaTime );

		MeshInfo getInfo();

		void Shutdown();

};
#endif
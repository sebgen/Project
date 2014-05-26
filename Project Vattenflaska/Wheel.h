#ifndef _WHEEL_H_
#define _WHEEL_H_

#include "DrawableObject.h"

class Wheel : public DrawableObject
{
	private:
		bool m_isRotating;
		float m_rotationTimer;

	public:
		Wheel();
		Wheel( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo );
		~Wheel();

		void RotateWheel();
		virtual HRESULT Update( float deltaTime, Camera* camera );
		virtual HRESULT Draw( float deltaTime );

		void Shutdown();
};
#endif
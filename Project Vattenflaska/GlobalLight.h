#ifndef _GLOBALLIGHT_H_
#define _GLOBALLIGHT_H_

#include <DirectXMath.h>
using namespace DirectX;

class GlobalLight
{
	public:
		GlobalLight();
		GlobalLight( XMFLOAT4 &diffuse, XMFLOAT3 &position, float &range );
		~GlobalLight();

		XMFLOAT4 m_diffuse;
		XMFLOAT4 m_positionAndRange;
	
		float m_intensity;
		XMFLOAT3 padding;
	
		void Update( float deltaTime );
};
#endif
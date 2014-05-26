#ifndef _TORCH_H_
#define _TORCH_H_

#include "DrawableObject.h"

class Torch : public DrawableObject
{
	public:
		Torch();
		Torch( ID3D11Device* device, ID3D11DeviceContext* deviceContext, MeshInfo meshInfo );
		~Torch();

		virtual HRESULT Update( float deltaTime, Camera* camera );
		virtual HRESULT Draw( float deltaTime );

		void Shutdown();
};
#endif